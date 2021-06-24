/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "JirachiChecksum.hpp"
#include "ui_JirachiChecksum.h"
#include <QSettings>
#include <QStandardItemModel>

JirachiChecksum::JirachiChecksum(QWidget *parent) : QWidget(parent), ui(new Ui::JirachiChecksum)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->tidTB->setValues(InputType::TIDSID);
    ui->sidTB->setValues(InputType::TIDSID);
    ui->minuteTB->setValues(InputType::Delay);
    ui->secondsTB->setValues(InputType::Delay);
    ui->framesTB->setValues(InputType::Delay);
    ui->frameStyleTB->setValues(InputType::Delay);
}

JirachiChecksum::~JirachiChecksum()
{
    QSettings setting;
    setting.setValue("jirachiChecksum/geometry", this->saveGeometry());

    delete ui;
}

u32 JirachiChecksum::nameConvert(QString name) const
{
    QMap<QString, u32> characters;
    characters["A"] = 0xBB;
    characters["B"] = 0xBC;
    characters["C"] = 0xBD;
    characters["D"] = 0xBE;
    characters["E"] = 0xBF;
    characters["F"] = 0xC0;
    characters["G"] = 0xC1;
    characters["H"] = 0xC2;
    characters["I"] = 0xC3;
    characters["J"] = 0xC4;
    characters["K"] = 0xC5;
    characters["L"] = 0xC6;
    characters["M"] = 0xC7;
    characters["N"] = 0xC8;
    characters["O"] = 0xC9;
    characters["P"] = 0xCA;
    characters["Q"] = 0xCB;
    characters["R"] = 0xCC;
    characters["S"] = 0xCD;
    characters["T"] = 0xCE;
    characters["U"] = 0xCF;
    characters["V"] = 0xD0;
    characters["W"] = 0xD1;
    characters["X"] = 0xD2;
    characters["Y"] = 0xD3;
    characters["Z"] = 0xD4;

    characters["a"] = 0xD5;
    characters["b"] = 0xD6;
    characters["c"] = 0xD7;
    characters["d"] = 0xD8;
    characters["e"] = 0xD9;
    characters["f"] = 0xDA;
    characters["g"] = 0xDB;
    characters["h"] = 0xDC;
    characters["i"] = 0xDD;
    characters["j"] = 0xDE;
    characters["k"] = 0xDF;
    characters["l"] = 0xE0;
    characters["m"] = 0xE1;
    characters["n"] = 0xE2;
    characters["o"] = 0xE3;
    characters["p"] = 0xE4;
    characters["q"] = 0xE5;
    characters["r"] = 0xE6;
    characters["s"] = 0xE7;
    characters["t"] = 0xE8;
    characters["u"] = 0xE9;
    characters["v"] = 0xEA;
    characters["w"] = 0xEB;
    characters["x"] = 0xEC;
    characters["y"] = 0xED;
    characters["z"] = 0xEE;


    characters[" "] = 0x00;
    characters["．"] = 0xAD;
    characters["，"] = 0xB8;

    characters["0"] = 0xA1;
    characters["1"] = 0xA2;
    characters["2"] = 0xA3;
    characters["3"] = 0xA4;
    characters["4"] = 0xA5;
    characters["5"] = 0xA6;
    characters["6"] = 0xA7;
    characters["7"] = 0xA8;
    characters["8"] = 0xA9;
    characters["9"] = 0xAA;

    characters["！"] = 0xAB;
    characters["？"] = 0xAC;

    characters["♂"] = 0xB5;
    characters["♀"] = 0xB6;

    characters["／"] = 0xBA;

    characters["－"] = 0xAE;

    characters["‥"] = 0xB0;
    characters["“"] = 0xB1;
    characters["”"] = 0xB2;
    characters["‘"] = 0xB3;
    characters["’"] = 0xB4;

    u32 tvalue = 0xFF000000;
    int num = 0;
    do
    {
        if (num < name.size())
        {
            tvalue = ((((u64)tvalue) + ((u64)(characters[name.at(num)] << ((num % 4) * 8)))) & 0xFFFFFFFFu);
        }
        else
        {
            tvalue = ((((u64)tvalue) + ((u64)(255 << ((num % 4) * 8)))) & 0xFFFFFFFFu);
        }
        num++;
    }
    while (num <= 6);
    return tvalue;
}

