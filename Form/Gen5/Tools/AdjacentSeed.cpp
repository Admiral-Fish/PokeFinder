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

#include "AdjacentSeed.hpp"
#include "ui_AdjacentSeed.h"
#include <Core/Enum/Buttons.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/DateTime.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Form/Util/IVCalculator.hpp>
#include <Model/Gen5/AdjacentSeedModel.hpp>
#include <QAbstractItemView>
#include <QEvent>
#include <QItemSelectionModel>
#include <QMouseEvent>
#include <QSettings>
#include <QStandardItemModel>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>
#include <QTimer>
#include <array>

constexpr u32 previewCount = 25;
constexpr int chatotPreview = 0;
constexpr int needlePreview = 1;

constexpr std::array<Buttons, 12> keypressButtons
    = { Buttons::Start, Buttons::Select, Buttons::A, Buttons::B, Buttons::Right, Buttons::Left,
        Buttons::Up,    Buttons::Down,   Buttons::R, Buttons::L, Buttons::X,     Buttons::Y };

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

AdjacentSeed::AdjacentSeed(QWidget *parent) : QWidget(parent), ui(new Ui::AdjacentSeed)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new AdjacentSeedModel(ui->tableView);
    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new TargetRowDelegate(ui->tableView));
    keypressModel = new QStandardItemModel(this);
    ui->comboBoxKeypresses->setModel(keypressModel);
    ui->comboBoxKeypresses->setEditable(true);
    ui->comboBoxKeypresses->lineEdit()->setReadOnly(true);
    ui->comboBoxKeypresses->lineEdit()->installEventFilter(this);
    ui->comboBoxKeypresses->setMaxVisibleItems(static_cast<int>(keypressButtons.size()));
    ui->comboBoxKeypresses->view()->setMinimumHeight(190);
    ui->comboBoxKeypresses->view()->setMaximumHeight(205);

    ui->textBoxMinIVAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxMaxIVAdvance->setValues(InputType::Advance32Bit);

    ui->comboBoxMethod->addItem(tr("Wild / Static / Grotto"), static_cast<int>(AdjacentSeedMethod::Standard));
    ui->comboBoxMethod->addItem(tr("Roamer"), static_cast<int>(AdjacentSeedMethod::Roamer));

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &AdjacentSeed::profileIndexChanged);
    connect(ui->comboBoxKeypresses->view(), &QAbstractItemView::pressed, this, &AdjacentSeed::keypressIndexPressed);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &AdjacentSeed::profileManager);
    connect(ui->pushButtonIVCalculator, &QPushButton::clicked, this, &AdjacentSeed::openIVCalculator);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &AdjacentSeed::generate);
    connect(ui->comboBoxPreviewMode, &QComboBox::currentIndexChanged, this, &AdjacentSeed::updatePreview);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            [=](const QModelIndex &, const QModelIndex &) { updatePreview(); });

    updateProfiles();
}

AdjacentSeed::AdjacentSeed(const DateTime &dateTime, Buttons buttons, AdjacentSeedMethod method, QWidget *parent) : AdjacentSeed(parent)
{
    setInitialSearch(dateTime, buttons);
    setMethod(method);
}

AdjacentSeed::AdjacentSeed(const DateTime &dateTime, Buttons buttons, const Profile5 &profile, AdjacentSeedMethod method, QWidget *parent) :
    AdjacentSeed(parent)
{
    setProfile(profile);
    setInitialSearch(dateTime, buttons);
    setMethod(method);
}

AdjacentSeed::~AdjacentSeed()
{
    QSettings setting;
    setting.beginGroup("AdjacentSeed");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("seconds", ui->spinBoxSeconds->value());
    setting.setValue("minIVAdvance", ui->textBoxMinIVAdvance->text());
    setting.setValue("maxIVAdvance", ui->textBoxMaxIVAdvance->text());
    setting.endGroup();

    delete ui;
}

bool AdjacentSeed::hasProfiles() const
{
    return !profiles.empty();
}

bool AdjacentSeed::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->comboBoxKeypresses->lineEdit() && event->type() == QEvent::MouseButtonPress)
    {
        auto *mouse = static_cast<QMouseEvent *>(event);
        if (mouse->button() == Qt::LeftButton)
        {
            ui->comboBoxKeypresses->showPopup();
            return true;
        }
    }

    return QWidget::eventFilter(object, event);
}

void AdjacentSeed::setInitialSearch(const DateTime &dateTime, Buttons buttons)
{
    ui->dateTimeEdit->setDateTime(dateTime);
    setSelectedButtons(buttons);
}

void AdjacentSeed::setMethod(AdjacentSeedMethod method)
{
    int index = ui->comboBoxMethod->findData(static_cast<int>(method));
    if (index != -1)
    {
        ui->comboBoxMethod->setCurrentIndex(index);
    }
}

void AdjacentSeed::setProfile(const Profile5 &profile)
{
    for (int i = 0; i < static_cast<int>(profiles.size()); i++)
    {
        if (profiles[i] == profile)
        {
            ui->comboBoxProfiles->setCurrentIndex(i);
            return;
        }
    }
}

