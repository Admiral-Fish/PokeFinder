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

#include "PokeSpot.hpp"
#include "ui_PokeSpot.h"

PokeSpot::PokeSpot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PokeSpot)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

PokeSpot::~PokeSpot()
{
    delete ui;
    delete model;
}

void PokeSpot::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                translate();
                break;
            default:
                break;
        }
    }
}

void PokeSpot::on_pushButtonAnyAbility_clicked()
{
    ui->comboBoxAbility->setCurrentIndex(0);
}

void PokeSpot::on_pushButtonAnyNature_clicked()
{
    ui->comboBoxNature->uncheckAll();
}

void PokeSpot::on_pushButtonAnySpotType_clicked()
{
    ui->comboBoxSpotType->uncheckAll();
}

void PokeSpot::setupModels()
{
    ui->textBoxSeed->setValues(0, 32, false);
    ui->textBoxStartingFrame->setValues(1, 32, true);
    ui->textBoxMaxResults->setValues(1, 32, true);
    ui->textBoxTID->setValues(0, 48, true);
    ui->textBoxSID->setValues(0, 48, true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);

    translate();
}

void PokeSpot::translate()
{
    vector<QString> natures = Nature::getNatures();
    //ui->comboBoxNature->addCheckItems(natures, QVariant(), Qt::Unchecked);

    //ui->comboBoxSpotType->clear();
    //ui->comboBoxSpotType->addCheckItem(tr("Common"), QVariant(), Qt::Unchecked);
    //ui->comboBoxSpotType->addCheckItem(tr("Uncommon"), QVariant(), Qt::Unchecked);
    //ui->comboBoxSpotType->addCheckItem(tr("Rare"), QVariant(), Qt::Unchecked);
}

void PokeSpot::on_pushButtonGenerate_clicked()
{
    model->clear();

    vector<Frame3> frames;

    u32 seed = ui->textBoxSeed->text().toUInt(NULL, 16);
    u32 initialFrame = ui->textBoxStartingFrame->text().toUInt(NULL, 10);
    u32 maxFrames = ui->textBoxMaxResults->text().toUInt(NULL, 10);
    u32 tid = ui->textBoxTID->text().toUInt(NULL, 10);
    u32 sid = ui->textBoxSID->text().toUInt(NULL, 10);

    int genderRatio = ui->comboBoxGenderRatio->currentIndex();

    rng.seed = seed;
    rng.advanceFrames(initialFrame - 1);

    rngList.clear();
    for (int x = 0; x < 5; x++)
        rngList.push_back(rng.nextUShort());

    u32 max = initialFrame + maxFrames;
    u32 call1, call2, call3;

    Frame3 frame = Frame3(tid, sid, tid ^ sid);
    FrameCompare compare = FrameCompare(ui->comboBoxGender->currentIndex(), genderRatio, ui->comboBoxAbility->currentIndex(),
                                        ui->comboBoxNature->getChecked(), ui->checkBoxShinyOnly->isChecked());
    frame.genderRatio = genderRatio;

    vector<bool> spots = ui->comboBoxSpotType->getChecked();

    for (u32 cnt = initialFrame; cnt < max; cnt++, rngList.erase(rngList.begin()), rngList.push_back(rng.nextUShort()))
    {
        // Check if frame is a valid pokespot call
        call1 = rngList[0] % 3;
        if (call1 != 0)
            continue;

        // Munchlax provides a frame skip
        call2 = rngList[1] % 100;
        if (call2 < 10)
            continue;

        // Check what type the pokespot is
        call3 = rngList[2] % 100;
        if (call3 < 50)
        {
            if (!spots[0])
                continue;
            frame.lockReason = tr("Common");
        }
        else if (call3 > 49 && call3 < 85)
        {
            if (!spots[1])
                continue;
            frame.lockReason = tr("Uncommon");
        }
        else
        {
            if (!spots[2])
                continue;
            frame.lockReason = tr("Rare");
        }

        frame.setPID(rngList[4], rngList[3]);
        if (compare.comparePID(frame))
        {
            frame.frame = cnt;
            frames.push_back(frame);
        }
    }
    model->setModel(frames);
}
