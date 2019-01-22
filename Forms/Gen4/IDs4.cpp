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

#include "IDs4.hpp"
#include "ui_IDs4.h"

IDs4::IDs4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IDs4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
}

IDs4::~IDs4()
{
    delete ui;
    delete model;
}

void IDs4::setupModels()
{
    ui->tableView->setModel(model);
    model->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("TID") << tr("SID") << tr("Delay") << tr("Seconds"));
}

void IDs4::updateView(QVector<QList<QStandardItem *>> frames, int progress)
{
    for (const auto &item : frames)
    {
        model->appendRow(item);
    }
    ui->progressBar->setValue(progress);
}

void IDs4::on_pushButtonShinyPIDSearch_clicked()
{
    if (!ui->pushButtonTIDSIDSearch->isEnabled())
    {
        return;
    }

    model->removeRows(0, model->rowCount());

    ui->pushButtonShinyPIDSearch->setEnabled(false);
    ui->pushButtonShinyPIDCancel->setEnabled(true);

    u32 pid = ui->textBoxShinyPIDPID->text().toUInt(nullptr, 16);
    bool useTID = ui->checkBoxShinyPIDSearchTID->isChecked();
    u16 tid = ui->textBoxShinyPIDTID->text().toUShort();
    u32 year = ui->textBoxShinyPIDYear->text().toUInt();
    u32 minDelay = ui->textBoxShinyPIDMinDelay->text().toUInt();
    u32 maxDelay = ui->textBoxShinyPIDMaxDelay->text().toUInt();
    bool infinite = ui->checkBoxShinyPIDInfiniteSearch->isChecked();
    minDelay += (year - 2000);
    maxDelay += (year - 2000);

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(static_cast<int>(256 * 24 * (infinite ? 0xE8FFFF : (maxDelay - minDelay + 1))));

    auto *search = new ShinyPIDSearcher(pid, useTID, tid, year, minDelay, maxDelay, infinite);
    auto *timer = new QTimer();

    connect(search, &ShinyPIDSearcher::finished, timer, &QTimer::deleteLater);
    connect(search, &ShinyPIDSearcher::finished, timer, &QTimer::stop);
    connect(search, &ShinyPIDSearcher::finished, this, [ = ] { ui->pushButtonShinyPIDSearch->setEnabled(true); ui->pushButtonShinyPIDCancel->setEnabled(false); });
    connect(search, &ShinyPIDSearcher::finished, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(ui->pushButtonShinyPIDCancel, &QPushButton::clicked, search, &ShinyPIDSearcher::cancelSearch);

    search->start();
    timer->start(1000);
}

void IDs4::on_pushButtonTIDSIDSearch_clicked()
{
    if (!ui->pushButtonShinyPIDSearch->isEnabled())
    {
        return;
    }

    model->removeRows(0, model->rowCount());

    ui->pushButtonTIDSIDSearch->setEnabled(false);
    ui->pushButtonTIDSIDCancel->setEnabled(true);

    u16 tid = ui->textBoxTIDSIDTID->text().toUShort();
    bool useSID = ui->checkBoxTIDSIDSearchSID->isChecked();
    u16 searchSID = ui->textBoxTIDSIDSID->text().toUShort();
    u32 year = ui->textBoxTIDSIDYear->text().toUInt();
    u32 minDelay = ui->textBoxTIDSIDMinDelay->text().toUInt();
    u32 maxDelay = ui->textBoxTIDSIDMaxDelay->text().toUInt();
    bool infinite = ui->checkBoxTIDSIDInfiniteSearch->isChecked();
    minDelay += (year - 2000);
    maxDelay += (year - 2000);

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(static_cast<int>(256 * 24 * (infinite ? 0xE8FFFF : (maxDelay - minDelay + 1))));

    auto *search = new TIDSIDSearcher(tid, useSID, searchSID, year, minDelay, maxDelay, infinite);
    auto *timer = new QTimer();

    connect(search, &TIDSIDSearcher::finished, timer, &QTimer::deleteLater);
    connect(search, &TIDSIDSearcher::finished, timer, &QTimer::stop);
    connect(search, &TIDSIDSearcher::finished, this, [ = ] { ui->pushButtonTIDSIDSearch->setEnabled(true); ui->pushButtonTIDSIDCancel->setEnabled(false); });
    connect(search, &TIDSIDSearcher::finished, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(ui->pushButtonTIDSIDCancel, &QPushButton::clicked, search, &TIDSIDSearcher::cancelSearch);

    search->start();
    timer->start(1000);
}

void IDs4::on_pushButtonSeedFinderSearch_clicked()
{
    if (!ui->pushButtonShinyPIDSearch->isEnabled() || !ui->pushButtonTIDSIDSearch->isEnabled())
    {
        return;
    }

    model->removeRows(0, model->rowCount());

    u16 tid = ui->textBoxSeedFinderTID->text().toUShort();
    QDateTime dateTime = ui->dateTimeEdit->dateTime();
    u32 month = dateTime.date().month();
    u32 day = dateTime.date().day();
    u32 year = dateTime.date().year();
    u32 hour = dateTime.time().hour();
    u32 minute = dateTime.time().minute();
    u32 minDelay = ui->textBoxSeedFinderMinDelay->text().toUInt();
    u32 maxDelay = ui->textBoxSeedFinderMaxDelay->text().toUInt();

    minDelay += (year - 2000);
    maxDelay += (year - 2000);

    for (u8 second = 0; second < 60; second++)
    {
        for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
        {
            u32 seed = (((((month * day) + (minute + second)) & 0xFF) << 24) | (hour << 16)) + efgh;

            MersenneTwister mt(seed, 1);
            u32 y = mt.nextUInt();

            u16 id = y & 0xFFFF;
            u16 sid = y >> 16;

            if (tid == id)
            {
                u32 delay = efgh + 2000 - year;
                auto frame = QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper().rightJustified(8, '0')) << new QStandardItem(QString::number(id))
                             << new QStandardItem(QString::number(sid)) << new QStandardItem(QString::number(delay)) << new QStandardItem(QString::number(second));
                model->appendRow(frame);
            }
        }
    }
}


ShinyPIDSearcher::ShinyPIDSearcher(u32 pid, bool useTID, u16 tid, u32 year, u32 minDelay, u32 maxDelay, bool infinite)
{
    this->pid = pid;
    this->useTID = useTID;
    this->tid = tid;
    this->year = year;
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
    this->infinite = infinite;
    cancel = false;
    progress = 0;

    connect(this, &ShinyPIDSearcher::finished, this, &ShinyPIDSearcher::deleteLater);
}

void ShinyPIDSearcher::run()
{
    maxDelay = infinite ? 0xE8FFFF : maxDelay;
    for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
    {
        for (u16 ab = 0; ab < 256; ab++)
        {
            for (u8 cd = 0; cd < 24; cd++)
            {
                if (cancel)
                {
                    return;
                }

                u32 seed = ((ab << 24) | (cd << 16)) + efgh;
                MersenneTwister mt(seed, 1);

                u32 y = mt.nextUInt();

                u16 id = y & 0xFFFF;
                u16 sid = y >> 16;

                if (Utilities::shiny(pid, id, sid) && (!useTID || id == tid))
                {
                    u32 delay = efgh + 2000 - year;
                    auto frame = QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper().rightJustified(8, '0')) << new QStandardItem(QString::number(id))
                                 << new QStandardItem(QString::number(sid)) << new QStandardItem(QString::number(delay)) << new QStandardItem("0");
                    QMutexLocker locker(&mutex);
                    results.append(frame);
                }

                progress++;
            }
        }
    }
}

