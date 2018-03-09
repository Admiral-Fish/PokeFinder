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

#include "IVtoPID_SID_SEED.hpp"
#include "ui_IVtoPID_SID_SEED.h"

IVtoPID_SID_SEED::IVtoPID_SID_SEED(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IVtoPID_SID_SEED)
{
    ui->setupUi(this);

    setupModels();

    ui->tableView->setModel(m);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->textBoxID->setValues(0, 48, true);
}

IVtoPID_SID_SEED::~IVtoPID_SID_SEED()
{
    delete ui;
    delete m;
}

void IVtoPID_SID_SEED::changeEvent(QEvent *event)
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

void IVtoPID_SID_SEED::setupModels()
{
    m->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("PID") << tr("Method") << tr("Ability") << "50%" << "12.5%" << "25%" << "75%" << tr("SID"));
}

void IVtoPID_SID_SEED::on_pushButtonFind_clicked()
{
    vector<u32> vals = ui->ivFilterGenerator->getValues();
    u32 hp = vals[0];
    u32 atk = vals[1];
    u32 def = vals[2];
    u32 spa = vals[3];
    u32 spd = vals[4];
    u32 spe = vals[5];

    m->removeRows(0, m->rowCount());

    u32 nature = Nature::getAdjustedNature(ui->comboBoxNatureGenerator->currentIndex());

    u32 tid = ui->textBoxID->text().toUInt(NULL, 10);

    u32 ivs2 = spe | (spa << 5) | (spd << 10);
    u32 ivs1 = hp | (atk << 5) | (def << 10);

    getSeeds(ivs1, ivs2, nature, tid);
    getSeeds(ivs1 ^ 0x8000, ivs2 ^ 0x8000, nature, tid);
}

