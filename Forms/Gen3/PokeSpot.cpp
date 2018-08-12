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

#include "PokeSpot.hpp"
#include "ui_PokeSpot.h"

PokeSpot::PokeSpot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PokeSpot)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

PokeSpot::~PokeSpot()
{
    saveSettings();

    delete ui;
    delete model;
    delete rng;
}

void PokeSpot::changeEvent(QEvent *event)
{
    if (event)
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

    ui->comboBoxNature->setup();
    ui->comboBoxSpotType->setup();

    rng = new XDRNG(0);

    loadSettings();
}

void PokeSpot::saveSettings()
{
    QSettings setting;
    setting.setValue("pokespotTID", ui->textBoxTID->text());
    setting.setValue("pokespotSID", ui->textBoxSID->text());
}

void PokeSpot::loadSettings()
{
    QSettings setting;
    if (setting.contains("pokespotTID")) ui->textBoxTID->setText(setting.value("pokespotTID").toString());
    if (setting.contains("pokespotSID")) ui->textBoxSID->setText(setting.value("pokespotSID").toString());
}

void PokeSpot::on_pushButtonGenerate_clicked()
{
    model->clear();

    QVector<Frame3> frames;

    u32 seed = ui->textBoxSeed->text().toUInt(nullptr, 16);
    u32 initialFrame = ui->textBoxStartingFrame->text().toUInt();
    u32 maxFrames = ui->textBoxMaxResults->text().toUInt();
    u16 tid = ui->textBoxTID->text().toUShort();
    u16 sid = ui->textBoxSID->text().toUShort();

    int genderRatio = ui->comboBoxGenderRatio->currentIndex();

    rng->setSeed(seed, initialFrame - 1);

    QVector<u16> rngList;
    for (int x = 0; x < 5; x++)
        rngList.append(rng->nextUShort());

    u32 max = initialFrame + maxFrames;
    u32 call1, call2, call3;

    Frame3 frame = Frame3(tid, sid, tid ^ sid);
    FrameCompare compare = FrameCompare(ui->comboBoxGender->currentIndex(), genderRatio, ui->comboBoxAbility->currentIndex(),
                                        ui->comboBoxNature->getChecked(), ui->checkBoxShinyOnly->isChecked());
    frame.setGenderRatio(static_cast<u32>(genderRatio));

    QVector<bool> spots = ui->comboBoxSpotType->getChecked();

    for (u32 cnt = initialFrame; cnt < max; cnt++, rngList.removeFirst(), rngList.append(rng->nextUShort()))
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
            frame.setLockReason(tr("Common"));
        }
        else if (call3 > 49 && call3 < 85)
        {
            if (!spots[1])
                continue;
            frame.setLockReason(tr("Uncommon"));
        }
        else
        {
            if (!spots[2])
                continue;
            frame.setLockReason(tr("Rare"));
        }

        frame.setPID(rngList[4], rngList[3]);
        if (compare.comparePID(frame))
        {
            frame.setFrame(cnt);
            frames.append(frame);
        }
    }
    model->setModel(frames);
}
