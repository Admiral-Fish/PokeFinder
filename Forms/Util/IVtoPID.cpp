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

#include "IVtoPID.hpp"
#include "ui_IVtoPID.h"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/RNGEuclidean.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <QSettings>

IVtoPID::IVtoPID(QWidget *parent) : QWidget(parent), ui(new Ui::IVtoPID)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

IVtoPID::~IVtoPID()
{
    QSettings setting;
    setting.setValue("ivToPID/geometry", this->saveGeometry());

    delete ui;
}

void IVtoPID::setupModels()
{
    model = new QStandardItemModel(ui->tableView);
    model->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("PID") << tr("Method") << tr("Ability") << "50%"
                                                   << "12.5%"
                                                   << "25%"
                                                   << "75%" << tr("SID"));
    ui->tableView->setModel(model);

    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->comboBoxNature->addItems(Translator::getNatures());

    connect(ui->pushButtonFind, &QPushButton::clicked, this, &IVtoPID::find);

    QSettings setting;
    if (setting.contains("ivToPID/geometry"))
    {
        this->restoreGeometry(setting.value("ivToPID/geometry").toByteArray());
    }
}

QVector<QList<QStandardItem *>> IVtoPID::getSeeds(u16 ivs1, u16 ivs2, u8 nature, u16 tid)
{
    QVector<QList<QStandardItem *>> results;

    u32 x_test = ivs2 << 16;
    u32 x_testXD = ivs1 << 16;
    u32 pid;
    u16 sid;
    bool pass;

    for (u32 cnt = 0; cnt <= 0xFFFF; cnt++)
    {
        u32 seedXD = x_testXD | cnt;
        XDRNG rngXD(seedXD);
        XDRNGR rngXDR(seedXD);
        u16 rng1XD = rngXD.nextUShort();

        // Gales/Colo
        if ((rng1XD & 0x7FFF) == (ivs2 & 0x7FFF))
        {
            rngXD.nextUShort();
            u16 rng3XD = rngXD.nextUShort();
            u16 rng4XD = rngXD.nextUShort();
            u32 XDColoSeed = rngXDR.nextUInt();
            sid = (rng4XD ^ rng3XD ^ tid) & 0xFFF8;

            pid = (rng3XD << 16) | rng4XD;
            if (pid % 25 == nature)
            {
                QList<QStandardItem *> result;
                result << new QStandardItem(QString::number(XDColoSeed, 16).toUpper())
                       << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("XD/Colo"))
                       << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F")
                       << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F")
                       << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                results.append(result);
            }
        }

        u32 seed = x_test | cnt;
        PokeRNGR rng(seed);
        u16 rng1 = rng.nextUShort();

        u16 rng2 = rng.nextUShort();
        u16 rng3 = rng.nextUShort();
        u16 rng4 = rng.nextUShort();

        u32 method1Seed = rng.getSeed();
        sid = (rng2 ^ rng3 ^ tid) & 0xFFF8;

        u32 method234Seed = rng.nextUInt();

        if ((rng1 & 0x7FFF) == (ivs1 & 0x7FFF))
        {
            // Method 1
            pid = (rng2 << 16) | rng3;
            if (pid % 25 == nature)
            {
                QList<QStandardItem *> result;
                result << new QStandardItem(QString::number(method1Seed, 16).toUpper())
                       << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("Method 1"))
                       << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F")
                       << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F")
                       << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                results.append(result);
            }

            // Method 1 reverse pid
            pid = (rng3 << 16) | rng2;
            if (pid % 25 == nature)
            {
                QList<QStandardItem *> result;
                result << new QStandardItem(QString::number(method1Seed, 16).toUpper())
                       << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("Reverse Method 1"))
                       << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F")
                       << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F")
                       << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                results.append(result);
            }

            // Method 2
            pid = (rng3 << 16) | rng4;
            if (pid % 25 == nature)
            {
                sid = (rng3 ^ rng4 ^ tid) & 0xFFF8;
                QList<QStandardItem *> result;
                result << new QStandardItem(QString::number(method234Seed, 16).toUpper())
                       << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("Method 2"))
                       << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F")
                       << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F")
                       << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                results.append(result);
            }

            // Cute Charm DPPt
            if (rng3 / 0x5556 != 0)
            {
                u8 choppedPID = rng2 / 0xA3E;
                pass = choppedPID % 25 == nature;

                if (pass)
                {
                    for (auto i : { 0, 0x96, 0x32, -0x7d, -0x19 })
                    {
                        u8 cPID = choppedPID + i;
                        sid = (cPID ^ tid) & 0xFFF8;
                        QList<QStandardItem *> result;
                        result << new QStandardItem(QString::number(method1Seed, 16).toUpper())
                               << new QStandardItem(QString::number(cPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (DPPt)"))
                               << new QStandardItem(QString::number(cPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F")
                               << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                               << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F")
                               << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                        results.append(result);
                    }
                }
            }

            // Cute Charm HGSS
            if (rng3 % 3 != 0)
            {
                u8 choppedPID = rng2 % 25;
                pass = choppedPID == nature;

                if (pass)
                {
                    for (auto i : { 0, 0x96, 0x32, -0x7d, -0x19 })
                    {
                        u8 cPID = choppedPID + i;
                        sid = (cPID ^ tid) & 0xFFF8;
                        QList<QStandardItem *> result;
                        result << new QStandardItem(QString::number(method1Seed, 16).toUpper())
                               << new QStandardItem(QString::number(cPID, 16).toUpper()) << new QStandardItem(tr("Cute Charm (DPPt)"))
                               << new QStandardItem(QString::number(cPID & 1)) << new QStandardItem(((choppedPID & 0xFF) > 126) ? "M" : "F")
                               << new QStandardItem(((choppedPID & 0xFF) > 30) ? "M" : "F")
                               << new QStandardItem(((choppedPID & 0xFF) > 63) ? "M" : "F")
                               << new QStandardItem(((choppedPID & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                        results.append(result);
                    }
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
                QList<QStandardItem *> result;
                result << new QStandardItem(QString::number(method234Seed, 16).toUpper())
                       << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("Method 4"))
                       << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F")
                       << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F")
                       << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
                results.append(result);
            }
        }
    }

    return results;
}

QVector<QList<QStandardItem *>> IVtoPID::getSeedsChannel(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature)
{
    QVector<QList<QStandardItem *>> results;

    RNGEuclidean euclidean(Method::Channel);
    QVector<u32> seeds = euclidean.recoverLower27BitsChannel(hp, atk, def, spa, spd, spe);

    for (const auto &seed : seeds)
    {
        XDRNGR rng(seed);
        rng.advanceFrames(3);

        u16 pid2 = rng.nextUShort();
        u16 pid1 = rng.nextUShort();
        u16 sid = rng.nextUShort();

        // Determine if PID needs to be XORed
        if ((pid2 > 7 ? 0 : 1) != (pid1 ^ sid ^ 40122))
        {
            pid1 ^= 0x8000;
        }

        u32 pid = (pid1 << 16) | pid2;

        if (pid % 25 == nature)
        {
            QList<QStandardItem *> result;
            result << new QStandardItem(QString::number(rng.nextUInt(), 16).toUpper())
                   << new QStandardItem(QString::number(pid, 16).toUpper()) << new QStandardItem(tr("Channel"))
                   << new QStandardItem(QString::number(pid & 1)) << new QStandardItem(((pid & 0xFF) > 126) ? "M" : "F")
                   << new QStandardItem(((pid & 0xFF) > 30) ? "M" : "F") << new QStandardItem(((pid & 0xFF) > 63) ? "M" : "F")
                   << new QStandardItem(((pid & 0xFF) > 190) ? "M" : "F") << new QStandardItem(QString::number(sid));
            results.append(result);
        }
    }

    return results;
}

void IVtoPID::find()
{
    model->removeRows(0, model->rowCount());

    u8 hp = ui->spinBoxHP->value();
    u8 atk = ui->spinBoxAtk->value();
    u8 def = ui->spinBoxDef->value();
    u8 spa = ui->spinBoxSpA->value();
    u8 spd = ui->spinBoxSpD->value();
    u8 spe = ui->spinBoxSpe->value();

    u8 nature = static_cast<u8>(ui->comboBoxNature->currentIndex());

    u16 tid = ui->textBoxTID->text().toUShort();

    u32 ivs2 = spe | (spa << 5) | (spd << 10);
    u32 ivs1 = hp | (atk << 5) | (def << 10);

    auto seeds = getSeeds(ivs1, ivs2, nature, tid);
    seeds.append(getSeeds(ivs1 ^ 0x8000, ivs2 ^ 0x8000, nature, tid));
    seeds.append(getSeedsChannel(hp, atk, def, spa, spd, spe, nature));

    for (const auto &seed : seeds)
    {
        model->appendRow(seed);
    }
}
