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

#include "PIDtoIVs.hpp"
#include "ui_PIDtoIVs.h"

PIDtoIVs::PIDtoIVs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PIDtoIVs)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();

}

PIDtoIVs::~PIDtoIVs()
{
    delete ui;
    delete model;
    delete contextMenu;
}

void PIDtoIVs::setupModels()
{
    ui->pidInput->setValues(InputType::Seed32Bit);

    model->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("Method") << tr("IVs"));
    ui->tabePIDToIV->setModel(model);
    ui->tabePIDToIV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QAction *copySeed = new QAction("Copy Seed to Clipboard", this);
    QAction *moveResults = new QAction("Move Result to Stationary Generator", this);
    QAction *moveIVs = new QAction("Move IVs to Stationary Generator", this);

    connect(copySeed, &QAction::triggered, this, &PIDtoIVs::copySeed);
    connect(moveResults, &QAction::triggered, this, [ = ]
    {
        QStringList ivs = ui->tabePIDToIV->model()->data(ui->tabePIDToIV->model()->index(lastIndex.row(), 2)).toString().split(".");
        emit moveResultsToStationary(ui->tabePIDToIV->model()->data(ui->tabePIDToIV->model()->index(lastIndex.row(), 0)).toString(), ui->tabePIDToIV->model()->data(ui->tabePIDToIV->model()->index(lastIndex.row(), 1)).toString(),
                                     ivs.at(0).toUShort(), ivs.at(1).toUShort(), ivs.at(2).toUShort(), ivs.at(3).toUShort(), ivs.at(4).toUShort(), ivs.at(5).toUShort());
    });
    connect(moveIVs, &QAction::triggered, this, [ = ]
    {
        QStringList ivs = ui->tabePIDToIV->model()->data(ui->tabePIDToIV->model()->index(lastIndex.row(), 2)).toString().split(".");
        emit moveResultsToStationary("", "", ivs.at(0).toUShort(), ivs.at(1).toUShort(), ivs.at(2).toUShort(), ivs.at(3).toUShort(), ivs.at(4).toUShort(), ivs.at(5).toUShort());
    });

    contextMenu->addAction(copySeed);
    contextMenu->addAction(moveResults);
    contextMenu->addAction(moveIVs);
}

void PIDtoIVs::calcFromPID(u32 pid)
{
    calcMethod124(pid);
    calcMethodXD(pid);
    calcMethodChannel(pid);
}

void PIDtoIVs::calcMethod124(u32 pid)
{
    RNGCache cache(Method1);

    u32 pidl = (pid & 0xFFFF) << 16;
    u32 pidh = pid & 0xFFFF0000;

    QVector<u32> seeds = cache.recoverLower16BitsPID(pidl, pidh);
    for (const auto &seed : seeds)
    {
        PokeRNG forward(seed, 1);
        PokeRNGR backward(seed);
        addSeed(backward.nextUInt(), forward.nextUInt());
    }
}

void PIDtoIVs::calcMethodXD(u32 pid)
{
    RNGEuclidean euclidean(XDColo);

    QVector<QPair<u32, u32>> seeds = euclidean.recoverLower16BitsPID(pid & 0xFFFF0000, (pid & 0xFFFF) << 16);
    for (const auto &pair : seeds)
    {
        XDRNGR backward(pair.first, 1);
        u16 iv2 = backward.nextUShort();
        u16 iv1 = backward.nextUShort();
        addSeedGC(backward.nextUInt(), iv1, iv2);
    }
}

void PIDtoIVs::calcMethodChannel(u32 pid)
{
    RNGEuclidean euclidean(XDColo);

    u32 pid1 = pid >> 16;
    u32 pid2 = pid & 0xFFFF;

    // Whether PID is xored or unxored is determined by SID which we don't know by only providing a PID
    // So we have to check both xored and unxored and recalculate the PID to see if we have a match

    QVector<QPair<u32, u32>> seeds = euclidean.recoverLower16BitsPID(pid1 << 16, pid2 << 16);
    for (const auto &pair : seeds)
    {
        XDRNGR backward(pair.first);
        u16 sid = backward.nextUShort();
        u32 seed = backward.nextUInt();

        XDRNG forward(seed, 1);
        u16 high = forward.nextUShort();
        u16 low = forward.nextUShort();

        if ((low > 7 ? 0 : 1) != (high ^ 40122 ^ sid))
        {
            high ^= 0x8000;
        }

        u32 val = (high << 16) | low;
        if (val == pid) // PID matches based on SID
        {
            forward.advanceFrames(3);
            addSeedChannel(seed, forward.nextUInt());
        }
    }

    QVector<QPair<u32, u32>> seedsXOR = euclidean.recoverLower16BitsPID((pid1 ^ 0x8000) << 16, pid2 << 16);
    for (const auto &pair : seedsXOR)
    {
        XDRNGR backward(pair.first);
        u16 sid = backward.nextUShort();
        u32 seed = backward.nextUInt();

        XDRNG forward(seed, 1);
        u16 high = forward.nextUShort();
        u16 low = forward.nextUShort();

        if ((low > 7 ? 0 : 1) != (high ^ 40122 ^ sid))
        {
            high ^= 0x8000;
        }

        u32 val = (high << 16) | low;
        if (val == pid) // PID matches based on SID
        {
            forward.advanceFrames(3);
            addSeedChannel(seed, forward.nextUInt());
        }
    }
}

