/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "AdjacentSeeds.hpp"
#include "ui_AdjacentSeeds.h"
#include <Core/Enum/Buttons.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Tools/AdjacentSeedsCalculator.hpp>
#include <Core/Util/DateTime.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Form/Util/IVCalculator.hpp>
#include <Model/Gen5/AdjacentSeedsModel.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QSpinBox>
#include <QStringList>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>
#include <algorithm>

static const QString settingPrefix = QStringLiteral("adjacentSeeds");

constexpr u32 roamerIndex = 1;

class TargetRowDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override
    {
        QStyledItemDelegate::initStyleOption(option, index);
        if (index.data(Qt::UserRole).toBool())
        {
            option->font.setBold(true);
        }
    }
};

AdjacentSeeds::AdjacentSeeds(QWidget *parent) : QWidget(parent), ui(new Ui::AdjacentSeeds)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->profileDisplay->setup(settingPrefix, Game::Gen5);

    model = new AdjacentSeedsModel(ui->tableView);
    proxyModel = new SortFilterProxyModel(ui->tableView, model);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setItemDelegate(new TargetRowDelegate(ui->tableView));

    ui->checkListKeypresses->setFull(false);
    for (int i = 0; i < 12; i++)
    {
        ui->checkListKeypresses->addItem(Translator::getKeypress(i), 1 << i);
    }

    ui->textBoxMinIVAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxMaxIVAdvance->setValues(InputType::Advance32Bit);

    connect(ui->profileDisplay, &ProfileDisplay5::profileChanged, this, &AdjacentSeeds::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay5::profilesChanged, this, &AdjacentSeeds::profilesChanged);
    connect(ui->pushButtonIVCalculator, &QPushButton::clicked, this, &AdjacentSeeds::openIVCalculator);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &AdjacentSeeds::generate);
    connect(ui->pushButtonResetIVs, &QPushButton::clicked, this, &AdjacentSeeds::resetIVs);
    connect(ui->comboBoxPreviewMode, &QComboBox::currentIndexChanged, this, &AdjacentSeeds::updatePreview);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            [=](const QModelIndex &, const QModelIndex &) { updatePreview(); });

    updateProfiles();
}

AdjacentSeeds::AdjacentSeeds(bool roamer, Buttons buttons, const DateTime &dateTime, const Profile5 &profile, QWidget *parent) :
    AdjacentSeeds(parent)
{
    if (roamer)
    {
        ui->comboBoxMethod->setCurrentIndex(roamerIndex);
    }
    ui->checkListKeypresses->setEnum(buttons);
    ui->profileDisplay->setProfile(profile);
    ui->dateTimeEdit->setDateTime(dateTime);
}

AdjacentSeeds::~AdjacentSeeds()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("seconds", ui->spinBoxSeconds->value());
    setting.setValue("minIVAdvance", ui->textBoxMinIVAdvance->text());
    setting.setValue("maxIVAdvance", ui->textBoxMaxIVAdvance->text());
    setting.endGroup();

    delete ui;
}

bool AdjacentSeeds::hasProfiles() const
{
    return ui->profileDisplay->hasProfiles();
}

