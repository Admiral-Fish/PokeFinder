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
#include <QSettings>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>

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

    ui->ivFilter->setOrientation(Qt::Horizontal);

    connect(ui->profileDisplay, &ProfileDisplay5::profileChanged, this, &AdjacentSeeds::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay5::profilesChanged, this, &AdjacentSeeds::profilesChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &AdjacentSeeds::generate);
    connect(ui->comboBoxPreviewMode, &QComboBox::currentIndexChanged, this, &AdjacentSeeds::updatePreview);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &AdjacentSeeds::updatePreview);

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
    if (!ui->ivFilter->isValid())
    {
        return;
    }

    ui->tableView->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
    model->clearModel();

    u32 initialIVAdvance = ui->textBoxMinIVAdvance->getUInt();
    u32 maxIVAdvance = initialIVAdvance + ui->textBoxMaxIVAdvance->getUInt();
    int seconds = ui->spinBoxSeconds->value();
    bool roamer = ui->comboBoxMethod->currentIndex() == roamerIndex;
    auto minIVs = ui->ivFilter->getMinIVs();
    auto maxIVs = ui->ivFilter->getMaxIVs();
    auto buttons = ui->checkListKeypresses->getEnum<Buttons>();
    DateTime dateTime = ui->dateTimeEdit->getDateTime();

    auto states = AdjacentSeedsCalculator::generate(initialIVAdvance, maxIVAdvance, seconds, roamer, buttons, dateTime, minIVs, maxIVs,
                                                    *currentProfile);
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
