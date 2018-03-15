/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

void PIDtoIVs::changeEvent(QEvent *event)
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

void PIDtoIVs::setupModels()
{
    ui->pidInput->setValues(0, 32, false);

    model->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("Method") << tr("IVs"));
    ui->tabePIDToIV->setModel(model);
    ui->tabePIDToIV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QAction *copySeed = new QAction("Copy Seed to Clipboard", this);
    QAction *moveResults = new QAction("Move Result to Stationary Generator", this);
    QAction *moveIVs = new QAction("Move IVs to Stationary Generator", this);

    connect(copySeed, &QAction::triggered, this, &PIDtoIVs::copySeed);
    connect(moveResults, &QAction::triggered, this, [=]{
        QStringList ivs = ui->tabePIDToIV->model()->data(ui->tabePIDToIV->model()->index(lastIndex.row(), 2)).toString().split(".");
        emit moveResultsToStationary(ui->tabePIDToIV->model()->data(ui->tabePIDToIV->model()->index(lastIndex.row(), 0)).toString(), ui->tabePIDToIV->model()->data(ui->tabePIDToIV->model()->index(lastIndex.row(), 1)).toString(), ((QString)ivs[0]).toUInt(), ((QString)ivs[1]).toUInt(), ((QString)ivs[2]).toUInt(), ((QString)ivs[3]).toUInt(), ((QString)ivs[4]).toUInt(), ((QString)ivs[5]).toUInt());
    });
    connect(moveIVs, &QAction::triggered, this, [=]{
        QStringList ivs = ui->tabePIDToIV->model()->data(ui->tabePIDToIV->model()->index(lastIndex.row(), 2)).toString().split(".");
        emit moveResultsToStationary("", "", ((QString)ivs[0]).toUInt(), ((QString)ivs[1]).toUInt(), ((QString)ivs[2]).toUInt(), ((QString)ivs[3]).toUInt(), ((QString)ivs[4]).toUInt(), ((QString)ivs[5]).toUInt());
    });

    contextMenu->addAction(copySeed);
    contextMenu->addAction(moveResults);
    contextMenu->addAction(moveIVs);

}

void PIDtoIVs::calcFromPID(u32 pid)
{
    calcMethod124(pid);
    calcMethodXD(pid);
}

void PIDtoIVs::calcMethod124(u32 pid)
{
    RNGCache cache = RNGCache(Method1);
    LCRNG forward = PokeRNG(0);
    LCRNG backward = PokeRNGR(0);

    u32 pidl = (pid & 0xFFFF) << 16;
    u32 pidh = pid & 0xFFFF0000;

    vector<u32> seeds = cache.recoverLower16BitsPID(pidl, pidh);
    for(int i = 0; i < seeds.size(); i++)
    {
        forward.seed = backward.seed = seeds[i];
        forward.nextUInt();
        addSeed(backward.nextUInt(), forward.nextUInt());
    }
}

void PIDtoIVs::calcMethodXD(u32 pid)
{
    RNGEuclidean euclidean = RNGEuclidean(XDColo);
    LCRNG rng = XDRNGR(0);

    vector<u32> seeds = euclidean.recoverLower16BitsPID(pid & 0xFFFF0000, (pid & 0xFFFF) << 16);
    for (auto i = 0; i < seeds.size(); i += 2)
    {
        rng.seed = seeds[i];
        rng.nextUInt();
        u32 iv2 = rng.nextUShort();
        u32 iv1 = rng.nextUShort();
        addSeedGC(rng.nextUInt(), iv1, iv2);
    }
}

void PIDtoIVs::addSeed(u32 seed, u32 iv1)
{
    QString monsterSeed = QString::number(seed, 16);
    model->appendRow(QList<QStandardItem *>() << new QStandardItem(monsterSeed.toUpper()) << new QStandardItem(tr("Method 1")) << new QStandardItem(calcIVs1(iv1)));
    model->appendRow(QList<QStandardItem *>() << new QStandardItem(monsterSeed.toUpper()) << new QStandardItem(tr("Method 2")) << new QStandardItem(calcIVs2(iv1)));
    model->appendRow(QList<QStandardItem *>() << new QStandardItem(monsterSeed.toUpper()) << new QStandardItem(tr("Method 4")) << new QStandardItem(calcIVs4(iv1)));
}

