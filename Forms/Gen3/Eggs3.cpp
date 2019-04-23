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

#include <QSettings>
#include "Eggs3.hpp"
#include "ui_Eggs3.h"
#include <Core/Gen3/Egg3.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Power.hpp>
#include <Forms/Gen3/ProfileManager3.hpp>

Eggs3::Eggs3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Eggs3)
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
    setting.setValue("size", this->size());
    setting.endGroup();

    delete ui;
}

void Eggs3::updateProfiles()
{
    profiles = Profile3::loadProfileList();
    profiles.insert(profiles.begin(), Profile3());

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
    emeraldIVs = new Egg3Model(ui->tableViewEmeraldIVs, Method::EBred);
    emeraldPID = new Egg3Model(ui->tableViewEmeraldPID, Method::EBredPID);
    rs = new Egg3Model(ui->tableViewRS, Method::RSBred);
    frlg = new Egg3Model(ui->tableViewFRLG, Method::FRLGBred);

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

    ui->comboBoxEmeraldNature->setup(Nature::getNatures());
    ui->comboBoxEverstone->addItems(Nature::getNatures());
    ui->comboBoxFRLGNature->setup(Nature::getNatures());
    ui->comboBoxRSNature->setup(Nature::getNatures());

    ui->comboBoxEmeraldHiddenPower->setup(Power::getPowers());
    ui->comboBoxFRLGHiddenPower->setup(Power::getPowers());
    ui->comboBoxRSHiddenPower->setup(Power::getPowers());

    ui->comboBoxEmeraldMethod->setItemData(0, Method::EBred);
    ui->comboBoxEmeraldMethod->setItemData(1, Method::EBredSplit);
    ui->comboBoxEmeraldMethod->setItemData(2, Method::EBredAlternate);

    ui->comboBoxRSMethod->setItemData(0, Method::RSBred);
    ui->comboBoxRSMethod->setItemData(1, Method::RSBredSplit);
    ui->comboBoxRSMethod->setItemData(2, Method::RSBredAlternate);

    ui->comboBoxFRLGMethod->setItemData(0, Method::FRLGBred);
    ui->comboBoxFRLGMethod->setItemData(1, Method::FRLGBredSplit);
    ui->comboBoxFRLGMethod->setItemData(2, Method::FRLGBredAlternate);

    connect(ui->eggSettingsEmerald, &EggSettings::toggleInheritance, emeraldIVs, &Egg3Model::toggleInheritance);
    connect(ui->eggSettingsRS, &EggSettings::toggleInheritance, rs, &Egg3Model::toggleInheritance);
    connect(ui->eggSettingsFRLG, &EggSettings::toggleInheritance, frlg, &Egg3Model::toggleInheritance);

    QSettings setting;
    if (setting.contains("eggs3/size")) this->resize(setting.value("eggs3/size").toSize());
}

void Eggs3::refreshProfiles()
{
    emit alertProfiles(3);
}

void Eggs3::on_pushButtonEmeraldPIDGenerate_clicked()
{
    emeraldPID->clearModel();

    u32 startingFrame = ui->textBoxEmeraldPIDMinFrame->getUInt();
    u32 maxResults = ui->textBoxEmeraldPIDMaxFrame->getUInt();
    u16 tid = ui->textBoxEmeraldTID->getUShort();
    u16 sid = ui->textBoxEmeraldSID->getUShort();
    u8 genderRatio = ui->comboBoxEmeraldGenderRatio->currentData().toUInt();

    Egg3 generator(maxResults, startingFrame, tid, sid, Method::EBredPID, genderRatio);
    generator.setMinRedraw(ui->textBoxMinRedraws->getUInt());
    generator.setMaxRedraw(ui->textBoxMaxRedraws->getUInt());
    generator.setCalibration(ui->textBoxCalibration->getUInt());
    generator.setCompatability(ui->comboBoxEmeraldCompatibility->currentData().toUInt());
    generator.setEverstone(ui->comboBoxEverstone->currentIndex() != 0);
    if (ui->comboBoxEverstone->currentIndex() != 0)
    {
        generator.setEverstoneNature(Nature::getAdjustedNature(ui->comboBoxEverstone->currentIndex() - 1));
    }

    FrameCompare compare(ui->comboBoxEmeraldGender->currentIndex(), ui->comboBoxEmeraldAbility->currentIndex(),
                         ui->comboBoxEmeraldNature->getChecked(), ui->checkBoxEmeraldShiny->isChecked());

    QVector<Frame3> frames = generator.generate(compare);
    emeraldPID->addItems(frames);
}

