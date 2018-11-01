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

#include "IDs4.hpp"
#include "ui_IDs4.h"

IDs4::IDs4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IDs4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();

    connect(this, &IDs4::updateProgress, this, &IDs4::updateProgressBar);
}

IDs4::~IDs4()
{
    delete ui;
    delete model;
}

void IDs4::changeEvent(QEvent *event)
{
    if (event)
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

void IDs4::setupModels()
{
    ui->tableView->setModel(model);
    model->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("TID") << tr("SID") << tr("Delay") << tr("Seconds"));
}

void IDs4::updateSearch()
{
    while (isSearching && !cancel)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        emit updateProgress();
    }
}

void IDs4::on_checkBoxSearchTIDShinyPID_toggled(bool checked)
{
    ui->label_2->setEnabled(checked);
    ui->textBoxTIDShinyPID->setEnabled(checked);
}

void IDs4::on_checkBoxInfiniteSearchShinyPID_toggled(bool checked)
{
    ui->label_5->setEnabled(checked);
    ui->textBoxMaxDelayShinyPID->setEnabled(checked);
}

void IDs4::on_checkBoxSearchSID_toggled(bool checked)
{
    ui->label_8->setEnabled(checked);
    ui->textBoxSIDTIDSID->setEnabled(checked);
}

void IDs4::on_checkBoxInfiniteSearchTIDSID_toggled(bool checked)
{
    ui->label_7->setEnabled(checked);
    ui->textBoxMaxDelayTIDSID->setEnabled(checked);
}

void IDs4::updateProgressBar()
{
    ui->progressBar->setValue(progress);
}

void IDs4::on_pushButtonSearchShinyPID_clicked()
{
    if (isSearching && ui->pushButtonSearchShinyPID->text() == tr("Cancel"))
    {
        cancel = true;
    }
    else if (!isSearching)
    {
        model->removeRows(0, model->rowCount());

        ui->progressBar->setValue(0);
        progress = 0;

        isSearching = true;
        cancel = false;
        ui->pushButtonSearchShinyPID->setText(tr("Cancel"));

        std::thread job(&IDs4::searchPID, this);
        job.detach();

        std::thread update(&IDs4::updateSearch, this);
        update.detach();
    }
}

void IDs4::on_pushButtonSearchTIDSID_clicked()
{
    if (isSearching && ui->pushButtonSearchTIDSID->text() == tr("Cancel"))
    {
        cancel = true;
    }
    else if (!isSearching)
    {
        model->removeRows(0, model->rowCount());

        ui->progressBar->setValue(0);
        progress = 0;

        isSearching = true;
        cancel = false;
        ui->pushButtonSearchTIDSID->setText(tr("Cancel"));

        std::thread job(&IDs4::searchTIDSID, this);
        job.detach();

        std::thread update(&IDs4::updateSearch, this);
        update.detach();
    }
}

void IDs4::on_pushButtonSearchSeedFinder_clicked()
{
    if (isSearching)
        return;

    model->removeRows(0, model->rowCount());

    u16 tid = ui->textBoxTIDSeedFinder->text().toUShort();
    u32 month = ui->textBoxMonthSeedFinder->text().toUInt();
    u32 day = ui->textBoxDaySeedFinder->text().toUInt();
    u32 year = ui->textBoxYearSeedFinder->text().toUInt();
    u32 hour = ui->textBoxHourSeedFinder->text().toUInt();
    u32 minute = ui->textBoxMinuteSeedFinder->text().toUInt();
    u32 minDelay = ui->textBoxMinDelaySeedFinder->text().toUInt();
    u32 maxDelay = ui->textBoxMaxDelaySeedFinder->text().toUInt();

    QDate date(static_cast<int>(year), static_cast<int>(month), static_cast<int>(day));
    if (!date.isValid())
    {
        QMessageBox error;
        error.setText(tr("Invalid date"));
        error.exec();
        return;
    }

    QTime time(static_cast<int>(hour), static_cast<int>(minute));
    if (!time.isValid())
    {
        QMessageBox error;
        error.setText(tr("Invalid hour/minute"));
        error.exec();
        return;
    }

    minDelay += (year - 2000);
    maxDelay += (year - 2000);

    for (u32 second = 0; second < 60; second++)
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

void IDs4::searchPID()
{
    u32 pid = ui->textBoxPID->text().toUInt(nullptr, 16);
    bool useTID = ui->checkBoxSearchTIDShinyPID->isChecked();
    u16 tid = ui->textBoxTIDShinyPID->text().toUShort();
    u32 year = ui->textBoxYearShinyPID->text().toUInt();
    u32 minDelay = ui->textBoxMinDelayShinyPID->text().toUInt();
    u32 maxDelay = ui->textBoxMaxDelayShinyPID->text().toUInt();
    bool infinite = ui->checkBoxInfiniteSearchShinyPID->isChecked();

    ui->progressBar->setMaximum(static_cast<int>(256 * 24 * (infinite ? 0xE8FFFF : (maxDelay - minDelay + 1))));

    minDelay += (year - 2000);
    maxDelay += (year - 2000);

    for (u32 efgh = minDelay; efgh <= (infinite ? 0xE8FFFF : maxDelay); efgh++)
    {
        for (u32 ab = 0; ab < 256; ab++)
        {
            for (u32 cd = 0; cd < 24; cd++)
            {
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
                    model->appendRow(frame);
                }

                progress++;

                if (cancel)
                {
                    isSearching = false;
                    ui->pushButtonSearchShinyPID->setText(tr("Search"));
                    emit updateProgress();
                    return;
                }
            }
        }
    }
    isSearching = false;
    ui->pushButtonSearchShinyPID->setText(tr("Search"));
    emit updateProgress();
}

void IDs4::searchTIDSID()
{
    u16 tid = ui->textBoxTIDTIDSID->text().toUShort();
    bool useSID = ui->checkBoxSearchSID->isChecked();
    u16 searchSID = ui->textBoxSIDTIDSID->text().toUShort();
    u32 year = ui->textBoxYearTIDSID->text().toUInt();
    u32 minDelay = ui->textBoxMinDelayTIDSID->text().toUInt();
    u32 maxDelay = ui->textBoxMaxDelayTIDSID->text().toUInt();
    bool infinite = ui->checkBoxInfiniteSearchTIDSID->isChecked();

    ui->progressBar->setMaximum(static_cast<int>(256 * 24 * (infinite ? 0xE8FFFF : (maxDelay - minDelay + 1))));

    minDelay += (year - 2000);
    maxDelay += (year - 2000);

    for (u32 efgh = minDelay; efgh <= (infinite ? 0xE8FFFF : maxDelay); efgh++)
    {
        for (u32 ab = 0; ab < 256; ab++)
        {
            for (u32 cd = 0; cd < 24; cd++)
            {
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
                    model->appendRow(frame);
                }

                progress++;

                if (cancel)
                {
                    isSearching = false;
                    ui->pushButtonSearchTIDSID->setText(tr("Search"));
                    emit updateProgress();
                    return;
                }
            }
        }
    }
    isSearching = false;
    ui->pushButtonSearchTIDSID->setText(tr("Search"));
    emit updateProgress();
}