QString PIDtoIVs::calcIVs(u32 iv1, int num)
{
    QString ivs = "";
    PokeRNG rng(iv1);
    u16 iv2;

    if (num == 1)
    {
        iv2 = rng.nextUShort();
        iv1 >>= 16;
    }
    else if (num == 2)
    {
        iv1 = rng.nextUShort();
        iv2 = rng.nextUShort();
    }
    else // method == 4
    {
        rng.nextUInt();
        iv2 = rng.nextUShort();
        iv1 >>= 16;
    }

    for (u32 x = 0; x < 3; x++)
    {
        ivs += QString::number((iv1 >> (x * 5)) & 31);
        ivs += ".";
    }

    for (u32 x : { 1u, 2u, 0u })
    {
        ivs += QString::number((iv2 >> (x * 5)) & 31);
        if (x != 0)
        {
            ivs += ".";
        }
    }

    return ivs;
}

QString PIDtoIVs::calcIVsXD(u16 iv1, u16 iv2)
{
    QString ivs = "";

    for (u32 x = 0; x < 3; x++)
    {
        ivs += QString::number((iv1 >> (x * 5)) & 31);
        ivs += ".";
    }

    for (u32 x : { 1u, 2u, 0u })
    {
        ivs += QString::number((iv2 >> (x * 5)) & 31);
        if (x != 0)
        {
            ivs += ".";
        }
    }

    return ivs;
}

QString PIDtoIVs::calcIVsChannel(u32 iv1)
{
    QString ivs = "";
    XDRNG rng(iv1);

    u32 val[6] = { iv1 >> 27, 0, 0, 0, 0, 0 };
    for (int x = 1; x < 6; x++)
    {
        val[x] = rng.nextUInt() >> 27;
    }

    QVector<int> order = { 0, 1, 2, 4, 5, 3};
    for (const int &x : order)
    {
        ivs += QString::number(x);
        if (x != 3)
        {
            ivs += ".";
        }
    }

    return ivs;
}

void PIDtoIVs::addSeed(u32 seed, u32 iv1)
{
    model->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper()) << new QStandardItem(tr("Method 1")) << new QStandardItem(calcIVs(iv1, 1)));
    model->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper()) << new QStandardItem(tr("Method 2")) << new QStandardItem(calcIVs(iv1, 2)));
    model->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper()) << new QStandardItem(tr("Method 4")) << new QStandardItem(calcIVs(iv1, 4)));
}

void PIDtoIVs::addSeedGC(u32 seed, u16 iv1, u16 iv2)
{
    model->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper()) << new QStandardItem(tr("XD/Colo")) << new QStandardItem(calcIVsXD(iv1, iv2)));
}

void PIDtoIVs::addSeedChannel(u32 seed, u32 iv1)
{
    model->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper()) << new QStandardItem(tr("Channel")) << new QStandardItem(calcIVsChannel(iv1)));
}

void PIDtoIVs::on_pushButtonGenerate_clicked()
{
    model->removeRows(0, model->rowCount());
    u32 pid = ui->pidInput->text().toUInt(nullptr, 16);
    calcFromPID(pid);
}

void PIDtoIVs::on_tabePIDToIV_customContextMenuRequested(const QPoint &pos)
{
    if (model->rowCount() == 0)
    {
        return;
    }

    lastIndex = ui->tabePIDToIV->indexAt(pos);
    contextMenu->popup(ui->tabePIDToIV->viewport()->mapToGlobal(pos));
}

void PIDtoIVs::copySeed()
{
    QApplication::clipboard()->setText(ui->tabePIDToIV->model()->data(ui->tabePIDToIV->model()->index(lastIndex.row(), 0)).toString());
}
