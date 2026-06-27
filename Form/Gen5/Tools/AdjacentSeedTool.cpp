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

#include "AdjacentSeedTool.hpp"
#include "ui_AdjacentSeedTool.h"
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Form/Util/IVCalculator.hpp>
#include <Model/Gen5/AdjacentSeedModel5.hpp>
#include <QAbstractItemView>
#include <QDate>
#include <QTime>
#include <QEvent>
#include <QItemSelectionModel>
#include <QLineEdit>
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

static const std::array<Buttons, 12> keypressButtons = { Buttons::Start, Buttons::Select, Buttons::A,    Buttons::B,
                                                         Buttons::Right, Buttons::Left,   Buttons::Up,   Buttons::Down,
                                                         Buttons::R,     Buttons::L,      Buttons::X,    Buttons::Y };

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

AdjacentSeedTool::AdjacentSeedTool(QWidget *parent) : QWidget(parent), ui(new Ui::AdjacentSeedTool)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new AdjacentSeedModel5(ui->tableView);
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

    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setDisplayFormat("HH:mm:ss");
    ui->timeEdit->setTime(QTime::currentTime());
    ui->textBoxMinIVAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxMaxIVAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxMinIVAdvance->setText("0");
    ui->textBoxMaxIVAdvance->setText("0");
    ui->gridLayoutSettings->setHorizontalSpacing(8);
    ui->gridLayoutSettings->setColumnStretch(0, 0);
    ui->gridLayoutSettings->setColumnStretch(1, 0);
    ui->gridLayoutSettings->setColumnStretch(2, 0);
    ui->gridLayoutSettings->setColumnStretch(3, 0);
    ui->gridLayoutSettings->setColumnStretch(4, 0);
    ui->gridLayoutSettings->setColumnStretch(5, 1);
    ui->gridLayoutSettings->setColumnStretch(6, 0);
    ui->gridLayoutSettings->setColumnStretch(7, 0);
    ui->gridLayoutSettings->setColumnStretch(8, 1);
    ui->dateEdit->setMaximumWidth(115);
    ui->timeEdit->setMaximumWidth(90);
    ui->comboBoxKeypresses->setMinimumWidth(300);
    ui->comboBoxKeypresses->setMaximumWidth(999);
    ui->pushButtonGenerate->setMinimumWidth(120);
    ui->pushButtonGenerate->setMaximumWidth(999);
    ui->spinBoxSeconds->setMaximumWidth(48);
    ui->comboBoxMethod->setMinimumWidth(180);
    ui->comboBoxMethod->setMaximumWidth(999);
    ui->textBoxMinIVAdvance->setMaximumWidth(65);
    ui->textBoxMaxIVAdvance->setMaximumWidth(65);
    ui->pushButtonIVCalculator->setMinimumWidth(120);
    ui->pushButtonIVCalculator->setMaximumWidth(999);
    ui->labelDate->setMaximumWidth(70);
    ui->labelTime->setMaximumWidth(70);
    ui->labelSeconds->setMaximumWidth(100);
    ui->labelKeypresses->setMaximumWidth(100);
    ui->labelMethod->setMaximumWidth(100);
    ui->labelMinIVAdvance->setMaximumWidth(150);
    ui->labelMaxIVAdvance->setMaximumWidth(150);
    ui->comboBoxPreviewMode->setMaximumWidth(140);

    ui->comboBoxMethod->addItem(tr("Wild / Static / Grotto"), static_cast<int>(AdjacentSeedMethod::Standard));
    ui->comboBoxMethod->addItem(tr("Roamer"), static_cast<int>(AdjacentSeedMethod::Roamer));
    ui->comboBoxMethod->setCurrentIndex(0);
    ui->comboBoxPreviewMode->addItem(tr("Chatot Pitches"), chatotPreview);
    ui->comboBoxPreviewMode->addItem(tr("Save Needles"), needlePreview);

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &AdjacentSeedTool::profileIndexChanged);
    connect(ui->comboBoxKeypresses->view(), &QAbstractItemView::pressed, this, &AdjacentSeedTool::keypressIndexPressed);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &AdjacentSeedTool::profileManager);
    connect(ui->pushButtonIVCalculator, &QPushButton::clicked, this, &AdjacentSeedTool::openIVCalculator);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &AdjacentSeedTool::generate);
    connect(ui->comboBoxPreviewMode, &QComboBox::currentIndexChanged, this, &AdjacentSeedTool::updatePreview);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            [=](const QModelIndex &, const QModelIndex &) { updatePreview(); });

    updateProfiles();
}

