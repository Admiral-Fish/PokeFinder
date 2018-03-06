/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

    setupModels();
    updateProfiles();
}

Eggs3::~Eggs3()
{
    delete ui;
}

void Eggs3::updateProfiles()
{
    profiles = Profile3::loadProfileList();

    QStandardItemModel *profile = new QStandardItemModel((int)profiles.size() + 1, 1, this);
    QStandardItem* firstProfile = new QStandardItem(tr("None"));
    profile->setItem(0, firstProfile);
    for(int i = 0; i < (int)profiles.size(); i++)
    {
        QStandardItem* item = new QStandardItem(profiles.at(i).profileName);
        profile->setItem(i + 1, item);
    }
    ui->comboBoxProfiles->setModel(profile);
}

void Eggs3::setupModels()
{
    vector<QString> natureList = Nature::getNatures();
    ui->comboBoxNatureEmerald->addCheckItems(natureList, QVariant(), Qt::Unchecked);
    ui->comboBoxNatureRS->addCheckItems(natureList, QVariant(), Qt::Unchecked);
    ui->comboBoxNatureFRLG->addCheckItems(natureList, QVariant(), Qt::Unchecked);

    vector<QString> hpList = Power::getPowers();
    ui->comboBoxHiddenPowerEmerald->addCheckItems(hpList, QVariant(), Qt::Unchecked);
    ui->comboBoxHiddenPowerRS->addCheckItems(hpList, QVariant(), Qt::Unchecked);
    ui->comboBoxHiddenPowerFRLG->addCheckItems(hpList, QVariant(), Qt::Unchecked);

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

    if (emeraldIVs != NULL)
        delete emeraldIVs;
    emeraldIVs = new Egg3Model(this, EBred);
    ui->tableViewEmeraldIVs->setModel(emeraldIVs);
    ui->tableViewEmeraldIVs->verticalHeader()->setVisible(false);
    ui->tableViewEmeraldIVs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if (emeraldPID != NULL)
        delete emeraldPID;
    emeraldPID = new Egg3Model(this, EBredPID);
    ui->tableViewEmeraldPID->setModel(emeraldPID);
    ui->tableViewEmeraldPID->verticalHeader()->setVisible(false);
    ui->tableViewEmeraldPID->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if (rs != NULL)
        delete rs;
    rs = new Egg3Model(this, RSBred);
    ui->tableViewRS->setModel(rs);
    ui->tableViewRS->verticalHeader()->setVisible(false);
    ui->tableViewRS->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if (frlg != NULL)
        delete frlg;
    frlg = new Egg3Model(this, FRLGBred);
    ui->tableViewFRLG->setModel(frlg);
    ui->tableViewFRLG->verticalHeader()->setVisible(false);
    ui->tableViewFRLG->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->comboBoxCompatibilityEmerald->clear();
    ui->comboBoxCompatibilityEmerald->addItem(tr("The two don't seem to like each other"), 20);
    ui->comboBoxCompatibilityEmerald->addItem(tr("The two seem to get along"), 50);
    ui->comboBoxCompatibilityEmerald->addItem(tr("The two seem to get along very well"), 70);

    ui->comboBoxCompatibilityRS->clear();
    ui->comboBoxCompatibilityRS->addItem(tr("The two don't seem to like each other"), 20);
    ui->comboBoxCompatibilityRS->addItem(tr("The two seem to get along"), 50);
    ui->comboBoxCompatibilityRS->addItem(tr("The two seem to get along very well"), 70);

    ui->comboBoxCompatibilityFRLG->clear();
    ui->comboBoxCompatibilityFRLG->addItem(tr("The two don't seem to like each other"), 20);
    ui->comboBoxCompatibilityFRLG->addItem(tr("The two seem to get along"), 50);
    ui->comboBoxCompatibilityFRLG->addItem(tr("The two seem to get along very well"), 70);
}

void Eggs3::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                setupModels();
                break;
            default:
                break;
        }
    }
}

void Eggs3::on_comboBoxProfiles_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->textBoxTIDEmerald->setText("12345");
        ui->textBoxSIDEmerald->setText("54321");
        ui->textBoxTIDRS->setText("12345");
        ui->textBoxSIDRS->setText("54321");
        ui->textBoxTIDFRLG->setText("12345");
        ui->textBoxSIDFRLG->setText("54321");
    }
    else
    {
        ui->textBoxTIDEmerald->setText(QString::number(profiles.at(index - 1).tid));
        ui->textBoxSIDEmerald->setText(QString::number(profiles.at(index - 1).sid));
        ui->textBoxTIDRS->setText(QString::number(profiles.at(index - 1).tid));
        ui->textBoxSIDRS->setText(QString::number(profiles.at(index - 1).sid));
        ui->textBoxTIDFRLG->setText(QString::number(profiles.at(index - 1).tid));
        ui->textBoxSIDFRLG->setText(QString::number(profiles.at(index - 1).sid));
    }
}

