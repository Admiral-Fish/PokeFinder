/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Eggs3.hpp"
#include "ui_Eggs3.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Generators/EggGenerator3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen3/Profile/ProfileManager3.hpp>
#include <Model/Gen3/EggModel3.hpp>
#include <QMessageBox>
#include <QSettings>

Eggs3::Eggs3(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    emerald = new EggModel3(ui->tableViewEmerald, true);
    ui->tableViewEmerald->setModel(emerald);

    rsfrlg = new EggModel3(ui->tableViewRSFRLG, false);
    ui->tableViewRSFRLG->setModel(rsfrlg);

    ui->textBoxEmeraldInitialAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxEmeraldMaxAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxEmeraldInitialAdvancesPickup->setValues(InputType::Advance32Bit);
    ui->textBoxEmeraldMaxAdvancesPickup->setValues(InputType::Advance32Bit);
    ui->textBoxEmeraldDelayHeld->setValues(InputType::Advance32Bit);
    ui->textBoxEmeraldDelayPickup->setValues(InputType::Advance32Bit);
    ui->textBoxEmeraldMinRedraws->setValues(0, 255, 3, 10);
    ui->textBoxEmeraldMaxRedraws->setValues(0, 255, 3, 10);
    ui->textBoxEmeraldCalibration->setValues(0, 255, 3, 10);
    ui->comboBoxEmeraldCompatibility->setup({ 20, 50, 70 });

    ui->textBoxRSFRLGSeedHeld->setValues(InputType::Seed16Bit);
    ui->textBoxRSFRLGSeedPickup->setValues(InputType::Seed16Bit);
    ui->textBoxRSFRLGInitialAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxRSFRLGMaxAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxRSFRLGInitialAdvancesPickup->setValues(InputType::Advance32Bit);
    ui->textBoxRSFRLGMaxAdvancesPickup->setValues(InputType::Advance32Bit);
    ui->textBoxRSFRLGDelayHeld->setValues(InputType::Advance32Bit);
    ui->textBoxRSFRLGDelayPickup->setValues(InputType::Advance32Bit);
    ui->comboBoxRSFRLGCompatibility->setup({ 20, 50, 70 });

    ui->comboBoxEmeraldMethod->setup({ toInt(Method::EBred), toInt(Method::EBredSplit), toInt(Method::EBredAlternate) });
    ui->comboBoxRSFRLGMethod->setup(
        { toInt(Method::RSFRLGBred), toInt(Method::RSFRLGBredSplit), toInt(Method::RSFRLGBredAlternate), toInt(Method::RSFRLGBredMixed) });

    ui->filterEmerald->disableControls(Controls::EncounterSlots | Controls::DisableFilter);
    ui->filterRSFRLG->disableControls(Controls::EncounterSlots | Controls::DisableFilter);

    ui->eggSettingsEmerald->setup(Game::Emerald);
    ui->eggSettingsRSFRLG->setup(Game::RS | Game::FRLG);

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Eggs3::profileIndexChanged);
    connect(ui->pushButtonEmeraldGenerate, &QPushButton::clicked, this, &Eggs3::emeraldGenerate);
    connect(ui->pushButtonRSFRLGGenerate, &QPushButton::clicked, this, &Eggs3::rsfrlgGenerate);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs3::profileManager);
    connect(ui->eggSettingsEmerald, &EggSettings::showInheritanceChanged, emerald, &EggModel3::setShowInheritance);
    connect(ui->eggSettingsRSFRLG, &EggSettings::showInheritanceChanged, rsfrlg, &EggModel3::setShowInheritance);
    connect(ui->filterEmerald, &Filter::showStatsChanged, emerald, &EggModel3::setShowStats);
    connect(ui->filterRSFRLG, &Filter::showStatsChanged, rsfrlg, &EggModel3::setShowStats);

    updateProfiles();

    QSettings setting;
    if (setting.contains("eggs3/geometry"))
    {
        this->restoreGeometry(setting.value("eggs3/geometry").toByteArray());
    }
}

