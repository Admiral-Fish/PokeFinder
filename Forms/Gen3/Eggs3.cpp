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

    ui->textBoxMinFrameEmeraldPID->setValues(InputType::Frame32Bit);
    ui->textBoxMaxFrameEmeraldPID->setValues(InputType::Frame32Bit);
    ui->textBoxMinRedraws->setValues(0, 255);
    ui->textBoxMaxRedraws->setValues(0, 255);
    ui->textBoxCalibration->setValues(0, 255);
    ui->textBoxTIDEmerald->setValues(InputType::TIDSID);
    ui->textBoxSIDEmerald->setValues(InputType::TIDSID);
    ui->textBoxMinFrameEmeraldIVs->setValues(InputType::Frame32Bit);
    ui->textBoxMaxFrameEmeraldIVs->setValues(InputType::Frame32Bit);

    ui->textBoxSeedRS->setValues(InputType::Seed16Bit);
    ui->textBoxMinHeldRS->setValues(InputType::Frame32Bit);
    ui->textBoxMaxHeldRS->setValues(InputType::Frame32Bit);
    ui->textBoxMinPickupRS->setValues(InputType::Frame32Bit);
    ui->textBoxMaxPickupRS->setValues(InputType::Frame32Bit);
    ui->textBoxTIDRS->setValues(InputType::TIDSID);
    ui->textBoxSIDRS->setValues(InputType::TIDSID);

    ui->textBoxSeedFRLG->setValues(InputType::Seed16Bit);
    ui->textBoxMinHeldFRLG->setValues(InputType::Frame32Bit);
    ui->textBoxMaxHeldFRLG->setValues(InputType::Frame32Bit);
    ui->textBoxMinPickupFRLG->setValues(InputType::Frame32Bit);
    ui->textBoxMaxPickupFRLG->setValues(InputType::Frame32Bit);
    ui->textBoxTIDFRLG->setValues(InputType::TIDSID);
    ui->textBoxSIDFRLG->setValues(InputType::TIDSID);

    ui->comboBoxCompatibilityEmerald->setItemData(0, 20);
    ui->comboBoxCompatibilityEmerald->setItemData(1, 50);
    ui->comboBoxCompatibilityEmerald->setItemData(2, 70);

    ui->comboBoxCompatibilityRS->setItemData(0, 20);
    ui->comboBoxCompatibilityRS->setItemData(1, 50);
    ui->comboBoxCompatibilityRS->setItemData(2, 70);

    ui->comboBoxCompatibilityFRLG->setItemData(0, 20);
    ui->comboBoxCompatibilityFRLG->setItemData(1, 50);
    ui->comboBoxCompatibilityFRLG->setItemData(2, 70);

    ui->comboBoxNatureEmerald->setup();
    ui->comboBoxNatureFRLG->setup();
    ui->comboBoxNatureRS->setup();

    ui->comboBoxHiddenPowerEmerald->setup();
    ui->comboBoxHiddenPowerFRLG->setup();
    ui->comboBoxHiddenPowerRS->setup();
}

void Eggs3::refreshProfiles()
{
    emit alertProfiles(3);
}

void Eggs3::on_pushButtonGenerateEmeraldPID_clicked()
{
    emeraldPID->clear();

    u32 startingFrame = ui->textBoxMinFrameEmeraldPID->text().toUInt();
    u32 maxResults = ui->textBoxMaxFrameEmeraldPID->text().toUInt();
    u16 tid = ui->textBoxTIDEmerald->text().toUShort();
    u16 sid = ui->textBoxSIDEmerald->text().toUShort();
    int genderRatioIndex = ui->comboBoxGenderRatioEmerald->currentIndex();

    Egg3 generator = Egg3(maxResults, startingFrame, tid, sid, EBredPID);
    generator.setMinRedraw(ui->textBoxMinRedraws->text().toUInt());
    generator.setMaxRedraw(ui->textBoxMaxRedraws->text().toUInt());
    generator.setCalibration(ui->textBoxCalibration->text().toUInt());
    generator.setCompatability(ui->comboBoxCompatibilityEmerald->currentData().toUInt());
    generator.setEverstone(ui->comboBoxEverstone->currentIndex() != 0);
    if (ui->comboBoxEverstone->currentIndex() != 0)
    {
        generator.setEverstoneNature(Nature::getAdjustedNature(ui->comboBoxEverstone->currentIndex() - 1));
    }

    FrameCompare compare = FrameCompare(ui->comboBoxGenderEmerald->currentIndex(), genderRatioIndex, ui->comboBoxAbilityEmerald->currentIndex(),
                                        ui->comboBoxNatureEmerald->getChecked(), ui->checkBoxShinyEmerald->isChecked());

    QVector<Frame3> frames = generator.generate(compare);
    emeraldPID->setModel(frames);
}

