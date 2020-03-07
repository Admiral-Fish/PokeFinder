/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Generators/EggGenerator3.hpp>
#include <Core/Gen3/ProfileLoader3.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen3/Profile/ProfileManager3.hpp>
#include <Models/Gen3/EggModel3.hpp>
#include <QSettings>

Eggs3::Eggs3(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();
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
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Eggs3::profilesIndexChanged);

    profiles = ProfileLoader3::getProfiles();
    profiles.insert(profiles.begin(), Profile3());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getName());
    }

    QSettings setting;
    int val = setting.value("eggs3/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Eggs3::setupModels()
{
    emerald = new EggModel3(ui->tableViewEmerald, Method::EBred);
    rs = new EggModel3(ui->tableViewRS, Method::RSBred);
    frlg = new EggModel3(ui->tableViewFRLG, Method::FRLGBred);

    ui->tableViewEmerald->setModel(emerald);
    ui->tableViewRS->setModel(rs);
    ui->tableViewFRLG->setModel(frlg);

    ui->textBoxEmeraldInitialFrame->setValues(InputType::Frame32Bit);
    ui->textBoxEmeraldMaxResults->setValues(InputType::Frame32Bit);
    ui->textBoxMinRedraws->setValues(0, 255, 3, 10);
    ui->textBoxMaxRedraws->setValues(0, 255, 3, 10);
    ui->textBoxCalibration->setValues(0, 255, 3, 10);

    ui->textBoxRSSeedHeld->setValues(InputType::Seed16Bit);
    ui->textBoxRSSeedPickup->setValues(InputType::Seed16Bit);
    ui->textBoxRSInitialFrameHeld->setValues(InputType::Frame32Bit);
    ui->textBoxRSMaxResultsHeld->setValues(InputType::Frame32Bit);
    ui->textBoxRSInitialFramePickup->setValues(InputType::Frame32Bit);
    ui->textBoxRSMaxResultsPickup->setValues(InputType::Frame32Bit);

    ui->textBoxFRLGSeedHeld->setValues(InputType::Seed16Bit);
    ui->textBoxFRLGSeedPickup->setValues(InputType::Seed16Bit);
    ui->textBoxFRLGInitialFrameHeld->setValues(InputType::Frame32Bit);
    ui->textBoxFRLGMaxResultsHeld->setValues(InputType::Frame32Bit);
    ui->textBoxFRLGInitialFramePickup->setValues(InputType::Frame32Bit);
    ui->textBoxFRLGMaxResultsPickup->setValues(InputType::Frame32Bit);

    ui->comboBoxEmeraldCompatibility->setup({ 20, 50, 70 });
    ui->comboBoxRSCompatibility->setup({ 20, 50, 70 });
    ui->comboBoxFRLGCompatibility->setup({ 20, 50, 70 });

    ui->comboBoxEverstone->addItems(Translator::getNatures());

    ui->comboBoxEmeraldMethod->setup({ Method::EBredPID, Method::EBred, Method::EBredSplit, Method::EBredAlternate });
    ui->comboBoxRSMethod->setup({ Method::RSBred, Method::RSBredSplit, Method::RSBredAlternate });
    ui->comboBoxFRLGMethod->setup({ Method::FRLGBred, Method::FRLGBredSplit, Method::FRLGBredAlternate });

    ui->filterEmerald->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);
    ui->filterRS->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);
    ui->filterFRLG->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    connect(ui->pushButtonEmeraldGenerate, &QPushButton::clicked, this, &Eggs3::emeraldGenerate);
    connect(ui->pushButtonRSGenerate, &QPushButton::clicked, this, &Eggs3::rsGenerate);
    connect(ui->pushButtonFRLGGenerate, &QPushButton::clicked, this, &Eggs3::frlgGenerate);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs3::profileManager);
    connect(ui->eggSettingsEmerald, &EggSettings::toggleInheritance, emerald, &EggModel3::toggleInheritance);
    connect(ui->eggSettingsRS, &EggSettings::toggleInheritance, rs, &EggModel3::toggleInheritance);
    connect(ui->eggSettingsFRLG, &EggSettings::toggleInheritance, frlg, &EggModel3::toggleInheritance);

    QSettings setting;
    if (setting.contains("eggs3/geometry"))
    {
        this->restoreGeometry(setting.value("eggs3/geometry").toByteArray());
    }
}