Eggs3::~Eggs3()
{
    QSettings setting;
    setting.beginGroup("eggs3");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Eggs3::updateProfiles()
{
    profiles = ProfileLoader3::getProfiles();
    profiles.insert(profiles.begin(), Profile3("None", Game::Emerald, 12345, 54321, false));

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("eggs3/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Eggs3::emeraldGenerate()
{
    if (!ui->eggSettingsEmerald->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }

    emerald->clearModel();

    u32 initialAdvancesHeld = ui->textBoxEmeraldInitialAdvancesHeld->getUInt();
    u32 maxAdvancesHeld = ui->textBoxEmeraldMaxAdvancesHeld->getUInt();
    u32 delayHeld = ui->textBoxEmeraldDelayHeld->getUInt();
    u32 initialAdvancesPickup = ui->textBoxEmeraldInitialAdvancesPickup->getUInt();
    u32 maxAdvancesPickup = ui->textBoxEmeraldMaxAdvancesPickup->getUInt();
    u32 delayPickup = ui->textBoxEmeraldDelayPickup->getUInt();
    u8 calibration = ui->textBoxEmeraldCalibration->getUChar();
    u8 minRedraw = ui->textBoxEmeraldMinRedraws->getUChar();
    u8 maxRedraw = ui->textBoxEmeraldMaxRedraws->getUChar();
    u8 compatability = ui->comboBoxEmeraldCompatibility->getCurrentUChar();
    auto method = ui->comboBoxEmeraldMethod->getEnum<Method>();

    StateFilter filter = ui->filterEmerald->getFilter<StateFilter>();
    EggGenerator3 generator(initialAdvancesHeld, maxAdvancesHeld, delayHeld, initialAdvancesPickup, maxAdvancesPickup, delayPickup,
                            calibration, minRedraw, maxRedraw, method, compatability, ui->eggSettingsEmerald->getDaycare(),
                            *currentProfile, filter);

    auto states = generator.generate();
    emerald->addItems(states);
}

void Eggs3::rsfrlgGenerate()
{
    if (!ui->eggSettingsRSFRLG->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }

    rsfrlg->clearModel();

    u32 initialAdvancesHeld = ui->textBoxRSFRLGInitialAdvancesHeld->getUInt();
    u32 maxAdvancesHeld = ui->textBoxRSFRLGMaxAdvancesHeld->getUInt();
    u32 delayHeld = ui->textBoxRSFRLGDelayHeld->getUInt();
    u32 initialAdvancesPickup = ui->textBoxRSFRLGInitialAdvancesPickup->getUInt();
    u32 maxAdvancesPickup = ui->textBoxRSFRLGMaxAdvancesPickup->getUInt();
    u32 delayPickup = ui->textBoxRSFRLGDelayPickup->getUInt();
    u8 compatability = ui->comboBoxRSFRLGCompatibility->getCurrentUChar();
    auto method = ui->comboBoxRSFRLGMethod->getEnum<Method>();

    StateFilter filter = ui->filterRSFRLG->getFilter<StateFilter>();
    EggGenerator3 generator(initialAdvancesHeld, maxAdvancesHeld, delayHeld, initialAdvancesPickup, maxAdvancesPickup, delayPickup, 0, 0, 0,
                            method, compatability, ui->eggSettingsRSFRLG->getDaycare(), *currentProfile, filter);

    auto states = generator.generate(ui->textBoxRSFRLGSeedHeld->getUInt(), ui->textBoxRSFRLGSeedPickup->getUInt());
    rsfrlg->addItems(states);
}

void Eggs3::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        if (currentProfile->getDeadBattery())
        {
            ui->textBoxRSFRLGSeedHeld->setText("5a0");
            ui->textBoxRSFRLGSeedPickup->setText("5a0");
        }

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));
    }
}

void Eggs3::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