void PIDtoIVs::addSeedGC(u32 seed, u32 iv1, u32 iv2)
{
    model->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper())<< new QStandardItem(tr("XD/Colo")) << new QStandardItem(calcIVsXD(iv1, iv2)));
}

QString PIDtoIVs::calcIVs1(u32 iv1)
{
    QString ivs = "";
    LCRNG rng = PokeRNG(iv1);
    u32 iv2 = rng.nextUShort();
    iv1 >>= 16;

    for(u32 x = 0; x < 3; x++)
    {
        u32 q = x * 5;
        u32 iv = (iv1 >> q) & 31;
        ivs += QString::number(iv);
        ivs += ".";
    }

    u32 val = (iv2 >> 5) & 31;
    ivs += QString::number(val);
    ivs += ".";

    val = (iv2 >> 10) & 31;
    ivs += QString::number(val);
    ivs += ".";

    val = iv2 & 31;
    ivs += QString::number(val);

    return ivs;
}

QString PIDtoIVs::calcIVs2(u32 iv1)
{
    QString ivs = "";
    LCRNG rng = PokeRNG(iv1);
    iv1 = rng.nextUInt();
    u32 iv2 = rng.nextUShort();
    iv1 >>= 16;

    for (u32 x = 0; x < 3; x++)
    {
        u32 q = x * 5;
        uint iv = (iv1 >> q) & 31;
        ivs += QString::number(iv);
        ivs += ".";
    }

    u32 val = (iv2 >> 5) & 31;
    ivs += QString::number(val);
    ivs += ".";

    val = (iv2 >> 10) & 31;
    ivs += QString::number(val);
    ivs += ".";

    val = iv2 & 31;
    ivs += QString::number(val);

    return ivs;
}

QString PIDtoIVs::calcIVs4(u32 iv1)
{
    QString ivs = "";
    LCRNG rng = PokeRNG(iv1);
    rng.nextUInt();
    u32 iv2 = rng.nextUShort();
    iv1 >>= 16;

    for (u32 x = 0; x < 3; x++)
    {
        u32 q = x * 5;
        u32 iv = (iv1 >> q) & 31;
        ivs += QString::number(iv);
        ivs += ".";
    }

    u32 val = (iv2 >> 5) & 31;
    ivs += QString::number(val);
    ivs += ".";

    val = (iv2 >> 10) & 31;
    ivs += QString::number(val);
    ivs += ".";

    val = iv2 & 31;
    ivs += QString::number(val);

    return ivs;
}

QString PIDtoIVs::calcIVsXD(u32 iv1, u32 iv2)
{
    QString ivs = "";

    for (u32 x = 0; x < 3; x++)
    {
        u32 q = x * 5;
        u32 iv = (iv1 >> q) & 31;
        ivs += QString::number(iv);
        ivs += ".";
    }

    u32 val = (iv2 >> 5) & 31;
    ivs += QString::number(val);
    ivs += ".";

    val = (iv2 >> 10) & 31;
    ivs += QString::number(val);
    ivs += ".";

    val = iv2 & 31;
    ivs += QString::number(val);

    return ivs;
}

void PIDtoIVs::on_pushButtonGenerate_clicked()
{
    model->removeRows(0, model->rowCount());
    u32 pid = ui->pidInput->text().toUInt(NULL, 16);
    calcFromPID(pid);
}

void PIDtoIVs::copySeed()
{
    QApplication::clipboard()->setText(ui->tabePIDToIV->model()->data(ui->tabePIDToIV->model()->index(lastIndex.row(), 0)).toString());
}

void PIDtoIVs::on_tabePIDToIV_customContextMenuRequested(const QPoint &pos)
{
    if (model->rowCount() == 0)
        return;

    lastIndex = ui->tabePIDToIV->indexAt(pos);

    contextMenu->popup(ui->tabePIDToIV->viewport()->mapToGlobal(pos));
}
