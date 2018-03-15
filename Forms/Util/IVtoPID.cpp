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

#include "IVtoPID.hpp"
#include "ui_IVtoPID.h"

IVtoPID::IVtoPID(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IVtoPID)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

IVtoPID::~IVtoPID()
{
    delete ui;
    delete model;
}

void IVtoPID::changeEvent(QEvent *event)
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

void IVtoPID::setupModels()
{
    ui->textBoxID->setValues(0, 48, true);

    model->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("PID") << tr("Method") << tr("Ability") << "50%" << "12.5%" << "25%" << "75%" << tr("SID"));
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, 106);
    ui->tableView->setColumnWidth(1, 106);
    ui->tableView->setColumnWidth(2, 150);
    ui->tableView->setColumnWidth(3, 50);
    ui->tableView->setColumnWidth(4, 50);
    ui->tableView->setColumnWidth(5, 50);
    ui->tableView->setColumnWidth(6, 50);
    ui->tableView->setColumnWidth(7, 50);
    ui->tableView->setColumnWidth(8, 70);
}

void IVtoPID::on_pushButtonFind_clicked()
{
    u32 hp = ui->spinBoxHP->value();
    u32 atk = ui->spinBoxAtk->value();
    u32 def = ui->spinBoxDef->value();
    u32 spa = ui->spinBoxSpA->value();
    u32 spd = ui->spinBoxSpD->value();
    u32 spe = ui->spinBoxSpe->value();

    model->removeRows(0, model->rowCount());

    u32 nature = Nature::getAdjustedNature(ui->comboBoxNatureGenerator->currentIndex());

    u32 tid = ui->textBoxID->text().toUInt(NULL, 10);

    u32 ivs2 = spe | (spa << 5) | (spd << 10);
    u32 ivs1 = hp | (atk << 5) | (def << 10);

    getSeeds(ivs1, ivs2, nature, tid);
    getSeeds(ivs1 ^ 0x8000, ivs2 ^ 0x8000, nature, tid);
    getSeedsChannel(hp, atk, def, spa, spd, spe, nature);
}