void Eggs3::on_pushButtonProfileManagerEmerald_clicked()
{
    ProfileManager3* manager = new ProfileManager3();
    manager->setAttribute(Qt::WA_QuitOnClose, false);
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(refreshProfiles()));
    manager->show();
}

void Eggs3::on_pushButtonProfileManagerRS_clicked()
{
    ProfileManager3* manager = new ProfileManager3();
    manager->setAttribute(Qt::WA_QuitOnClose, false);
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(refreshProfiles()));
    manager->show();
}

void Eggs3::on_pushButtonProfileManagerFRLG_clicked()
{
    ProfileManager3* manager = new ProfileManager3();
    manager->setAttribute(Qt::WA_QuitOnClose, false);
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(refreshProfiles()));
    manager->show();
}

void Eggs3::refreshProfiles()
{
    emit alertProfiles(3);
}

void Eggs3::on_pushButtonGenerateEmeraldPID_clicked()
{
    if (emeraldPID != NULL)
        delete emeraldPID;
    emeraldPID = new Egg3Model(this, EBredPID);
    ui->tableViewEmeraldPID->setModel(emeraldPID);

    u32 startingFrame = ui->textBoxMinFrameEmeraldPID->text().toUInt(NULL, 10);
    u32 maxResults = ui->textBoxMaxFrameEmeraldPID->text().toUInt(NULL, 10);
    u32 tid = ui->textBoxTIDEmerald->text().toUInt(NULL, 10);
    u32 sid = ui->textBoxSIDEmerald->text().toUInt(NULL, 10);
    int genderRatioIndex = ui->comboBoxGenderRatioEmerald->currentIndex();

    Egg3 generator = Egg3(maxResults, startingFrame, tid, sid, EBredPID);
    generator.minRedraw = ui->textBoxMinRedraws->text().toUInt(NULL, 10);
    generator.maxRedraw = ui->textBoxMaxRedraws->text().toUInt(NULL, 10);
    generator.calibration = ui->textBoxCalibration->text().toUInt(NULL, 10);
    generator.compatability = ui->comboBoxCompatibilityEmerald->currentData().toUInt(NULL);
    generator.everstone = ui->comboBoxEverstone->currentIndex() != 0;
    if (ui->comboBoxEverstone->currentIndex() != 0)
        generator.everstoneNature = Nature::getAdjustedNature(ui->comboBoxEverstone->currentIndex() - 1);

    FrameCompare compare = FrameCompare(ui->comboBoxGenderEmerald->currentIndex(), genderRatioIndex, ui->comboBoxAbilityEmerald->currentIndex(),
                                        ui->comboBoxNatureEmerald->getChecked(), ui->checkBoxShinyEmerald->isChecked());

    vector<Frame3> frames = generator.generate(compare);
    emeraldPID->setModel(frames);
    ui->tableViewEmeraldPID->viewport()->update();
}

void Eggs3::on_pushButtonGenerateEmeraldIVs_clicked()
{
    if (emeraldIVs != NULL)
        delete emeraldIVs;
    emeraldIVs = new Egg3Model(this, EBred);
    ui->tableViewEmeraldIVs->setModel(emeraldIVs);

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
                            (u32)ui->parent1SpAEmerald->value(), (u32)ui->parent1SpDEmerald->value(), (u32)ui->parent1SpeEmerald->value() };
    vector<u32> parent2 = { (u32)ui->parent2HPEmerald->value(), (u32)ui->parent2AtkEmerald->value(), (u32)ui->parent2DefEmerald->value(),
                            (u32)ui->parent2SpAEmerald->value(), (u32)ui->parent2SpDEmerald->value(), (u32)ui->parent2SpeEmerald->value() };

    Egg3 generator = Egg3(maxResults, startingFrame, tid, sid, method);
    generator.setParents(parent1, parent2);

    FrameCompare compare = FrameCompare(ui->ivFilterEmerald->getEvals(), ui->ivFilterEmerald->getValues(), ui->comboBoxHiddenPowerEmerald->getChecked());

    vector<Frame3> frames = generator.generate(compare);
    emeraldIVs->setModel(frames);
    ui->tableViewEmeraldPID->viewport()->update();
}

