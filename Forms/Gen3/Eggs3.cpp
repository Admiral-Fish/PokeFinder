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

    for (int i = 0; i < (int)profiles.size(); i++)
        ui->comboBoxProfiles->addItem(profiles[i].getProfileName());

    QSettings setting;
    int val = setting.value("egg3Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
        ui->comboBoxProfiles->setCurrentIndex(val);
}

void Eggs3::setupModels()
{
    ui->tableViewEmeraldIVs->setModel(emeraldIVs);

    ui->tableViewEmeraldPID->setModel(emeraldPID);

    ui->tableViewRS->setModel(rs);

    ui->tableViewFRLG->setModel(frlg);

    ui->textBoxMinFrameEmeraldPID->setValues(1, 32, true);
    ui->textBoxMaxFrameEmeraldPID->setValues(1, 32, true);
    ui->textBoxMinRedraws->setValues(0, 56, true);
    ui->textBoxMaxRedraws->setValues(0, 56, true);
    ui->textBoxCalibration->setValues(0, 56, true);
    ui->textBoxTIDEmerald->setValues(0, 48, true);
    ui->textBoxSIDEmerald->setValues(0, 48, true);
    ui->textBoxMinFrameEmeraldIVs->setValues(1, 32, true);
    ui->textBoxMaxFrameEmeraldIVs->setValues(1, 32, true);

    ui->textBoxSeedRS->setValues(0, 48, false);
    ui->textBoxMinHeldRS->setValues(1, 32, true);
    ui->textBoxMaxHeldRS->setValues(1, 32, true);
    ui->textBoxMinPickupRS->setValues(1, 32, true);
    ui->textBoxMaxPickupRS->setValues(1, 32, true);
    ui->textBoxTIDRS->setValues(0, 48, true);
    ui->textBoxSIDRS->setValues(0, 48, true);

    ui->textBoxSeedFRLG->setValues(0, 48, false);
    ui->textBoxMinHeldFRLG->setValues(1, 32, true);
    ui->textBoxMaxHeldFRLG->setValues(1, 32, true);
    ui->textBoxMinPickupFRLG->setValues(1, 32, true);
    ui->textBoxMaxPickupFRLG->setValues(1, 32, true);
    ui->textBoxTIDFRLG->setValues(0, 48, true);
    ui->textBoxSIDFRLG->setValues(0, 48, true);

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

void Eggs3::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
            default:
                break;
        }
    }
}

void Eggs3::on_comboBoxProfiles_currentIndexChanged(int index)
{
    auto profile = profiles[index >= 0 ? index : 0];

    ui->textBoxTIDEmerald->setText(QString::number(profile.getTid()));
    ui->textBoxSIDEmerald->setText(QString::number(profile.getSid()));
    ui->textBoxTIDRS->setText(QString::number(profile.getTid()));
    ui->textBoxSIDRS->setText(QString::number(profile.getSid()));
    ui->textBoxTIDFRLG->setText(QString::number(profile.getTid()));
    ui->textBoxSIDFRLG->setText(QString::number(profile.getSid()));
    ui->profileTID->setText(QString::number(profile.getTid()));
    ui->profileSID->setText(QString::number(profile.getSid()));
    ui->profileGame->setText(profile.getVersionString());
}

void Eggs3::on_pushButtonAnyAbilityEmerald_clicked()
{
    ui->comboBoxAbilityEmerald->setCurrentIndex(0);
}

void Eggs3::on_pushButtonAnyNatureEmerald_clicked()
{
    ui->comboBoxNatureEmerald->uncheckAll();
}

void Eggs3::on_pushButtonAnyAbilityRS_clicked()
{
    ui->comboBoxAbilityRS->setCurrentIndex(0);
}

void Eggs3::on_pushButtonAnyNatureRS_clicked()
{
    ui->comboBoxNatureRS->uncheckAll();
}

void Eggs3::on_pushButtonAnyAbilityFRLG_clicked()
{
    ui->comboBoxAbilityFRLG->setCurrentIndex(0);
}