void IVtoPID::getSeeds(u32 ivs1, u32 ivs2, u32 nature, u32 tid)
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
            rngXD.nextUShort();
            u32 rng3XD = rngXD.nextUShort();
            u32 rng4XD = rngXD.nextUShort();
            u32 XDColoSeed = rngXDR.nextUInt();
            sid = (rng4XD ^ rng3XD ^ tid) & 0xFFF8;

            pid = (rng3XD << 16) | rng4XD;
            if (pid % 25 == nature)
            {
                QList<QStandardItem *> newSeed;
                newSeed << new QStandardItem(QString::number(XDColoSeed, 16).toUpper()) << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("XD/Colo"))
                        << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                        << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                model->appendRow(newSeed);
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
                QList<QStandardItem *> newSeed;
                newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("Method 1"))
                        << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                        << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                model->appendRow(newSeed);
            }

            // Method 1 reverse pid
            pid = (rng3 << 16) | rng2;
            if (pid % 25 == nature)
            {
                QList<QStandardItem *> newSeed;
                newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("Reverse Method 1"))
                        << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                        << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                model->appendRow(newSeed);
            }

            // Method 2
            pid = (rng3 << 16) | rng4;
            if (pid % 25 == nature)
            {
                sid = (rng3 ^ rng4 ^ tid) & 0xFFF8;
                QList<QStandardItem *> newSeed;
                newSeed << new QStandardItem(QString::number(method234Seed, 16).toUpper()) << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("Method 2"))
                        << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                        << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                model->appendRow(newSeed);
            }

            // Cute Charm DPPt
            if (rng3 / 0x5556 != 0)
            {
                choppedPID = rng2 / 0xA3E;
                sid = (choppedPID ^ tid) & 0xFFF8;
                pass = choppedPID % 25 == nature;
                if (pass)
                {
                    QList<QStandardItem *> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(choppedPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (DPPt)"))
                            << new QStandardItem(QString::number(choppedPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                    model->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID += 0x96;
                    sid = (choppedPID ^ tid) & 0xFFF8;
                    QList<QStandardItem *> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(choppedPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (DPPt)"))
                            << new QStandardItem(QString::number(choppedPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                    model->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID += 0x32;
                    sid = (choppedPID ^ tid) & 0xFFF8;
                    QList<QStandardItem *> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(choppedPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (DPPt)"))
                            << new QStandardItem(QString::number(choppedPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                    model->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID -= 0x7D;
                    sid = (choppedPID ^ tid) & 0xFFF8;
                    QList<QStandardItem *> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(choppedPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (DPPt)"))
                            << new QStandardItem(QString::number(choppedPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                    model->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID -= 0x19;
                    sid = (choppedPID ^ tid) & 0xFFF8;
                    QList<QStandardItem *> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(choppedPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (DPPt)"))
                            << new QStandardItem(QString::number(choppedPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                    model->appendRow(newSeed);
                }

            }

            // Cute Charm HGSS
            if (rng3 % 3 != 0)
            {
                choppedPID = rng2 % 25;
                sid = (choppedPID ^ tid) & 0xFFF8;
                pass = choppedPID == nature;
                if (pass)
                {
                    QList<QStandardItem *> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(choppedPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (HGSS)"))
                            << new QStandardItem(QString::number(choppedPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                    model->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID += 0x96;
                    sid = (choppedPID ^ tid) & 0xFFF8;
                    QList<QStandardItem *> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(choppedPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (HGSS)"))
                            << new QStandardItem(QString::number(choppedPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                    model->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID += 0x32;
                    sid = (choppedPID ^ tid) & 0xFFF8;
                    QList<QStandardItem *> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(choppedPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (HGSS)"))
                            << new QStandardItem(QString::number(choppedPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                    model->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID -= 0x7D;
                    sid = (choppedPID ^ tid) & 0xFFF8;
                    QList<QStandardItem *> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(choppedPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (HGSS)"))
                            << new QStandardItem(QString::number(choppedPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                    model->appendRow(newSeed);
                }

                if (pass)
                {
                    choppedPID -= 0x19;
                    sid = (choppedPID ^ tid) & 0xFFF8;
                    QList<QStandardItem *> newSeed;
                    newSeed << new QStandardItem(QString::number(method1Seed, 16).toUpper()) << new QStandardItem(QString::number(choppedPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (HGSS)"))
                            << new QStandardItem(QString::number(choppedPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                            << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                    model->appendRow(newSeed);
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
                QList<QStandardItem *> newSeed;
                newSeed << new QStandardItem(QString::number(method234Seed, 16).toUpper()) << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("Method 4"))
                        << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                        << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                model->appendRow(newSeed);
            }
        }
    }
}

void IVtoPID::getSeedsChannel(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe, u32 nature)
{
    RNGEuclidean euclidean = RNGEuclidean(Channel);
    LCRNG rng = XDRNGR(0);

    vector<u32> seeds = euclidean.recoverLower27BitsChannel(hp, atk, def, spa, spd, spe);

    for (auto i = 0; i < seeds.size(); i++)
    {
        rng.seed = seeds[i];
        rng.advanceFrames(3);

        u32 pid2 = rng.nextUShort();
        u32 pid1 = rng.nextUShort();
        u32 sid = rng.nextUShort();

        // Determine if PID needs to be XORed
        if ((pid2 > 7 ? 0 : 1) != (pid1 ^ sid ^ 40122))
            pid1 ^= 0x8000;

        u32 pid = (pid1 << 16) | pid2;

        if (pid % 25 == nature)
        {
            QList<QStandardItem *> newSeed;
            newSeed << new QStandardItem(QString::number(rng.nextUInt(), 16).toUpper()) << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("Channel"))
                    << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F")
                    << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
            model->appendRow(newSeed);
        }
    }
}