void JirachiChecksum::on_Change()
{
    u32 c = 0x10;
    c += ui->tidTB->getUShort()<<16;
    c += ui->sidTB->getUShort();
    c += nameConvert(ui->nameTB->text());
    if (ui->femaleRadio->isChecked())
    {
        c += 1;
    }
    if (ui->xnRadio->isChecked())
    {
        c += 0x180000;
    }
    else if (ui->xxRadio->isChecked())
    {
        c += 0x3C170000;
    }
    else
    {
        c += 0x1;
    }
    if (ui->treeckoRadio->isChecked())
    {
        c += 0x08000000 + 0x40000000 + 0x08000000;
    }
    else if (ui->torchicRadio->isChecked())
    {
        c += 0x40000000 + 0x2 + 0x40000000;
    }
    else
    {
        c += 0x2 + 0x08000000 + 0x2;
    }
    if (ui->midRadio->isChecked())
    {
        c += 0x1;
    }
    else if (ui->fastRadio->isChecked())
    {
        c += 0x2;
    }
    if (ui->offRadio->isChecked())
    {
        c += 0x400;
    }
    if (ui->setRadio->isChecked())
    {
        c += 0x200;
    }
    if (ui->stereoRadio->isChecked())
    {
        c += 0x100;
    }
    if (ui->lrRadio->isChecked())
    {
        c += 0x1000000;
    }
    else if (ui->laRadio->isChecked())
    {
        c += 0x2000000;
    }
    if (ui->zigzagoonCheck->isChecked())
    {
        c += 0x40;
    }
    if (ui->wurmpleRadio->isChecked())
    {
        c += 0x100;
    }
    if (ui->wingullCheck->isChecked())
    {
        c += 0x200000;
    }
    c += ((ui->frameStyleTB->getUInt() - 1) * 0x8);

    u32 u32checksum = (ui->minuteTB->getUInt() + (ui->secondsTB->getUInt() << 8)) + (ui->framesTB->getUInt() << 16) + c;
    u16 u16checksum = ((u32checksum >> 16) + (u32checksum & 0xFFFF));
    ui->seedLabel->setText(QString::number(u16checksum,16).toUpper());
}

void JirachiChecksum::on_nameTB_textChanged(const QString &arg1)
{
    on_Change();
}

void JirachiChecksum::on_tidTB_textChanged(const QString &arg1)
{
    on_Change();
}

void JirachiChecksum::on_sidTB_textChanged(const QString &arg1)
{
    on_Change();
}

void JirachiChecksum::on_frameStyleTB_textChanged(const QString &arg1)
{
    on_Change();
}

void JirachiChecksum::on_minuteTB_textChanged(const QString &arg1)
{
    on_Change();
}

void JirachiChecksum::on_secondsTB_textChanged(const QString &arg1)
{
    on_Change();
}

void JirachiChecksum::on_framesTB_textChanged(const QString &arg1)
{
    on_Change();
}

void JirachiChecksum::on_maleRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_femaleRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_slowRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_midRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_fastRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_onRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_offRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_shiftRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_setRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_monRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_stereoRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_naRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_lrRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_laRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_nnRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_xnRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_xxRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_treeckoRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_torchicRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_mudkipRadio_clicked()
{
    on_Change();
}

void JirachiChecksum::on_zigzagoonCheck_toggled(bool checked)
{
    on_Change();
}

void JirachiChecksum::on_wurmpleRadio_toggled(bool checked)
{
    on_Change();
}

void JirachiChecksum::on_wingullCheck_toggled(bool checked)
{
    on_Change();
}
