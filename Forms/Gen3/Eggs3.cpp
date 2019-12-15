/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen3/Egg3.hpp>
#include <Core/Parents/FrameCompare.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen3/ProfileManager3.hpp>
#include <Models/Gen3/Egg3Model.hpp>
#include <QSettings>

namespace PokeFinderForms
{
    Eggs3::Eggs3(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Eggs3)
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
        connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Eggs3::profilesIndexChanged);

        profiles = PokeFinderCore::Profile3::loadProfileList();
        profiles.insert(profiles.begin(), PokeFinderCore::Profile3());

        ui->comboBoxProfiles->clear();

        for (const auto &profile : profiles)
        {
            ui->comboBoxProfiles->addItem(profile.getProfileName());
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
        emeraldIVs = new PokeFinderModels::Egg3Model(ui->tableViewEmeraldIVs, PokeFinderCore::Method::EBred);
        emeraldPID = new PokeFinderModels::Egg3Model(ui->tableViewEmeraldPID, PokeFinderCore::Method::EBredPID);
        rs = new PokeFinderModels::Egg3Model(ui->tableViewRS, PokeFinderCore::Method::RSBred);
        frlg = new PokeFinderModels::Egg3Model(ui->tableViewFRLG, PokeFinderCore::Method::FRLGBred);

        ui->tableViewEmeraldIVs->setModel(emeraldIVs);
        ui->tableViewEmeraldPID->setModel(emeraldPID);
        ui->tableViewRS->setModel(rs);
        ui->tableViewFRLG->setModel(frlg);

        ui->textBoxEmeraldPIDMinFrame->setValues(InputType::Frame32Bit);
        ui->textBoxEmeraldPIDMaxFrame->setValues(InputType::Frame32Bit);
        ui->textBoxMinRedraws->setValues(0, 255);
        ui->textBoxMaxRedraws->setValues(0, 255);
        ui->textBoxCalibration->setValues(0, 255);
        ui->textBoxEmeraldTID->setValues(InputType::TIDSID);
        ui->textBoxEmeraldSID->setValues(InputType::TIDSID);
        ui->textBoxEmeraldIVsMinFrame->setValues(InputType::Frame32Bit);
        ui->textBoxEmeraldIVsMaxFrame->setValues(InputType::Frame32Bit);

        ui->textBoxRSSeedHeld->setValues(InputType::Seed16Bit);
        ui->textBoxRSSeedPickup->setValues(InputType::Seed16Bit);
        ui->textBoxRSMinHeld->setValues(InputType::Frame32Bit);
        ui->textBoxRSMaxHeld->setValues(InputType::Frame32Bit);
        ui->textBoxRSMinPickup->setValues(InputType::Frame32Bit);
        ui->textBoxRSMaxPickup->setValues(InputType::Frame32Bit);
        ui->textBoxRSTID->setValues(InputType::TIDSID);
        ui->textBoxRSSID->setValues(InputType::TIDSID);

        ui->textBoxFRLGSeedHeld->setValues(InputType::Seed16Bit);
        ui->textBoxFRLGSeedPickup->setValues(InputType::Seed16Bit);
        ui->textBoxFRLGMinHeld->setValues(InputType::Frame32Bit);
        ui->textBoxFRLGMaxHeld->setValues(InputType::Frame32Bit);
        ui->textBoxFRLGMinPickup->setValues(InputType::Frame32Bit);
        ui->textBoxFRLGMaxPickup->setValues(InputType::Frame32Bit);
        ui->textBoxFRLGTID->setValues(InputType::TIDSID);
        ui->textBoxFRLGSID->setValues(InputType::TIDSID);

        ui->comboBoxEmeraldCompatibility->setItemData(0, 20);
        ui->comboBoxEmeraldCompatibility->setItemData(1, 50);
        ui->comboBoxEmeraldCompatibility->setItemData(2, 70);

        ui->comboBoxRSCompatibility->setItemData(0, 20);
        ui->comboBoxRSCompatibility->setItemData(1, 50);
        ui->comboBoxRSCompatibility->setItemData(2, 70);

        ui->comboBoxFRLGCompatibility->setItemData(0, 20);
        ui->comboBoxFRLGCompatibility->setItemData(1, 50);
        ui->comboBoxFRLGCompatibility->setItemData(2, 70);

        ui->comboBoxEmeraldGenderRatio->setItemData(0, 0);
        ui->comboBoxEmeraldGenderRatio->setItemData(1, 127);
        ui->comboBoxEmeraldGenderRatio->setItemData(2, 191);
        ui->comboBoxEmeraldGenderRatio->setItemData(3, 63);
        ui->comboBoxEmeraldGenderRatio->setItemData(4, 31);
        ui->comboBoxEmeraldGenderRatio->setItemData(5, 1);
        ui->comboBoxEmeraldGenderRatio->setItemData(6, 2);

        ui->comboBoxFRLGGenderRatio->setItemData(0, 0);
        ui->comboBoxFRLGGenderRatio->setItemData(1, 127);
        ui->comboBoxFRLGGenderRatio->setItemData(2, 191);
        ui->comboBoxFRLGGenderRatio->setItemData(3, 63);
        ui->comboBoxFRLGGenderRatio->setItemData(4, 31);
        ui->comboBoxFRLGGenderRatio->setItemData(5, 1);
        ui->comboBoxFRLGGenderRatio->setItemData(6, 2);

        ui->comboBoxRSGenderRatio->setItemData(0, 0);
        ui->comboBoxRSGenderRatio->setItemData(1, 127);
        ui->comboBoxRSGenderRatio->setItemData(2, 191);
        ui->comboBoxRSGenderRatio->setItemData(3, 63);
        ui->comboBoxRSGenderRatio->setItemData(4, 31);
        ui->comboBoxRSGenderRatio->setItemData(5, 1);
        ui->comboBoxRSGenderRatio->setItemData(6, 2);

        ui->comboBoxEmeraldNature->setup(PokeFinderCore::Translator::getNatures());
        ui->comboBoxEverstone->addItems(PokeFinderCore::Translator::getNatures());
        ui->comboBoxFRLGNature->setup(PokeFinderCore::Translator::getNatures());
        ui->comboBoxRSNature->setup(PokeFinderCore::Translator::getNatures());

        ui->comboBoxEmeraldHiddenPower->setup(PokeFinderCore::Translator::getPowers());
        ui->comboBoxFRLGHiddenPower->setup(PokeFinderCore::Translator::getPowers());
        ui->comboBoxRSHiddenPower->setup(PokeFinderCore::Translator::getPowers());

        ui->comboBoxEmeraldMethod->setItemData(0, PokeFinderCore::Method::EBred);
        ui->comboBoxEmeraldMethod->setItemData(1, PokeFinderCore::Method::EBredSplit);
        ui->comboBoxEmeraldMethod->setItemData(2, PokeFinderCore::Method::EBredAlternate);

        ui->comboBoxRSMethod->setItemData(0, PokeFinderCore::Method::RSBred);
        ui->comboBoxRSMethod->setItemData(1, PokeFinderCore::Method::RSBredSplit);
        ui->comboBoxRSMethod->setItemData(2, PokeFinderCore::Method::RSBredAlternate);

        ui->comboBoxFRLGMethod->setItemData(0, PokeFinderCore::Method::FRLGBred);
        ui->comboBoxFRLGMethod->setItemData(1, PokeFinderCore::Method::FRLGBredSplit);
        ui->comboBoxFRLGMethod->setItemData(2, PokeFinderCore::Method::FRLGBredAlternate);

        connect(ui->pushButtonEmeraldPIDGenerate, &QPushButton::clicked, this, &Eggs3::emeraldPIDGenerate);
        connect(ui->pushButtonEmeraldIVsGenerate, &QPushButton::clicked, this, &Eggs3::emeraldIVsGenerate);
        connect(ui->pushButtonRSGenerate, &QPushButton::clicked, this, &Eggs3::rsGenerate);
        connect(ui->pushButtonFRLGGenerate, &QPushButton::clicked, this, &Eggs3::frlgGenerate);
        connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs3::profileManager);
        connect(ui->eggSettingsEmerald, &EggSettings::toggleInheritance, emeraldIVs,
            &PokeFinderModels::Egg3Model::toggleInheritance);
        connect(
            ui->eggSettingsRS, &EggSettings::toggleInheritance, rs, &PokeFinderModels::Egg3Model::toggleInheritance);
        connect(ui->eggSettingsFRLG, &EggSettings::toggleInheritance, frlg,
            &PokeFinderModels::Egg3Model::toggleInheritance);

        QSettings setting;
        if (setting.contains("eggs3/geometry"))
        {
            this->restoreGeometry(setting.value("eggs3/geometry").toByteArray());
        }
    }

    void Eggs3::refreshProfiles() { emit alertProfiles(3); }

    void Eggs3::emeraldPIDGenerate()
    {
        emeraldPID->clearModel();

        u32 startingFrame = ui->textBoxEmeraldPIDMinFrame->getUInt();
        u32 maxResults = ui->textBoxEmeraldPIDMaxFrame->getUInt();
        u16 tid = ui->textBoxEmeraldTID->getUShort();
        u16 sid = ui->textBoxEmeraldSID->getUShort();
        u8 genderRatio = ui->comboBoxEmeraldGenderRatio->currentData().toUInt();

        PokeFinderCore::Egg3 generator(
            maxResults, startingFrame, tid, sid, PokeFinderCore::Method::EBredPID, genderRatio);
        generator.setMinRedraw(ui->textBoxMinRedraws->getUInt());
        generator.setMaxRedraw(ui->textBoxMaxRedraws->getUInt());
        generator.setCalibration(ui->textBoxCalibration->getUInt());
        generator.setCompatability(ui->comboBoxEmeraldCompatibility->currentData().toUInt());
        generator.setEverstone(ui->comboBoxEverstone->currentIndex() != 0);
        if (ui->comboBoxEverstone->currentIndex() != 0)
        {
            generator.setEverstoneNature(
                PokeFinderCore::Nature::getAdjustedNature(ui->comboBoxEverstone->currentIndex() - 1));
        }

        PokeFinderCore::FrameCompare compare(ui->comboBoxEmeraldGender->currentIndex(),
            ui->comboBoxEmeraldAbility->currentIndex(), ui->checkBoxEmeraldShiny->isChecked(), false, QVector<u8>(),
            QVector<u8>(), ui->comboBoxEmeraldNature->getChecked(), QVector<bool>(), QVector<bool>());

        QVector<PokeFinderCore::Frame3> frames = generator.generate(compare);
        emeraldPID->addItems(frames);
    }

    void Eggs3::emeraldIVsGenerate()
    {
        emeraldIVs->clearModel();

        u32 startingFrame = ui->textBoxEmeraldIVsMinFrame->getUInt();
        u32 maxResults = ui->textBoxEmeraldIVsMaxFrame->getUInt();
        u16 tid = ui->textBoxEmeraldTID->getUShort();
        u16 sid = ui->textBoxEmeraldSID->getUShort();
        PokeFinderCore::Method method
            = static_cast<PokeFinderCore::Method>(ui->comboBoxEmeraldMethod->currentData().toUInt());

        PokeFinderCore::Egg3 generator(maxResults, startingFrame, tid, sid, method, 0);
        generator.setParents(ui->eggSettingsEmerald->getParent1(), ui->eggSettingsEmerald->getParent2());

        PokeFinderCore::FrameCompare compare(0, 0, false, false, ui->ivFilterEmerald->getLower(),
            ui->ivFilterEmerald->getUpper(), QVector<bool>(), ui->comboBoxEmeraldHiddenPower->getChecked(),
            QVector<bool>());

        QVector<PokeFinderCore::Frame3> frames = generator.generate(compare);
        emeraldIVs->addItems(frames);
    }

    void Eggs3::rsGenerate()
    {
        rs->clearModel();

        u32 minHeld = ui->textBoxRSMinHeld->getUInt();
        u32 maxHeld = ui->textBoxRSMaxHeld->getUInt();
        u16 tid = ui->textBoxRSTID->getUShort();
        u16 sid = ui->textBoxRSSID->getUShort();
        u8 genderRatio = ui->comboBoxRSGenderRatio->currentData().toUInt();
        PokeFinderCore::Method method
            = static_cast<PokeFinderCore::Method>(ui->comboBoxRSMethod->currentData().toUInt());

        PokeFinderCore::Egg3 generator(
            maxHeld, minHeld, tid, sid, method, genderRatio, ui->textBoxRSSeedHeld->getUInt());
        generator.setPickupSeed(ui->textBoxRSSeedPickup->getUInt());
        generator.setParents(ui->eggSettingsRS->getParent1(), ui->eggSettingsRS->getParent2());

        generator.setMinPickup(ui->textBoxRSMinPickup->getUInt());
        generator.setMaxPickup(ui->textBoxRSMaxPickup->getUInt());
        generator.setCompatability(ui->comboBoxRSCompatibility->currentData().toUInt());

        PokeFinderCore::FrameCompare compare(ui->comboBoxRSGender->currentIndex(),
            ui->comboBoxRSAbility->currentIndex(), ui->checkBoxRSShiny->isChecked(), false, ui->ivFilterRS->getLower(),
            ui->ivFilterRS->getUpper(), ui->comboBoxRSNature->getChecked(), ui->comboBoxRSHiddenPower->getChecked(),
            QVector<bool>());

        QVector<PokeFinderCore::Frame3> frames = generator.generate(compare);
        rs->addItems(frames);
    }

    void Eggs3::frlgGenerate()
    {
        frlg->clearModel();

        u32 minHeld = ui->textBoxFRLGMinHeld->getUInt();
        u32 maxHeld = ui->textBoxFRLGMaxHeld->getUInt();
        u16 tid = ui->textBoxFRLGTID->getUShort();
        u16 sid = ui->textBoxFRLGSID->getUShort();
        u8 genderRatio = ui->comboBoxFRLGGenderRatio->currentData().toUInt();
        PokeFinderCore::Method method
            = static_cast<PokeFinderCore::Method>(ui->comboBoxFRLGMethod->currentData().toUInt());

        PokeFinderCore::Egg3 generator(
            maxHeld, minHeld, tid, sid, method, genderRatio, ui->textBoxFRLGSeedHeld->getUInt());
        generator.setPickupSeed(ui->textBoxFRLGSeedPickup->getUInt());
        generator.setParents(ui->eggSettingsFRLG->getParent1(), ui->eggSettingsFRLG->getParent2());

        generator.setMinPickup(ui->textBoxFRLGMinPickup->getUInt());
        generator.setMaxPickup(ui->textBoxFRLGMaxPickup->getUInt());
        generator.setCompatability(ui->comboBoxFRLGCompatibility->currentData().toUInt());

        PokeFinderCore::FrameCompare compare(ui->comboBoxFRLGGender->currentIndex(),
            ui->comboBoxFRLGAbility->currentIndex(), ui->checkBoxFRLGShiny->isChecked(), false,
            ui->ivFilterFRLG->getLower(), ui->ivFilterFRLG->getUpper(), ui->comboBoxFRLGNature->getChecked(),
            ui->comboBoxFRLGHiddenPower->getChecked(), QVector<bool>());

        QVector<PokeFinderCore::Frame3> frames = generator.generate(compare);
        frlg->addItems(frames);
    }

    void Eggs3::profilesIndexChanged(int index)
    {
        if (index < 0)
        {
            return;
        }

        auto profile = profiles.at(index);
        QString tid = QString::number(profile.getTID());
        QString sid = QString::number(profile.getSID());

        ui->textBoxEmeraldTID->setText(tid);
        ui->textBoxEmeraldSID->setText(sid);
        ui->textBoxRSTID->setText(tid);
        ui->textBoxRSSID->setText(sid);
        ui->textBoxFRLGTID->setText(tid);
        ui->textBoxFRLGSID->setText(sid);
        ui->labelProfileTIDValue->setText(tid);
        ui->labelProfileSIDValue->setText(sid);
        ui->labelProfileGameValue->setText(profile.getVersionString());
    }

    void Eggs3::profileManager()
    {
        auto *manager = new ProfileManager3();
        connect(manager, &ProfileManager3::updateProfiles, this, &Eggs3::refreshProfiles);
        manager->show();
    }
}