void Eggs3::on_pushButtonAnyNatureFRLG_clicked()
{
    ui->comboBoxNatureFRLG->uncheckAll();
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
    u32 tid = ui->textBoxTIDEmerald->text().toUInt();
    u32 sid = ui->textBoxSIDEmerald->text().toUInt();
    int genderRatioIndex = ui->comboBoxGenderRatioEmerald->currentIndex();

    Egg3 generator = Egg3(maxResults, startingFrame, tid, sid, EBredPID);
    generator.setMinRedraw(ui->textBoxMinRedraws->text().toUInt());
    generator.setMaxRedraw(ui->textBoxMaxRedraws->text().toUInt());
    generator.setCalibration(ui->textBoxCalibration->text().toUInt());
    generator.setCompatability(ui->comboBoxCompatibilityEmerald->currentData().toUInt());
    generator.setEverstone(ui->comboBoxEverstone->currentIndex() != 0);
    if (ui->comboBoxEverstone->currentIndex() != 0)
        generator.setEverstoneNature(Nature::getAdjustedNature(ui->comboBoxEverstone->currentIndex() - 1));

    FrameCompare compare = FrameCompare(ui->comboBoxGenderEmerald->currentIndex(), genderRatioIndex, ui->comboBoxAbilityEmerald->currentIndex(),
                                        ui->comboBoxNatureEmerald->getChecked(), ui->checkBoxShinyEmerald->isChecked());

    vector<Frame3> frames = generator.generate(compare);
    emeraldPID->setModel(frames);
}

void Eggs3::on_pushButtonGenerateEmeraldIVs_clicked()
{
    emeraldIVs->clear();

    u32 startingFrame = ui->textBoxMinFrameEmeraldIVs->text().toUInt(NULL, 10);
    u32 maxResults = ui->textBoxMaxFrameEmeraldIVs->text().toUInt(NULL, 10);
    u32 tid = ui->textBoxTIDEmerald->text().toUInt(NULL, 10);
    u32 sid = ui->textBoxSIDEmerald->text().toUInt(NULL, 10);

    Method method = EBredAlternate;
    if (ui->radioButtonNormal->isChecked())
        method = EBred;
    else if (ui->radioButtonSplit->isChecked())
        method = EBredSplit;

    vector<u32> parent1 = { (u32)ui->parent1HPEmerald->value(), (u32)ui->parent1AtkEmerald->value(), (u32)ui->parent1DefEmerald->value(),
                            (u32)ui->parent1SpAEmerald->value(), (u32)ui->parent1SpDEmerald->value(), (u32)ui->parent1SpeEmerald->value()
                          };
    vector<u32> parent2 = { (u32)ui->parent2HPEmerald->value(), (u32)ui->parent2AtkEmerald->value(), (u32)ui->parent2DefEmerald->value(),
                            (u32)ui->parent2SpAEmerald->value(), (u32)ui->parent2SpDEmerald->value(), (u32)ui->parent2SpeEmerald->value()
                          };

    Egg3 generator = Egg3(maxResults, startingFrame, tid, sid, method);
    generator.setParents(parent1, parent2);

    FrameCompare compare = FrameCompare(ui->ivFilterEmerald->getEvals(), ui->ivFilterEmerald->getValues(), ui->comboBoxHiddenPowerEmerald->getChecked());

    vector<Frame3> frames = generator.generate(compare);
    emeraldIVs->setModel(frames);
}

