/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Util/Translator.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <QSettings>

JirachiChecksum::JirachiChecksum(QWidget *parent) : QWidget(parent), ui(new Ui::JirachiChecksum)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->tidTB->setValues(InputType::TIDSID);
    ui->sidTB->setValues(InputType::TIDSID);
    ui->minutesTB->setValues(InputType::Delay);
    ui->secondsTB->setValues(InputType::Delay);
    ui->framesTB->setValues(InputType::Delay);
    ui->searchMinutesTB->setValues(InputType::Delay);
    ui->searchSecondsTB->setValues(InputType::Delay);
    ui->searchFramesTB->setValues(InputType::Delay);
    ui->frameStyleTB->setValues(InputType::Delay);
    model = new QStandardItemModel(ui->tableView);
    model->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("Total Frame") << tr("Minute") << tr("Second") << tr("Frame") << tr("Options") << tr("PID") << tr("Shiny") << tr("Nature") << tr("HP") << tr("Atk") << tr("Def") << tr("SpA") << tr("SpD") << tr("Spe"));
    ui->tableView->setModel(model);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &JirachiChecksum::generate);
}

JirachiChecksum::~JirachiChecksum()
{
    QSettings setting;
    setting.setValue("jirachiChecksum/geometry", this->saveGeometry());

    delete ui;
}

u32 JirachiChecksum::optionlessTimelessSeed() const
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
    return c;
}

void JirachiChecksum::generateJirachi(QList<QStandardItem *> *row, u16 seed) const
{
    // TODO: make this use actual states instead of this
    PokeRNG rng(seed);

    // Method 1 Reverse [SEED] [PID] [PID] [IVS] [IVS]

    State state(0);

    u16 high = rng.nextUShort();
    u16 low = rng.nextUShort();
    u16 iv1 = rng.nextUShort();
    u16 iv2 = rng.nextUShort();

    state.setPID(high, low);
    state.setNature(state.getPID() % 25);
    state.setShiny<8>(20043, high ^ low);

    state.setIVs(iv1, iv2);
    *row << new QStandardItem(QString::number(state.getPID(),16).toUpper());
    *row << new QStandardItem(state.getShiny() == 2 ? tr("Square") : state.getShiny() == 1 ? tr("Star") : tr("No"));
    *row << new QStandardItem(QString::fromStdString(Translator::getNature(state.getNature())));
    *row << new QStandardItem(QString::number(state.getIV(0)));
    *row << new QStandardItem(QString::number(state.getIV(1)));
    *row << new QStandardItem(QString::number(state.getIV(2)));
    *row << new QStandardItem(QString::number(state.getIV(3)));
    *row << new QStandardItem(QString::number(state.getIV(4)));
    *row << new QStandardItem(QString::number(state.getIV(5)));

    return;
}

void JirachiChecksum::generate() const
{
    model->removeRows(0, model->rowCount());
    u32 c = optionlessTimelessSeed();
    u32 u32checksum;
    u16 u16checksum;
    u8 minute = ui->minutesTB->getUInt();
    u8 second = ui->secondsTB->getUInt();
    u8 frame = ui->framesTB->getUInt();
    if (ui->checkBoxOptions->isChecked())
    {
        for (u8 text = 0; text < 3; text++)
        {
            for (u8 scene = 0; scene < 2; scene++)
            {
                for (u8 style = 0; style < 2; style++)
                {
                    for (u8 sound = 0; sound < 2; sound++)
                    {
                        for (u8 button = 0; button < 3; button++)
                        {
                            for (u8 frameStyle = 0; frameStyle < 20; frameStyle++)
                            {
                                c = optionlessTimelessSeed();
                                if (text == 1)
                                {
                                    c += 0x1;
                                }
                                else if (text == 2)
                                {
                                    c += 0x2;
                                }
                                if (style)
                                {
                                    c += 0x200;
                                }
                                if (scene)
                                {
                                    c += 0x400;
                                }
                                if (sound)
                                {
                                    c += 0x100;
                                }
                                if (button == 1)
                                {
                                    c += 0x1000000;
                                }
                                else if (button == 2)
                                {
                                    c += 0x2000000;
                                }
                                c += frameStyle * 0x8;
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
                                c += ((ui->frameStyleTB->getUInt() - 1) * 0x8);
                                u32checksum = (minute + (second << 8)) + (frame << 16) + c;
                                u16checksum = ((u32checksum >> 16) + (u32checksum & 0xFFFF));
                                QList<QStandardItem *> row;
                                row << new QStandardItem(QString::number(u16checksum,16).toUpper());
                                row << new QStandardItem(QString::number(minute*60*60+second*60+frame));
                                row << new QStandardItem(QString::number(minute));
                                row << new QStandardItem(QString::number(second));
                                row << new QStandardItem(QString::number(frame));
                                row << new QStandardItem(QString("%1|%2|%3|%4|%5|%6").arg(QString::number(text), QString::number(scene), QString::number(style), QString::number(sound), QString::number(button), QString::number(frameStyle)));
                                generateJirachi(&row,u16checksum);
                                model->appendRow(row);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        u8 searchFrame = ui->searchFramesTB->getUInt()+frame;
        u8 searchSecond = ui->searchSecondsTB->getUInt()+second;
        u8 searchMinute = ui->searchMinutesTB->getUInt()+minute;
        if (searchFrame > 59)
        {
            searchSecond += 1;
        }
        if (searchSecond > 59)
        {
            searchMinute += 1;
        }
        searchFrame %= 60;
        searchSecond %= 60;
        searchMinute %= 60;
        for (; minute <= searchMinute; minute++)
        {
            for (; second <= ((minute == searchMinute) ? searchSecond : 59); second++)
            {
                for (; frame <= ((second == searchSecond) ? searchFrame : 59); frame++)
                {
                    u32checksum = (minute + (second << 8)) + (frame << 16) + c;
                    u16checksum = ((u32checksum >> 16) + (u32checksum & 0xFFFF));
                    QList<QStandardItem *> row;
                    row << new QStandardItem(QString::number(u16checksum,16).toUpper());
                    row << new QStandardItem(QString::number(minute*60*60+second*60+frame));
                    row << new QStandardItem(QString::number(minute));
                    row << new QStandardItem(QString::number(second));
                    row << new QStandardItem(QString::number(frame));
                    row << new QStandardItem("");
                    generateJirachi(&row,u16checksum);
                    model->appendRow(row);
                }
                frame = 0;
            }
            second = 0;
        }
    }
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
    u32 c = optionlessTimelessSeed();
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
    c += ((ui->frameStyleTB->getUInt() - 1) * 0x8);

    u32 u32checksum = (ui->minutesTB->getUInt() + (ui->secondsTB->getUInt() << 8)) + (ui->framesTB->getUInt() << 16) + c;
    u16 u16checksum = ((u32checksum >> 16) + (u32checksum & 0xFFFF));
    ui->seedLabel->setText(QString::number(u16checksum,16).toUpper());
}

// TODO: fix this mess
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

void JirachiChecksum::on_minutesTB_textChanged(const QString &arg1)
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