int ShinyPIDSearcher::currentProgress() const
{
    return progress;
}

QVector<QList<QStandardItem *>> ShinyPIDSearcher::getResults()
{
    QMutexLocker locker(&mutex);
    auto data(results);
    results.clear();
    return data;
}

void ShinyPIDSearcher::cancelSearch()
{
    cancel = true;
}


TIDSIDSearcher::TIDSIDSearcher(u16 tid, bool useSID, u16 searchSID, u32 year, u32 minDelay, u32 maxDelay, bool infinite)
{
    this->tid = tid;
    this->useSID = useSID;
    this->searchSID = searchSID;
    this->year = year;
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
    this->infinite = infinite;
    cancel = false;
    progress = 0;

    connect(this, &TIDSIDSearcher::finished, this, &TIDSIDSearcher::deleteLater);
}

void TIDSIDSearcher::run()
{
    maxDelay = infinite ? 0xE8FFFF : maxDelay;
    for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
    {
        for (u16 ab = 0; ab < 256; ab++)
        {
            for (u8 cd = 0; cd < 24; cd++)
            {
                if (cancel)
                {
                    return;
                }

                u32 seed = ((ab << 24) | (cd << 16)) + efgh;
                MersenneTwister mt(seed, 1);

                u32 y = mt.nextUInt();

                u16 id = y & 0xFFFF;
                u16 sid = y >> 16;

                if (id == tid && (!useSID || sid == searchSID))
                {
                    u32 delay = efgh + 2000 - year;
                    auto frame = QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper().rightJustified(8, '0')) << new QStandardItem(QString::number(id))
                                 << new QStandardItem(QString::number(sid)) << new QStandardItem(QString::number(delay)) << new QStandardItem("0");
                    QMutexLocker locker(&mutex);
                    results.append(frame);
                }

                progress++;
            }
        }
    }
}

int TIDSIDSearcher::currentProgress() const
{
    return progress;
}

QVector<QList<QStandardItem *>> TIDSIDSearcher::getResults()
{
    QMutexLocker locker(&mutex);
    auto data(results);
    results.clear();
    return data;
}

void TIDSIDSearcher::cancelSearch()
{
    cancel = true;
}
