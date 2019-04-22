/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <QClipboard>
#include "PIDtoIVs.hpp"
#include "ui_PIDtoIVs.h"
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <Core/RNG/RNGEuclidean.hpp>

PIDtoIVs::PIDtoIVs(QWidget *parent) :
    QWidget(parent),
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
}

void PIDtoIVs::setupModels()
{
    model = new QStandardItemModel(ui->tableView);
    contextMenu = new QMenu(ui->tableView);

    ui->textBoxPID->setValues(InputType::Seed32Bit);

    model->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("Method") << tr("IVs"));
    ui->tableView->setModel(model);

    QAction *copySeed = contextMenu->addAction(tr("Copy Seed to Clipboard"));
    QAction *moveResults = contextMenu->addAction(tr("Move Result to Stationary Generator"));
    QAction *moveIVs = contextMenu->addAction(tr("Move IVs to Stationary Generator"));

    auto data = [ = ](int column) { return ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), column)); };
    connect(copySeed, &QAction::triggered, this, &PIDtoIVs::copySeed);
    connect(moveResults, &QAction::triggered, this, [ = ]
    {
        QStringList ivs = data(2).toString().split(".");
        emit moveResultsToStationary(data(0).toString(), data(1).toString(), ivs.at(0).toUShort(), ivs.at(1).toUShort(),
                                     ivs.at(2).toUShort(), ivs.at(3).toUShort(), ivs.at(4).toUShort(), ivs.at(5).toUShort());
    });
    connect(moveIVs, &QAction::triggered, this, [ = ]
    {
        QStringList ivs = data(2).toString().split(".");
        emit moveResultsToStationary("", "", ivs.at(0).toUShort(), ivs.at(1).toUShort(), ivs.at(2).toUShort(),
                                     ivs.at(3).toUShort(), ivs.at(4).toUShort(), ivs.at(5).toUShort());
    });
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
    QStringList ivs;
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
        ivs.append(QString::number((iv1 >> (x * 5)) & 31));
    }

    for (u32 x : { 1u, 2u, 0u })
    {
        ivs.append(QString::number((iv2 >> (x * 5)) & 31));
    }

    return ivs.join('.');
}

QString PIDtoIVs::calcIVsXD(u16 iv1, u16 iv2)
{
    QStringList ivs;

    for (u32 x = 0; x < 3; x++)
    {
        ivs.append(QString::number((iv1 >> (x * 5)) & 31));
    }

    for (u32 x : { 1u, 2u, 0u })
    {
        ivs.append(QString::number((iv2 >> (x * 5)) & 31));
    }

    return ivs.join('.');
}

QString PIDtoIVs::calcIVsChannel(u32 iv1)
{
    QStringList ivs;
    XDRNG rng(iv1);

    QVector<u32> val = { iv1 >> 27, 0, 0, 0, 0, 0 };
    for (u8 x : { 1, 2, 4, 5, 3 })
    {
        val[x] = rng.nextUInt() >> 27;
    }

    for (u32 x : val)
    {
        ivs.append(QString::number(x));
    }

    return ivs.join('.');
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
    u32 pid = ui->textBoxPID->getUInt();
    calcFromPID(pid);
}

void PIDtoIVs::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    if (model->rowCount() == 0)
    {
        return;
    }

    contextMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void PIDtoIVs::copySeed()
{
    QVariant data = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0));
    QApplication::clipboard()->setText(data.toString());
}