void AdjacentSeeds::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void AdjacentSeeds::generate()
{
    std::array<QSpinBox *, 6> minIVWidgets
        = { ui->spinBoxHPMin, ui->spinBoxAtkMin, ui->spinBoxDefMin, ui->spinBoxSpAMin, ui->spinBoxSpDMin, ui->spinBoxSpeMin };
    std::array<QSpinBox *, 6> maxIVWidgets
        = { ui->spinBoxHPMax, ui->spinBoxAtkMax, ui->spinBoxDefMax, ui->spinBoxSpAMax, ui->spinBoxSpDMax, ui->spinBoxSpeMax };
    QStringList stats = { tr("HP"), tr("Atk"), tr("Def"), tr("SpAtk"), tr("SpDef"), tr("Spe") };
    for (size_t i = 0; i < minIVWidgets.size(); i++)
    {
        if (minIVWidgets[i]->value() > maxIVWidgets[i]->value())
        {
            QMessageBox msg(QMessageBox::Warning, tr("Invalid IV settings"), tr("%1 minimum is greater than maximum").arg(stats[i]));
            msg.exec();
            return;
        }
    }

    ui->tableView->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
    proxyModel->sort(-1);
    model->clearModel();

    DateTime dateTime = ui->dateTimeEdit->getDateTime();
    auto buttons = ui->checkListKeypresses->getEnum<Buttons>();
    int seconds = ui->spinBoxSeconds->value();
    u32 initialIVAdvance = ui->textBoxMinIVAdvance->getUInt();
    u32 maxIVAdvance = initialIVAdvance + ui->textBoxMaxIVAdvance->getUInt();
    bool roamer = ui->comboBoxMethod->currentIndex() == roamerIndex;

    auto states = AdjacentSeedsCalculator::generate(initialIVAdvance, maxIVAdvance, seconds, roamer, buttons, dateTime, *currentProfile);
    std::erase_if(states, [=](const AdjacentSeedsState &state) {
        for (size_t i = 0; i < minIVWidgets.size(); i++)
        {
            if (state.getIV(static_cast<u8>(i)) < minIVWidgets[i]->value() || state.getIV(static_cast<u8>(i)) > maxIVWidgets[i]->value())
            {
                return true;
            }
        }
        return false;
    });
    model->addItems(states);

    if (model->rowCount() > 0)
    {
        int targetRow = 0;
        for (int row = 0; row < model->rowCount(); row++)
        {
            const auto &state = model->getItem(row);
            if (state.isTarget())
            {
                targetRow = row;
                break;
            }
        }
        ui->tableView->selectRow(proxyModel->mapFromSource(model->index(targetRow, 0)).row());
    }

    updatePreview();
}

void AdjacentSeeds::openIVCalculator()
{
    auto *calculator = new IVCalculator();
    connect(calculator, &IVCalculator::ivsCalculated, this, &AdjacentSeeds::updateIVs);
    calculator->show();
}

void AdjacentSeeds::resetIVs()
{
    std::array<QSpinBox *, 6> minIVs
        = { ui->spinBoxHPMin, ui->spinBoxAtkMin, ui->spinBoxDefMin, ui->spinBoxSpAMin, ui->spinBoxSpDMin, ui->spinBoxSpeMin };
    std::array<QSpinBox *, 6> maxIVs
        = { ui->spinBoxHPMax, ui->spinBoxAtkMax, ui->spinBoxDefMax, ui->spinBoxSpAMax, ui->spinBoxSpDMax, ui->spinBoxSpeMax };
    for (size_t i = 0; i < minIVs.size(); i++)
    {
        minIVs[i]->setValue(0);
        maxIVs[i]->setValue(31);
    }
}

void AdjacentSeeds::updateIVs(const std::array<std::vector<u8>, 6> &ivs)
{
    std::array<QSpinBox *, 6> minIVs
        = { ui->spinBoxHPMin, ui->spinBoxAtkMin, ui->spinBoxDefMin, ui->spinBoxSpAMin, ui->spinBoxSpDMin, ui->spinBoxSpeMin };
    std::array<QSpinBox *, 6> maxIVs
        = { ui->spinBoxHPMax, ui->spinBoxAtkMax, ui->spinBoxDefMax, ui->spinBoxSpAMax, ui->spinBoxSpDMax, ui->spinBoxSpeMax };
    for (size_t i = 0; i < ivs.size(); i++)
    {
        const auto &iv = ivs[i];
        minIVs[i]->setValue(iv.empty() ? 0 : iv.front());
        maxIVs[i]->setValue(iv.empty() ? 31 : iv.back());
    }
}

void AdjacentSeeds::profileChanged(const Profile5 &profile)
{
    currentProfile = &profile;
}

void AdjacentSeeds::updatePreview()
{
    constexpr u32 chatotIndex = 0;
    constexpr u32 previewCount = 25;

    if (model->rowCount() == 0)
    {
        ui->lineEditPreview->clear();
        return;
    }

    QModelIndex index = ui->tableView->selectionModel()->currentIndex();
    if (!index.isValid())
    {
        ui->lineEditPreview->clear();
        return;
    }

    index = proxyModel->mapToSource(index);
    const auto &state = model->getItem(index.row());
    std::string preview = AdjacentSeedsCalculator::previewPRNG(state.getSeed(), state.getPIDAdvance(), previewCount,
                                                               ui->comboBoxPreviewMode->currentIndex() == chatotIndex);

    ui->lineEditPreview->setText(QString::fromStdString(preview));
    ui->lineEditPreview->setCursorPosition(0);
}