Buttons AdjacentSeed::getSelectedButtons() const
{
    u16 buttons = 0;
    for (int i = 0; i < keypressModel->rowCount(); i++)
    {
        auto *item = keypressModel->item(i);
        if (item->checkState() == Qt::Checked)
        {
            buttons |= static_cast<u16>(item->data().toUInt());
        }
    }

    return static_cast<Buttons>(buttons);
}

void AdjacentSeed::setSelectedButtons(Buttons buttons)
{
    currentButtons = buttons;
    for (int i = 0; i < keypressModel->rowCount(); i++)
    {
        auto *item = keypressModel->item(i);
        item->setCheckState((buttons & static_cast<Buttons>(item->data().toUInt())) != Buttons::None ? Qt::Checked : Qt::Unchecked);
    }
    updateKeypressText();
}

void AdjacentSeed::updateProfiles()
{
    currentButtons = getSelectedButtons();
    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int profileIndex = setting.value("AdjacentSeed/profile", 0).toInt();
    if (profileIndex < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(profileIndex);
    }
}

void AdjacentSeed::generate()
{
    if (!currentProfile)
    {
        return;
    }

    u32 initialIVAdvance = ui->textBoxMinIVAdvance->getUInt();
    u32 maxIVAdvance = initialIVAdvance + ui->textBoxMaxIVAdvance->getUInt();

    DateTime dateTime = ui->dateTimeEdit->getDateTime();

    AdjacentSeedSettings settings { *currentProfile,
                                    dateTime,
                                    getSelectedButtons(),
                                    static_cast<u32>(ui->spinBoxSeconds->value()),
                                    initialIVAdvance,
                                    maxIVAdvance,
                                    static_cast<AdjacentSeedMethod>(ui->comboBoxMethod->currentData().toUInt()) };

    model->clearModel();
    ui->lineEditPreview->clear();
    model->addItems(AdjacentSeedCalculator::generate(settings));
    ui->tableView->resizeColumnsToContents();
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
        ui->tableView->selectRow(targetRow);
    }
    updatePreview();
}

void AdjacentSeed::keypressIndexPressed(const QModelIndex &index)
{
    auto *item = keypressModel->item(index.row());
    if (!item)
    {
        return;
    }

    item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    currentButtons = getSelectedButtons();
    QTimer::singleShot(0, this, &AdjacentSeed::updateKeypressText);
}

void AdjacentSeed::openIVCalculator()
{
    auto *calculator = new IVCalculator();
    calculator->show();
}

void AdjacentSeed::profileIndexChanged(int index)
{
    if (index < 0 || index >= static_cast<int>(profiles.size()))
    {
        currentProfile = nullptr;
        keypressModel->clear();
        updateKeypressText();
        return;
    }

    currentProfile = &profiles[index];

    ui->labelProfileMACAddressValue->setText(QString::number(currentProfile->getMac(), 16));
    ui->labelProfileDSTypeValue->setText(QString::fromStdString(currentProfile->getDSTypeString()));
    ui->labelProfileVCountValue->setText(QString::number(currentProfile->getVCount(), 16));
    ui->labelProfileTimer0Value->setText(QString::number(currentProfile->getTimer0Min(), 16) + "-"
                                         + QString::number(currentProfile->getTimer0Max(), 16));
    ui->labelProfileGxStatValue->setText(QString::number(currentProfile->getGxStat()));
    ui->labelProfileVFrameValue->setText(QString::number(currentProfile->getVFrame()));
    ui->labelProfileKeypressesValue->setText(QString::fromStdString(currentProfile->getKeypressesString()));
    ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));

    keypressModel->clear();
    for (Buttons button : keypressButtons)
    {
        auto *item = new QStandardItem(QString::fromStdString(Translator::getKeypresses(button)));
        item->setData(toInt(button));
        item->setCheckable(true);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState((currentButtons & button) != Buttons::None ? Qt::Checked : Qt::Unchecked);
        keypressModel->appendRow(item);
    }
    updateKeypressText();
}

void AdjacentSeed::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) {
        emit profilesModified(num);
        updateProfiles();
    });
    manager->show();
}

void AdjacentSeed::updateKeypressText()
{
    Buttons buttons = getSelectedButtons();
    ui->comboBoxKeypresses->lineEdit()->setText(buttons == Buttons::None ? tr("None")
                                                                         : QString::fromStdString(Translator::getKeypresses(buttons)));
}

void AdjacentSeed::updatePreview()
{
    if (!currentProfile || model->rowCount() == 0)
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

    const auto &state = model->getItem(index.row());
    QStringList preview;
    for (u32 prng : AdjacentSeedCalculator::previewPRNG(state.getSeed(), *currentProfile, state.getPIDAdvance(), previewCount))
    {
        if (ui->comboBoxPreviewMode->currentData().toInt() == needlePreview)
        {
            u8 needle = static_cast<u8>((static_cast<u64>(prng) * 8) >> 32);
            preview << QString::fromStdString(Translator::getNeedle(needle));
        }
        else
        {
            u8 chatot = static_cast<u8>(((static_cast<u64>(prng) * 0x1fff) >> 32) / 82);
            preview << QString::fromStdString(Utilities5::getChatot(chatot));
        }
    }

    ui->lineEditPreview->setText(preview.join(", "));
    ui->lineEditPreview->setCursorPosition(0);
}