void Eggs3::emeraldGenerate()
{
    auto method = static_cast<Method>(ui->comboBoxEmeraldMethod->getCurrentInt());
    emerald->clearModel();
    emerald->setMethod(method);

    u32 initialFrame = ui->textBoxEmeraldInitialFrame->getUInt();
    u32 maxResults = ui->textBoxEmeraldMaxResults->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterEmerald->getGenderRatio();

    FrameFilter filter(ui->filterEmerald->getGender(), ui->filterEmerald->getAbility(), ui->filterEmerald->getShiny(), false,
                       ui->filterEmerald->getMinIVs(), ui->filterEmerald->getMaxIVs(), ui->filterEmerald->getNatures(),
                       ui->filterEmerald->getHiddenPowers(), {});

    EggGenerator3 generator(initialFrame, maxResults, tid, sid, genderRatio, method, filter);
    generator.setMinRedraw(ui->textBoxMinRedraws->getUChar());
    generator.setMaxRedraw(ui->textBoxMaxRedraws->getUChar());
    generator.setCalibration(ui->textBoxCalibration->getUChar());
    generator.setCompatability(ui->comboBoxEmeraldCompatibility->getCurrentByte());
    generator.setEverstone(ui->comboBoxEverstone->currentIndex() != 0);
    if (ui->comboBoxEverstone->currentIndex() != 0)
    {
        generator.setEverstoneNature(static_cast<u8>(ui->comboBoxEverstone->currentIndex() - 1));
    }
    if (method != Method::EBredPID)
    {
        generator.setParents(ui->eggSettingsEmerald->getParent1(), ui->eggSettingsEmerald->getParent2());
    }

    auto frames = generator.generate();
    emerald->addItems(frames);
}

void Eggs3::rsGenerate()
{
    rs->clearModel();

    u32 initialFrameHeld = ui->textBoxRSInitialFrameHeld->getUInt();
    u32 maxResultsHeld = ui->textBoxRSMaxResultsHeld->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterRS->getGenderRatio();
    auto method = static_cast<Method>(ui->comboBoxRSMethod->getCurrentInt());
    FrameFilter filter(ui->filterRS->getGender(), ui->filterRS->getAbility(), ui->filterRS->getShiny(), false, ui->filterRS->getMinIVs(),
                       ui->filterRS->getMaxIVs(), ui->filterRS->getNatures(), ui->filterRS->getHiddenPowers(), {});

    EggGenerator3 generator(initialFrameHeld, maxResultsHeld, tid, sid, genderRatio, method, filter);
    generator.setParents(ui->eggSettingsRS->getParent1(), ui->eggSettingsRS->getParent2());
    generator.setCompatability(static_cast<u8>(ui->comboBoxRSCompatibility->currentData().toUInt()));
    generator.setInitialFramePickup(ui->textBoxRSInitialFramePickup->getUInt());
    generator.setMaxResultsPickup(ui->textBoxRSMaxResultsPickup->getUInt());

    auto frames = generator.generate(ui->textBoxRSSeedHeld->getUInt(), ui->textBoxRSSeedPickup->getUInt());
    rs->addItems(frames);
}

void Eggs3::frlgGenerate()
{
    frlg->clearModel();

    u32 initialFrameHeld = ui->textBoxFRLGInitialFrameHeld->getUInt();
    u32 maxResultsHeld = ui->textBoxFRLGMaxResultsHeld->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterFRLG->getGenderRatio();
    auto method = static_cast<Method>(ui->comboBoxFRLGMethod->currentData().toUInt());

    FrameFilter filter(ui->filterFRLG->getGender(), ui->filterFRLG->getAbility(), ui->filterFRLG->getShiny(), false,
                       ui->filterFRLG->getMinIVs(), ui->filterFRLG->getMaxIVs(), ui->filterFRLG->getNatures(),
                       ui->filterFRLG->getHiddenPowers(), {});

    EggGenerator3 generator(initialFrameHeld, maxResultsHeld, tid, sid, genderRatio, method, filter);
    generator.setParents(ui->eggSettingsFRLG->getParent1(), ui->eggSettingsFRLG->getParent2());
    generator.setCompatability(static_cast<u8>(ui->comboBoxFRLGCompatibility->currentData().toUInt()));
    generator.setInitialFramePickup(ui->textBoxFRLGInitialFramePickup->getUInt());
    generator.setMaxResultsPickup(ui->textBoxFRLGMaxResultsPickup->getUInt());

    auto frames = generator.generate(ui->textBoxFRLGSeedHeld->getUInt(), ui->textBoxFRLGSeedPickup->getUInt());
    frlg->addItems(frames);
}

void Eggs3::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles.at(index);

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(currentProfile.getVersionString());
    }
}

void Eggs3::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}
