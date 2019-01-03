/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

Eggs3::Eggs3(QWidget *parent) :
    QMainWindow(parent),
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
    setting.setValue("egg3Profile", ui->comboBoxProfiles->currentIndex());

    delete ui;
    delete emeraldIVs;
    delete emeraldPID;
    delete rs;
    delete frlg;
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
    int val = setting.value("egg3Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

void Eggs3::setupModels()
{
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

    ui->comboBoxEmeraldNature->setup();
    ui->comboBoxFRLGNature->setup();
    ui->comboBoxRSNature->setup();

    ui->comboBoxEmeraldHiddenPower->setup();
    ui->comboBoxFRLGHiddenPower->setup();
    ui->comboBoxRSHiddenPower->setup();

    ui->comboBoxEmeraldMethod->setItemData(0, Method::EBred);
    ui->comboBoxEmeraldMethod->setItemData(1, Method::EBredSplit);
    ui->comboBoxEmeraldMethod->setItemData(2, Method::EBredAlternate);

    ui->comboBoxRSMethod->setItemData(0, Method::RSBred);
    ui->comboBoxRSMethod->setItemData(1, Method::RSBredAlternate);

    ui->comboBoxFRLGMethod->setItemData(0, Method::FRLGBred);
    ui->comboBoxFRLGMethod->setItemData(1, Method::FRLGBredAlternate);
}

void Eggs3::refreshProfiles()
{
    emit alertProfiles(3);
}

void Eggs3::on_pushButtonEmeraldPIDGenerate_clicked()
{
    emeraldPID->clear();

    u32 startingFrame = ui->textBoxEmeraldPIDMinFrame->text().toUInt();
    u32 maxResults = ui->textBoxEmeraldPIDMaxFrame->text().toUInt();
    u16 tid = ui->textBoxEmeraldTID->text().toUShort();
    u16 sid = ui->textBoxEmeraldSID->text().toUShort();
    int genderRatioIndex = ui->comboBoxEmeraldGenderRatio->currentIndex();

    Egg3 generator = Egg3(maxResults, startingFrame, tid, sid, EBredPID);
    generator.setMinRedraw(ui->textBoxMinRedraws->text().toUInt());
    generator.setMaxRedraw(ui->textBoxMaxRedraws->text().toUInt());
    generator.setCalibration(ui->textBoxCalibration->text().toUInt());
    generator.setCompatability(ui->comboBoxEmeraldCompatibility->currentData().toUInt());
    generator.setEverstone(ui->comboBoxEverstone->currentIndex() != 0);
    if (ui->comboBoxEverstone->currentIndex() != 0)
    {
        generator.setEverstoneNature(Nature::getAdjustedNature(ui->comboBoxEverstone->currentIndex() - 1));
    }

    FrameCompare compare = FrameCompare(ui->comboBoxEmeraldGender->currentIndex(), genderRatioIndex, ui->comboBoxEmeraldAbility->currentIndex(),
                                        ui->comboBoxEmeraldNature->getChecked(), ui->checkBoxEmeraldShiny->isChecked());

    QVector<Frame3> frames = generator.generate(compare);
    emeraldPID->setModel(frames);
}

void Eggs3::on_pushButtonEmeraldIVsGenerate_clicked()
{
    emeraldIVs->clear();

    u32 startingFrame = ui->textBoxEmeraldIVsMinFrame->text().toUInt();
    u32 maxResults = ui->textBoxEmeraldIVsMaxFrame->text().toUInt();
    u16 tid = ui->textBoxEmeraldTID->text().toUShort();
    u16 sid = ui->textBoxEmeraldSID->text().toUShort();
    Method method = static_cast<Method>(ui->comboBoxEmeraldMethod->currentData().toUInt());

    QVector<u8> parent1 =
    {
        static_cast<u8>(ui->spinBoxEmeraldParent1HP->value()), static_cast<u8>(ui->spinBoxEmeraldParent1Atk->value()), static_cast<u8>(ui->spinBoxEmeraldParent1Def->value()),
        static_cast<u8>(ui->spinBoxEmeraldParent1SpA->value()), static_cast<u8>(ui->spinBoxEmeraldParent1SpD->value()), static_cast<u8>(ui->spinBoxEmeraldParent1Spe->value())
    };
    QVector<u8> parent2 =
    {
        static_cast<u8>(ui->spinBoxEmeraldParent2HP->value()), static_cast<u8>(ui->spinBoxEmeraldParent2Atk->value()), static_cast<u8>(ui->spinBoxEmeraldParent2Def->value()),
        static_cast<u8>(ui->spinBoxEmeraldParent2SpA->value()), static_cast<u8>(ui->spinBoxEmeraldParent2SpD->value()), static_cast<u8>(ui->spinBoxEmeraldParent2Spe->value())
    };

    Egg3 generator = Egg3(maxResults, startingFrame, tid, sid, method);
    generator.setParents(parent1, parent2);

    FrameCompare compare = FrameCompare(ui->ivFilterEmerald->getEvals(), ui->ivFilterEmerald->getValues(), ui->comboBoxEmeraldHiddenPower->getChecked());

    QVector<Frame3> frames = generator.generate(compare);
    emeraldIVs->setModel(frames);
}

void Eggs3::on_pushButtonRSGenerate_clicked()
{
    rs->clear();

    u32 minHeld = ui->textBoxRSMinHeld->text().toUInt();
    u32 maxHeld = ui->textBoxRSMaxHeld->text().toUInt();
    u16 tid = ui->textBoxRSTID->text().toUShort();
    u16 sid = ui->textBoxRSSID->text().toUShort();
    Method method = static_cast<Method>(ui->comboBoxRSMethod->currentData().toUInt());

    QVector<u8> parent1 =
    {
        static_cast<u8>(ui->spinBoxRSParent1HP->value()), static_cast<u8>(ui->spinBoxRSParent1Atk->value()), static_cast<u8>(ui->spinBoxRSParent1Def->value()),
        static_cast<u8>(ui->spinBoxRSParent1SpA->value()), static_cast<u8>(ui->spinBoxRSParent1SpD->value()), static_cast<u8>(ui->spinBoxRSParent1Spe->value())
    };
    QVector<u8> parent2 =
    {
        static_cast<u8>(ui->spinBoxRSParent2HP->value()), static_cast<u8>(ui->spinBoxRSParent2Atk->value()), static_cast<u8>(ui->spinBoxRSParent2Def->value()),
        static_cast<u8>(ui->spinBoxRSParent2SpA->value()), static_cast<u8>(ui->spinBoxRSParent2SpD->value()), static_cast<u8>(ui->spinBoxRSParent2Spe->value())
    };

    Egg3 generator = Egg3(maxHeld, minHeld, tid, sid, method, ui->textBoxRSSeedHeld->text().toUInt(nullptr, 16));
    generator.setPickupSeed(ui->textBoxRSSeedPickup->text().toUInt(nullptr, 16));
    generator.setParents(parent1, parent2);

    generator.setMinPickup(ui->textBoxRSMinPickup->text().toUInt());
    generator.setMaxPickup(ui->textBoxRSMaxPickup->text().toUInt());
    generator.setCompatability(ui->comboBoxRSCompatibility->currentData().toUInt());

    FrameCompare compare = FrameCompare(ui->ivFilterRS->getEvals(), ui->ivFilterRS->getValues(), ui->comboBoxRSGender->currentIndex(),
                                        ui->comboBoxRSGenderRatio->currentIndex(), ui->comboBoxRSAbility->currentIndex(), ui->comboBoxRSNature->getChecked(),
                                        ui->comboBoxRSHiddenPower->getChecked(), ui->checkBoxRSShiny->isChecked(), false);

    QVector<Frame3> frames = generator.generate(compare);
    rs->setModel(frames);
}

void Eggs3::on_pushButtonFRLGGenerate_clicked()
{
    frlg->clear();

    u32 minHeld = ui->textBoxFRLGMinHeld->text().toUInt();
    u32 maxHeld = ui->textBoxFRLGMaxHeld->text().toUInt();
    u16 tid = ui->textBoxFRLGTID->text().toUShort();
    u16 sid = ui->textBoxFRLGSID->text().toUShort();
    Method method = static_cast<Method>(ui->comboBoxFRLGMethod->currentData().toUInt());

    QVector<u8> parent1 =
    {
        static_cast<u8>(ui->spinBoxFRLGParent1HP->value()), static_cast<u8>(ui->spinBoxFRLGParent1Atk->value()), static_cast<u8>(ui->spinBoxFRLGParent1Def->value()),
        static_cast<u8>(ui->spinBoxFRLGParent1SpA->value()), static_cast<u8>(ui->spinBoxFRLGParent1SpD->value()), static_cast<u8>(ui->spinBoxFRLGParent1Spe->value())
    };
    QVector<u8> parent2 =
    {
        static_cast<u8>(ui->spinBoxFRLGParent2HP->value()), static_cast<u8>(ui->spinBoxFRLGParent2Atk->value()), static_cast<u8>(ui->spinBoxFRLGParent2Def->value()),
        static_cast<u8>(ui->spinBoxFRLGParent2SpA->value()), static_cast<u8>(ui->spinBoxFRLGParent2SpD->value()), static_cast<u8>(ui->spinBoxFRLGParent2Spe->value())
    };

    Egg3 generator = Egg3(maxHeld, minHeld, tid, sid, method, ui->textBoxFRLGSeedHeld->text().toUInt(nullptr, 16));
    generator.setPickupSeed(ui->textBoxFRLGSeedPickup->text().toUInt(nullptr, 16));
    generator.setParents(parent1, parent2);

    generator.setMinPickup(ui->textBoxFRLGMinPickup->text().toUInt());
    generator.setMaxPickup(ui->textBoxFRLGMaxPickup->text().toUInt());
    generator.setCompatability(ui->comboBoxFRLGCompatibility->currentData().toUInt());

    FrameCompare compare = FrameCompare(ui->ivFilterFRLG->getEvals(), ui->ivFilterFRLG->getValues(), ui->comboBoxFRLGGender->currentIndex(),
                                        ui->comboBoxFRLGGenderRatio->currentIndex(), ui->comboBoxFRLGAbility->currentIndex(), ui->comboBoxFRLGNature->getChecked(),
                                        ui->comboBoxFRLGHiddenPower->getChecked(), ui->checkBoxFRLGShiny->isChecked(), false);

    QVector<Frame3> frames = generator.generate(compare);
    frlg->setModel(frames);
}

void Eggs3::on_comboBoxProfiles_currentIndexChanged(int index)
{
    if (index < 0)
    {
        return;
    }

    auto profile = profiles[index];
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