void IVtoPID_SID_SEED::getSeeds(u32 ivs1, u32 ivs2, u32 nature, u32 tid)
{
    u32 x_test = ivs2 << 16;
    u32 x_testXD = ivs1 << 16;
    u32 pid = 0, sid = 0;
    bool pass = false;

    for (u32 cnt = 0; cnt <= 0xFFFF; cnt++)
    {
        u32 seedXD = x_testXD | cnt;
        LCRNG rngXD = XDRNG(seedXD);
        LCRNG rngXDR = XDRNGR(seedXD);
        u32 rng1XD = rngXD.nextUShort();

        // Gales/Colo
        if ((rng1XD & 0x7FFF) == (ivs2 & 0x7FFF))
        {
            u32 rng2XD = rngXD.nextUShort();
            u32 rng3XD = rngXD.nextUShort();
            u32 rng4XD = rngXD.nextUShort();
            u32 XDColoSeed = rngXDR.nextUInt();
            sid = (rng4XD ^ rng3XD ^ tid) & 0xFFF8;

            pid = (rng3XD << 16) | rng4XD;
            if (pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(XDColoSeed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(tr("Colosseum/XD"))
                        << new QStandardItem(pid & 1) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                        << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(sid);
                m->appendRow(newSeed);
            }
        }

        u32 seed = x_test | cnt;
        LCRNG rng = PokeRNGR(seed);
        u32 rng1 = rng.nextUShort();

        u32 rng2 = rng.nextUShort();
        u32 rng3 = rng.nextUShort();
        u32 rng4 = rng.nextUShort();

        u32 method1Seed = rng.seed;
        sid = (rng2 ^ rng3 ^ tid) & 0xFFF8;

        u32 method234Seed = rng.nextUInt();

        if ((rng1 & 0x7FFF) == (ivs1 & 0x7FFF))
        {
            u32 choppedPID;

            // Method 1
            pid = (rng2 << 16) | rng3;
            if (pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(tr("Method 1"))
                        << new QStandardItem(pid & 1) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                        << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(sid);
                m->appendRow(newSeed);
            }

            // Method 1 reverse pid
            pid = (rng3 << 16) | rng2;
            if (pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(tr("Reverse Method 1"))
                        << new QStandardItem(pid & 1) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                        << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(sid);
                m->appendRow(newSeed);
            }

            // Wishmkr
            if (pid % 25 == nature)
            {
                if (method1Seed < 0x10000)
                {
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(QString(((((pid & 0xFFFF) ^ (pid >> 16) ^ 20043) & 0xFFF8) == 0) ? tr("Wishmkr Shiny") : tr("Wishmkr")))
                            << new QStandardItem(pid & 1) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(sid);
                    m->appendRow(newSeed);
                }
            }

            // Method 2
            pid = (rng3 << 16) | rng4;
            sid = (rng3 ^ rng4 ^ tid) & 0xFFF8;
            if (pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method234Seed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(tr("Method 2"))
                        << new QStandardItem(pid & 1) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                        << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(sid);
                m->appendRow(newSeed);
            }

            // Cute Charm DPPt
            if (rng3 / 0x5556 != 0)
            {
                choppedPID = rng2 / 0xA3E;
                pass = choppedPID % 25 == nature;
                if (pass)
                {
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(tr("Cute Charm (DPPt)"))
                            << new QStandardItem(choppedPID & 1) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem((choppedPID ^ tid) & 0xFFF8);
                    m->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID += 0x96;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(tr("Cute Charm (DPPt)"))
                            << new QStandardItem(choppedPID & 1) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem((choppedPID ^ tid) & 0xFFF8);
                    m->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID += 0x32;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(tr("Cute Charm (DPPt)"))
                            << new QStandardItem(choppedPID & 1) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem((choppedPID ^ tid) & 0xFFF8);
                    m->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID -= 0x7D;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(tr("Cute Charm (DPPt)"))
                            << new QStandardItem(choppedPID & 1) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem((choppedPID ^ tid) & 0xFFF8);
                    m->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID -= 0x19;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(tr("Cute Charm (DPPt)"))
                            << new QStandardItem(choppedPID & 1) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem((choppedPID ^ tid) & 0xFFF8);
                    m->appendRow(newSeed);
                }

            }

            // Cute Charm HGSS
            if (rng3 % 3 != 0)
            {
                choppedPID = rng2 % 25;
                pass = choppedPID == nature;
                if (pass)
                {
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(tr("Cute Charm (HGSS)"))
                            << new QStandardItem(choppedPID & 1) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem((choppedPID ^ tid) & 0xFFF8);
                    m->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID += 0x96;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(tr("Cute Charm (HGSS)"))
                            << new QStandardItem(choppedPID & 1) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem((choppedPID ^ tid) & 0xFFF8);
                    m->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID += 0x32;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(tr("Cute Charm (HGSS)"))
                            << new QStandardItem(choppedPID & 1) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem((choppedPID ^ tid) & 0xFFF8);
                    m->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID -= 0x7D;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(tr("Cute Charm (HGSS)"))
                            << new QStandardItem(choppedPID & 1) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem((choppedPID ^ tid) & 0xFFF8);
                    m->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID -= 0x19;
                    QList<QStandardItem*> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16)) << new QStandardItem(QString::number(choppedPID, 16)) << new QStandardItem(tr("Cute Charm (HGSS)"))
                            << new QStandardItem(choppedPID & 1) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem((choppedPID ^ tid) & 0xFFF8);
                    m->appendRow(newSeed);
                }
            }
        }

        // Method 4
        if ((rng2 & 0x7FFF) == (ivs1 & 0x7FFF))
        {
            pid = (rng3 << 16) | rng4;
            sid = (rng3 ^ rng4 ^ tid) & 0xFFF8;
            if (pid % 25 == nature)
            {
                QList<QStandardItem*> newSeed;
                newSeed << new QStandardItem(QString::number(method234Seed, 16)) << new QStandardItem(QString::number(pid, 16)) << new QStandardItem(tr("Method 4"))
                        << new QStandardItem(pid & 1) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                        << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(sid);
                m->appendRow(newSeed);
            }
        }
    }

    ui->tableView->viewport()->update();
}