void Eggs3::on_pushButtonEmeraldIVsGenerate_clicked()
{
    emeraldIVs->clearModel();

    u32 startingFrame = ui->textBoxEmeraldIVsMinFrame->getUInt();
    u32 maxResults = ui->textBoxEmeraldIVsMaxFrame->getUInt();
    u16 tid = ui->textBoxEmeraldTID->getUShort();
    u16 sid = ui->textBoxEmeraldSID->getUShort();
    Method method = static_cast<Method>(ui->comboBoxEmeraldMethod->currentData().toUInt());

    Egg3 generator = Egg3(maxResults, startingFrame, tid, sid, method, 0);
    generator.setParents(ui->eggSettingsEmerald->getParent1(), ui->eggSettingsEmerald->getParent2());

    FrameCompare compare = FrameCompare(ui->ivFilterEmerald->getLower(), ui->ivFilterEmerald->getUpper(), ui->comboBoxEmeraldHiddenPower->getChecked());

    QVector<Frame3> frames = generator.generate(compare);
    emeraldIVs->addItems(frames);
}

void Eggs3::on_pushButtonRSGenerate_clicked()
{
    rs->clearModel();

    u32 minHeld = ui->textBoxRSMinHeld->getUInt();
    u32 maxHeld = ui->textBoxRSMaxHeld->getUInt();
    u16 tid = ui->textBoxRSTID->getUShort();
    u16 sid = ui->textBoxRSSID->getUShort();
    u8 genderRatio = ui->comboBoxRSGenderRatio->currentData().toUInt();
    Method method = static_cast<Method>(ui->comboBoxRSMethod->currentData().toUInt());

    Egg3 generator(maxHeld, minHeld, tid, sid, method, genderRatio, ui->textBoxRSSeedHeld->getUInt());
    generator.setPickupSeed(ui->textBoxRSSeedPickup->getUInt());
    generator.setParents(ui->eggSettingsRS->getParent1(), ui->eggSettingsRS->getParent2());

    generator.setMinPickup(ui->textBoxRSMinPickup->getUInt());
    generator.setMaxPickup(ui->textBoxRSMaxPickup->getUInt());
    generator.setCompatability(ui->comboBoxRSCompatibility->currentData().toUInt());

    FrameCompare compare(ui->ivFilterRS->getLower(), ui->ivFilterRS->getUpper(), ui->comboBoxRSGender->currentIndex(),
                         ui->comboBoxRSAbility->currentIndex(), ui->comboBoxRSNature->getChecked(),
                         ui->comboBoxRSHiddenPower->getChecked(), ui->checkBoxRSShiny->isChecked(), false);

    QVector<Frame3> frames = generator.generate(compare);
    rs->addItems(frames);
}

void Eggs3::on_pushButtonFRLGGenerate_clicked()
{
    frlg->clearModel();

    u32 minHeld = ui->textBoxFRLGMinHeld->getUInt();
    u32 maxHeld = ui->textBoxFRLGMaxHeld->getUInt();
    u16 tid = ui->textBoxFRLGTID->getUShort();
    u16 sid = ui->textBoxFRLGSID->getUShort();
    u8 genderRatio = ui->comboBoxFRLGGenderRatio->currentData().toUInt();
    Method method = static_cast<Method>(ui->comboBoxFRLGMethod->currentData().toUInt());

    Egg3 generator(maxHeld, minHeld, tid, sid, method, genderRatio, ui->textBoxFRLGSeedHeld->getUInt());
    generator.setPickupSeed(ui->textBoxFRLGSeedPickup->getUInt());
    generator.setParents(ui->eggSettingsFRLG->getParent1(), ui->eggSettingsFRLG->getParent2());

    generator.setMinPickup(ui->textBoxFRLGMinPickup->getUInt());
    generator.setMaxPickup(ui->textBoxFRLGMaxPickup->getUInt());
    generator.setCompatability(ui->comboBoxFRLGCompatibility->currentData().toUInt());

    FrameCompare compare(ui->ivFilterFRLG->getLower(), ui->ivFilterFRLG->getUpper(), ui->comboBoxFRLGGender->currentIndex(),
                         ui->comboBoxFRLGAbility->currentIndex(), ui->comboBoxFRLGNature->getChecked(),
                         ui->comboBoxFRLGHiddenPower->getChecked(), ui->checkBoxFRLGShiny->isChecked(), false);

    QVector<Frame3> frames = generator.generate(compare);
    frlg->addItems(frames);
}

void Eggs3::on_comboBoxProfiles_currentIndexChanged(int index)
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

void Eggs3::on_pushButtonEmeraldAnyAbility_clicked()
{
    ui->comboBoxEmeraldAbility->setCurrentIndex(0);
}

void Eggs3::on_pushButtonRSAnyAbility_clicked()
{
    ui->comboBoxRSAbility->setCurrentIndex(0);
}

void Eggs3::on_pushButtonFRLGAnyAbility_clicked()
{
    ui->comboBoxFRLGAbility->setCurrentIndex(0);
}

void Eggs3::on_pushButtonProfileManager_clicked()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, &Eggs3::refreshProfiles);
    manager->show();
}
