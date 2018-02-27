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

#include "Egg3.hpp"
#include "ui_Egg3.h"

Egg3::Egg3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Egg3)
{
    ui->setupUi(this);

    setupModels();
}

Egg3::~Egg3()
{
    delete ui;
}

void Egg3::setupModels()
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
}

void Egg3::changeEvent(QEvent *event)
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

void Egg3::on_pushButtonProfileManagerEmerald_clicked()
{
    //ProfileManager3* manager = new ProfileManager3();
    //manager->setAttribute(Qt::WA_QuitOnClose, false);
    //connect(manager, SIGNAL(updateProfiles()), this, SLOT(updateProfiles()));
    //manager->show();
}