void Eggs3::on_pushButtonGenerateRS_clicked()
{
    if (rs != NULL)
        delete rs;
    rs = new Egg3Model(this, RSBred);
    ui->tableViewRS->setModel(rs);

    u32 minHeld = ui->textBoxMinHeldRS->text().toUInt(NULL, 10);
    u32 maxHeld = ui->textBoxMaxHeldRS->text().toUInt(NULL, 10);
    u32 tid = ui->textBoxTIDRS->text().toUInt(NULL, 10);
    u32 sid = ui->textBoxSIDRS->text().toUInt(NULL, 10);

    vector<u32> parent1 = { (u32)ui->parent1HPRS->value(), (u32)ui->parent1AtkRS->value(), (u32)ui->parent1DefRS->value(),
                            (u32)ui->parent1SpARS->value(), (u32)ui->parent1SpDRS->value(), (u32)ui->parent1SpeRS->value() };
    vector<u32> parent2 = { (u32)ui->parent2HPRS->value(), (u32)ui->parent2AtkRS->value(), (u32)ui->parent2DefRS->value(),
                            (u32)ui->parent2SpARS->value(), (u32)ui->parent2SpDRS->value(), (u32)ui->parent2SpeRS->value() };

    Egg3 generator = Egg3(maxHeld, minHeld, tid, sid, RSBred);
    generator.setParents(parent1, parent2);
    generator.seed = ui->textBoxSeedRS->text().toUInt(NULL, 16);

    generator.minPickup = ui->textBoxMinPickupRS->text().toUInt(NULL, 10);
    generator.maxPickup = ui->textBoxMaxPickupRS->text().toUInt(NULL, 10);
    generator.compatability = ui->comboBoxCompatibilityRS->currentData().toUInt(NULL);

    FrameCompare compare = FrameCompare(ui->ivFilterRS->getEvals(), ui->ivFilterRS->getValues(), ui->comboBoxGenderRS->currentIndex(),
                            ui->comboBoxGenderRatioRS->currentIndex(), ui->comboBoxAbilityRS->currentIndex(), ui->comboBoxNatureRS->getChecked(),
                            ui->comboBoxHiddenPowerRS->getChecked(), ui->checkBoxShinyRS->isChecked(), false);

    vector<Frame3> frames = generator.generate(compare);
    rs->setModel(frames);
    ui->tableViewRS->viewport()->update();
}

void Eggs3::on_pushButtonGenerateFRLG_clicked()
{
    if (frlg != NULL)
        delete frlg;
    frlg = new Egg3Model(this, RSBred);
    ui->tableViewFRLG->setModel(frlg);

    u32 minHeld = ui->textBoxMinHeldFRLG->text().toUInt(NULL, 10);
    u32 maxHeld = ui->textBoxMaxHeldFRLG->text().toUInt(NULL, 10);
    u32 tid = ui->textBoxTIDFRLG->text().toUInt(NULL, 10);
    u32 sid = ui->textBoxSIDFRLG->text().toUInt(NULL, 10);

    vector<u32> parent1 = { (u32)ui->parent1HPFRLG->value(), (u32)ui->parent1AtkFRLG->value(), (u32)ui->parent1DefFRLG->value(),
                            (u32)ui->parent1SpAFRLG->value(), (u32)ui->parent1SpDFRLG->value(), (u32)ui->parent1SpeFRLG->value() };
    vector<u32> parent2 = { (u32)ui->parent2HPFRLG->value(), (u32)ui->parent2AtkFRLG->value(), (u32)ui->parent2DefFRLG->value(),
                            (u32)ui->parent2SpAFRLG->value(), (u32)ui->parent2SpDFRLG->value(), (u32)ui->parent2SpeFRLG->value() };

    Egg3 generator = Egg3(maxHeld, minHeld, tid, sid, FRLGBred);
    generator.setParents(parent1, parent2);
    generator.seed = ui->textBoxSeedFRLG->text().toUInt(NULL, 16);

    generator.minPickup = ui->textBoxMinPickupFRLG->text().toUInt(NULL, 10);
    generator.maxPickup = ui->textBoxMaxPickupFRLG->text().toUInt(NULL, 10);
    generator.compatability = ui->comboBoxCompatibilityFRLG->currentData().toUInt(NULL);

    FrameCompare compare = FrameCompare(ui->ivFilterFRLG->getEvals(), ui->ivFilterFRLG->getValues(), ui->comboBoxGenderFRLG->currentIndex(),
                            ui->comboBoxGenderRatioFRLG->currentIndex(), ui->comboBoxAbilityFRLG->currentIndex(), ui->comboBoxNatureFRLG->getChecked(),
                            ui->comboBoxHiddenPowerFRLG->getChecked(), ui->checkBoxShinyFRLG->isChecked(), false);

    vector<Frame3> frames = generator.generate(compare);
    frlg->setModel(frames);
    ui->tableViewFRLG->viewport()->update();
}