void Eggs3::on_pushButtonGenerateRS_clicked()
{
    rs->clear();

    u32 minHeld = ui->textBoxMinHeldRS->text().toUInt(NULL, 10);
    u32 maxHeld = ui->textBoxMaxHeldRS->text().toUInt(NULL, 10);
    u32 tid = ui->textBoxTIDRS->text().toUInt(NULL, 10);
    u32 sid = ui->textBoxSIDRS->text().toUInt(NULL, 10);

    vector<u32> parent1 = { (u32)ui->parent1HPRS->value(), (u32)ui->parent1AtkRS->value(), (u32)ui->parent1DefRS->value(),
                            (u32)ui->parent1SpARS->value(), (u32)ui->parent1SpDRS->value(), (u32)ui->parent1SpeRS->value()
                          };
    vector<u32> parent2 = { (u32)ui->parent2HPRS->value(), (u32)ui->parent2AtkRS->value(), (u32)ui->parent2DefRS->value(),
                            (u32)ui->parent2SpARS->value(), (u32)ui->parent2SpDRS->value(), (u32)ui->parent2SpeRS->value()
                          };

    Egg3 generator = Egg3(maxHeld, minHeld, tid, sid, RSBred, ui->textBoxSeedRS->text().toUInt(NULL, 16));
    generator.setParents(parent1, parent2);

    generator.setMinPickup(ui->textBoxMinPickupRS->text().toUInt());
    generator.setMaxPickup(ui->textBoxMaxPickupRS->text().toUInt());
    generator.setCompatability(ui->comboBoxCompatibilityRS->currentData().toUInt());

    FrameCompare compare = FrameCompare(ui->ivFilterRS->getEvals(), ui->ivFilterRS->getValues(), ui->comboBoxGenderRS->currentIndex(),
                                        ui->comboBoxGenderRatioRS->currentIndex(), ui->comboBoxAbilityRS->currentIndex(), ui->comboBoxNatureRS->getChecked(),
                                        ui->comboBoxHiddenPowerRS->getChecked(), ui->checkBoxShinyRS->isChecked(), false);

    vector<Frame3> frames = generator.generate(compare);
    rs->setModel(frames);
}

void Eggs3::on_pushButtonGenerateFRLG_clicked()
{
    frlg->clear();

    u32 minHeld = ui->textBoxMinHeldFRLG->text().toUInt(NULL, 10);
    u32 maxHeld = ui->textBoxMaxHeldFRLG->text().toUInt(NULL, 10);
    u32 tid = ui->textBoxTIDFRLG->text().toUInt(NULL, 10);
    u32 sid = ui->textBoxSIDFRLG->text().toUInt(NULL, 10);

    vector<u32> parent1 = { (u32)ui->parent1HPFRLG->value(), (u32)ui->parent1AtkFRLG->value(), (u32)ui->parent1DefFRLG->value(),
                            (u32)ui->parent1SpAFRLG->value(), (u32)ui->parent1SpDFRLG->value(), (u32)ui->parent1SpeFRLG->value()
                          };
    vector<u32> parent2 = { (u32)ui->parent2HPFRLG->value(), (u32)ui->parent2AtkFRLG->value(), (u32)ui->parent2DefFRLG->value(),
                            (u32)ui->parent2SpAFRLG->value(), (u32)ui->parent2SpDFRLG->value(), (u32)ui->parent2SpeFRLG->value()
                          };

    Egg3 generator = Egg3(maxHeld, minHeld, tid, sid, FRLGBred, ui->textBoxSeedFRLG->text().toUInt(NULL, 16));
    generator.setParents(parent1, parent2);

    generator.setMinPickup(ui->textBoxMinPickupFRLG->text().toUInt());
    generator.setMaxPickup(ui->textBoxMaxPickupFRLG->text().toUInt());
    generator.setCompatability(ui->comboBoxCompatibilityFRLG->currentData().toUInt());

    FrameCompare compare = FrameCompare(ui->ivFilterFRLG->getEvals(), ui->ivFilterFRLG->getValues(), ui->comboBoxGenderFRLG->currentIndex(),
                                        ui->comboBoxGenderRatioFRLG->currentIndex(), ui->comboBoxAbilityFRLG->currentIndex(), ui->comboBoxNatureFRLG->getChecked(),
                                        ui->comboBoxHiddenPowerFRLG->getChecked(), ui->checkBoxShinyFRLG->isChecked(), false);

    vector<Frame3> frames = generator.generate(compare);
    frlg->setModel(frames);
}

void Eggs3::on_pushButtonProfileManager_clicked()
{
    ProfileManager3 *manager = new ProfileManager3();
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(refreshProfiles()));
    manager->show();
}