AdjacentSeedTool::AdjacentSeedTool(const DateTime &dateTime, Buttons buttons, AdjacentSeedMethod method, QWidget *parent) :
    AdjacentSeedTool(parent)
{
    setInitialSearch(dateTime, buttons);
    setMethod(method);
}

AdjacentSeedTool::AdjacentSeedTool(const DateTime &dateTime, Buttons buttons, const Profile5 &profile, AdjacentSeedMethod method,
                                   QWidget *parent) :
    AdjacentSeedTool(parent)
{
    setProfile(profile);
    setInitialSearch(dateTime, buttons);
    setMethod(method);
}

AdjacentSeedTool::~AdjacentSeedTool()
{
    QSettings setting;
    setting.beginGroup("adjacentseedtool");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("seconds", ui->spinBoxSeconds->value());
    setting.setValue("minIVAdvance", ui->textBoxMinIVAdvance->text());
    setting.setValue("maxIVAdvance", ui->textBoxMaxIVAdvance->text());
    setting.endGroup();

    delete ui;
}

bool AdjacentSeedTool::hasProfiles() const
{
    return !profiles.empty();
}

bool AdjacentSeedTool::eventFilter(QObject *object, QEvent *event)
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

void AdjacentSeedTool::setInitialSearch(const DateTime &dateTime, Buttons buttons)
{
    Date date = dateTime.getDate();
    Time time = dateTime.getTime();
    ui->dateEdit->setDate(QDate(date.year(), date.month(), date.day()));
    ui->timeEdit->setTime(QTime(time.hour(), time.minute(), time.second()));
    setSelectedButtons(buttons);
}

void AdjacentSeedTool::setMethod(AdjacentSeedMethod method)
{
    int index = ui->comboBoxMethod->findData(static_cast<int>(method));
    if (index != -1)
    {
        ui->comboBoxMethod->setCurrentIndex(index);
    }
}

void AdjacentSeedTool::setProfile(const Profile5 &profile)
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

Buttons AdjacentSeedTool::getSelectedButtons() const
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

void AdjacentSeedTool::setSelectedButtons(Buttons buttons)
{
    currentButtons = buttons;
    for (int i = 0; i < keypressModel->rowCount(); i++)
    {
        auto *item = keypressModel->item(i);
        item->setCheckState((buttons & static_cast<Buttons>(item->data().toUInt())) != Buttons::None ? Qt::Checked : Qt::Unchecked);
    }
    updateKeypressText();
}

void AdjacentSeedTool::updateProfiles()
{
    currentButtons = getSelectedButtons();
    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }
    ui->pushButtonGenerate->setEnabled(!profiles.empty());

    QSettings setting;
    int profileIndex = setting.value("adjacentseedtool/profile", 0).toInt();
    if (profileIndex < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(profileIndex);
    }

    ui->comboBoxMethod->setCurrentIndex(0);
    ui->spinBoxSeconds->setValue(setting.value("adjacentseedtool/seconds", 1).toInt());
    ui->textBoxMinIVAdvance->setText(setting.value("adjacentseedtool/minIVAdvance", "0").toString());
    ui->textBoxMaxIVAdvance->setText(setting.value("adjacentseedtool/maxIVAdvance", "0").toString());
}

void AdjacentSeedTool::generate()
{
    if (!currentProfile)
    {
        return;
    }

    u32 initialIVAdvance = ui->textBoxMinIVAdvance->getUInt();
    u32 maxIVAdvance = initialIVAdvance + ui->textBoxMaxIVAdvance->getUInt();

    QTime time = ui->timeEdit->time();
    DateTime dateTime(ui->dateEdit->getDate(), Time(time.hour(), time.minute(), time.second()));

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

void AdjacentSeedTool::keypressIndexPressed(const QModelIndex &index)
{
    auto *item = keypressModel->item(index.row());
    if (!item)
    {
        return;
    }

    item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    currentButtons = getSelectedButtons();
    QTimer::singleShot(0, this, &AdjacentSeedTool::updateKeypressText);
}

void AdjacentSeedTool::openIVCalculator()
{
    auto *calculator = new IVCalculator();
    calculator->show();
}

void AdjacentSeedTool::profileIndexChanged(int index)
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

void AdjacentSeedTool::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) {
        emit profilesModified(num);
        updateProfiles();
    });
    manager->show();
}

void AdjacentSeedTool::updateKeypressText()
{
    Buttons buttons = getSelectedButtons();
    ui->comboBoxKeypresses->lineEdit()->setText(buttons == Buttons::None ? tr("None") : QString::fromStdString(Translator::getKeypresses(buttons)));
}

void AdjacentSeedTool::updatePreview()
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
            preview << QString::fromStdString(Translator::getSaveNeedle(needle));
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
