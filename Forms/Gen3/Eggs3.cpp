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

    ui->textBoxMinFrameEmeraldPID->setValues(1, 32, true);
    ui->textBoxMaxFrameEmeraldPID->setValues(1, 32, true);
    ui->textBoxMinRedraws->setValues(0, 56, true);
    ui->textBoxMaxRedraws->setValues(0, 56, true);
    ui->textBoxCalibration->setValues(0, 56, true);
    ui->textBoxTIDEmerald->setValues(0, 48, true);
    ui->textBoxSIDEmerald->setValues(0, 48, true);
    ui->textBoxMinFrameEmeraldIVs->setValues(1, 32, true);
    ui->textBoxMaxFrameEmeraldIVs->setValues(1, 32, true);

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

    ui->comboBoxCompatibilityEmerald->clear();
    ui->comboBoxCompatibilityEmerald->addItem(tr("The two don't seem to like each other"), 20);
    ui->comboBoxCompatibilityEmerald->addItem(tr("The two seem to get along"), 50);
    ui->comboBoxCompatibilityEmerald->addItem(tr("The two seem to get along very well"), 70);
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

void Eggs3::on_pushButtonProfileManagerEmerald_clicked()
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
    if (ui->comboBoxEverstone->currentIndex() != 0)
    {
        generator.everstoneNature = Nature::getAdjustedNature(ui->comboBoxEverstone->currentIndex() - 1);
        generator.everstone = true;
    }
    else
        generator.everstone = false;

    FrameCompare compare = FrameCompare(ui->comboBoxGenderEmerald->currentIndex(), genderRatioIndex, ui->comboBoxAbilityEmerald->currentIndex(),
                                        ui->comboBoxNatureEmerald->getChecked(), ui->checkBoxShinyEmerald->isChecked());

    vector<Frame3> frames = generator.generate(compare);
    emeraldPID->setModel(frames);
    ui->tableViewEmeraldPID->viewport()->update();
}