void Eggs3::on_pushButtonGenerateEmeraldIVs_clicked()
{
    emeraldIVs->clear();

    u32 startingFrame = ui->textBoxMinFrameEmeraldIVs->text().toUInt();
    u32 maxResults = ui->textBoxMaxFrameEmeraldIVs->text().toUInt();
    u16 tid = ui->textBoxTIDEmerald->text().toUShort();
    u16 sid = ui->textBoxSIDEmerald->text().toUShort();

    Method method = Method::EBredAlternate;
    if (ui->radioButtonNormal->isChecked())
    {
        method = Method::EBred;
    }
    else if (ui->radioButtonSplit->isChecked())
    {
        method = Method::EBredSplit;
    }

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

    FrameCompare compare = FrameCompare(ui->ivFilterEmerald->getEvals(), ui->ivFilterEmerald->getValues(), ui->comboBoxHiddenPowerEmerald->getChecked());

    QVector<Frame3> frames = generator.generate(compare);
    emeraldIVs->setModel(frames);
}

void Eggs3::on_pushButtonGenerateRS_clicked()
{
    rs->clear();

    u32 minHeld = ui->textBoxMinHeldRS->text().toUInt();
    u32 maxHeld = ui->textBoxMaxHeldRS->text().toUInt();
    u16 tid = ui->textBoxTIDRS->text().toUShort();
    u16 sid = ui->textBoxSIDRS->text().toUShort();

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

    Egg3 generator = Egg3(maxHeld, minHeld, tid, sid, RSBred, ui->textBoxSeedRS->text().toUInt(nullptr, 16));
    generator.setParents(parent1, parent2);

    generator.setMinPickup(ui->textBoxMinPickupRS->text().toUInt());
    generator.setMaxPickup(ui->textBoxMaxPickupRS->text().toUInt());
    generator.setCompatability(ui->comboBoxCompatibilityRS->currentData().toUInt());

    FrameCompare compare = FrameCompare(ui->ivFilterRS->getEvals(), ui->ivFilterRS->getValues(), ui->comboBoxGenderRS->currentIndex(),
                                        ui->comboBoxGenderRatioRS->currentIndex(), ui->comboBoxAbilityRS->currentIndex(), ui->comboBoxNatureRS->getChecked(),
                                        ui->comboBoxHiddenPowerRS->getChecked(), ui->checkBoxShinyRS->isChecked(), false);

    QVector<Frame3> frames = generator.generate(compare);
    rs->setModel(frames);
}

void Eggs3::on_pushButtonGenerateFRLG_clicked()
{
    frlg->clear();

    u32 minHeld = ui->textBoxMinHeldFRLG->text().toUInt();
    u32 maxHeld = ui->textBoxMaxHeldFRLG->text().toUInt();
    u16 tid = ui->textBoxTIDFRLG->text().toUShort();
    u16 sid = ui->textBoxSIDFRLG->text().toUShort();

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

    Egg3 generator = Egg3(maxHeld, minHeld, tid, sid, FRLGBred, ui->textBoxSeedFRLG->text().toUInt(nullptr, 16));
    generator.setParents(parent1, parent2);

    generator.setMinPickup(ui->textBoxMinPickupFRLG->text().toUInt());
    generator.setMaxPickup(ui->textBoxMaxPickupFRLG->text().toUInt());
    generator.setCompatability(ui->comboBoxCompatibilityFRLG->currentData().toUInt());

    FrameCompare compare = FrameCompare(ui->ivFilterFRLG->getEvals(), ui->ivFilterFRLG->getValues(), ui->comboBoxGenderFRLG->currentIndex(),
                                        ui->comboBoxGenderRatioFRLG->currentIndex(), ui->comboBoxAbilityFRLG->currentIndex(), ui->comboBoxNatureFRLG->getChecked(),
                                        ui->comboBoxHiddenPowerFRLG->getChecked(), ui->checkBoxShinyFRLG->isChecked(), false);

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

    ui->textBoxTIDEmerald->setText(tid);
    ui->textBoxSIDEmerald->setText(sid);
    ui->textBoxTIDRS->setText(tid);
    ui->textBoxSIDRS->setText(sid);
    ui->textBoxTIDFRLG->setText(tid);
    ui->textBoxSIDFRLG->setText(sid);
    ui->labelProfileTIDValue->setText(tid);
    ui->labelProfileSIDValue->setText(sid);
    ui->labelProfileGameValue->setText(profile.getVersionString());
}

void Eggs3::on_pushButtonAnyAbilityEmerald_clicked()
{
    ui->comboBoxAbilityEmerald->setCurrentIndex(0);
}

void Eggs3::on_pushButtonAnyAbilityRS_clicked()
{
    ui->comboBoxAbilityRS->setCurrentIndex(0);
}

void Eggs3::on_pushButtonAnyAbilityFRLG_clicked()
{
    ui->comboBoxAbilityFRLG->setCurrentIndex(0);
}

void Eggs3::on_pushButtonProfileManager_clicked()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, &Eggs3::refreshProfiles);
    manager->show();
}
