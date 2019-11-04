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
#include <Core/Gen4/IDSearcher4.hpp>
#include <Core/RNG/MTRNG.hpp>
#include <QSettings>

namespace PokeFinderForms
{
    IDs4::IDs4(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::IDs4)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);

        setupModels();
    }

    IDs4::~IDs4()
    {
        QSettings setting;
        setting.setValue("ids4/geometry", this->saveGeometry());

        delete ui;
    }

    void IDs4::setupModels()
    {
        shinyPID = new QStandardItemModel(ui->tableViewShinyPID);
        ui->tableViewShinyPID->setModel(shinyPID);
        shinyPID->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("TID") << tr("SID") << tr("Delay"));

        tidSID = new QStandardItemModel(ui->tableViewTIDSID);
        ui->tableViewTIDSID->setModel(tidSID);
        tidSID->setHorizontalHeaderLabels(QStringList() << tr("Seed") << tr("TID") << tr("SID") << tr("Delay"));

        seedFinder = new QStandardItemModel(ui->tableViewSeedFinder);
        ui->tableViewSeedFinder->setModel(seedFinder);
        seedFinder->setHorizontalHeaderLabels(
            QStringList() << tr("Seed") << tr("TID") << tr("SID") << tr("Delay") << tr("Seconds"));

        ui->textBoxTIDSIDTID->setValues(InputType::TIDSID);
        ui->textBoxTIDSIDSID->setValues(InputType::TIDSID);
        ui->textBoxTIDSIDYear->setValues(0, 2099);
        ui->textBoxTIDSIDMinDelay->setValues(InputType::Delay);
        ui->textBoxTIDSIDMaxDelay->setValues(InputType::Delay);

        ui->textBoxShinyPIDPID->setValues(InputType::Seed32Bit);
        ui->textBoxShinyPIDTID->setValues(InputType::TIDSID);
        ui->textBoxShinyPIDYear->setValues(0, 2099);
        ui->textBoxShinyPIDMinDelay->setValues(InputType::Delay);
        ui->textBoxShinyPIDMaxDelay->setValues(InputType::Delay);

        ui->textBoxSeedFinderTID->setValues(InputType::TIDSID);
        ui->textBoxSeedFinderMinDelay->setValues(InputType::Delay);
        ui->textBoxSeedFinderMaxDelay->setValues(InputType::Delay);

        QSettings setting;
        if (setting.contains("ids4/geometry"))
            this->restoreGeometry(setting.value("ids4/geometry").toByteArray());
    }

    void IDs4::updateProgressShinyPID(const QVector<QList<QStandardItem *>> &frames, int progress)
    {
        for (const auto &item : frames)
        {
            shinyPID->appendRow(item);
        }
        ui->progressBarShinyPID->setValue(progress);
    }

    void IDs4::updateProgressTIDSID(const QVector<QList<QStandardItem *>> &frames, int progress)
    {
        for (const auto &item : frames)
        {
            tidSID->appendRow(item);
        }
        ui->progressBarTIDSID->setValue(progress);
    }

    void IDs4::on_pushButtonShinyPIDSearch_clicked()
    {
        if (!ui->pushButtonTIDSIDSearch->isEnabled())
        {
            return;
        }

        shinyPID->removeRows(0, shinyPID->rowCount());

        ui->pushButtonShinyPIDSearch->setEnabled(false);
        ui->pushButtonShinyPIDCancel->setEnabled(true);

        u32 pid = ui->textBoxShinyPIDPID->getUInt();
        bool useTID = ui->checkBoxShinyPIDSearchTID->isChecked();
        u16 tid = ui->textBoxShinyPIDTID->getUShort();
        u32 year = ui->textBoxShinyPIDYear->getUInt();
        u32 minDelay = ui->textBoxShinyPIDMinDelay->getUInt();
        u32 maxDelay = ui->textBoxShinyPIDMaxDelay->getUInt();
        bool infinite = ui->checkBoxShinyPIDInfiniteSearch->isChecked();
        minDelay += (year - 2000);
        maxDelay += (year - 2000);

        ui->progressBarShinyPID->setValue(0);
        ui->progressBarShinyPID->setMaximum(
            static_cast<int>(256 * 24 * (infinite ? 0xE8FFFF : (maxDelay - minDelay + 1))));

        auto *search = new PokeFinderCore::ShinyPIDSearcher(pid, useTID, tid, year, minDelay, maxDelay, infinite);

        connect(search, &PokeFinderCore::ShinyPIDSearcher::finished, this, [=] {
            ui->pushButtonShinyPIDSearch->setEnabled(true);
            ui->pushButtonShinyPIDCancel->setEnabled(false);
        });
        connect(search, &PokeFinderCore::ShinyPIDSearcher::updateProgress, this, &IDs4::updateProgressShinyPID);
        connect(ui->pushButtonShinyPIDCancel, &QPushButton::clicked, search,
            &PokeFinderCore::ShinyPIDSearcher::cancelSearch);

        search->startSearch();
    }

    void IDs4::on_pushButtonTIDSIDSearch_clicked()
    {
        if (!ui->pushButtonShinyPIDSearch->isEnabled())
        {
            return;
        }

        tidSID->removeRows(0, tidSID->rowCount());

        ui->pushButtonTIDSIDSearch->setEnabled(false);
        ui->pushButtonTIDSIDCancel->setEnabled(true);

        u16 tid = ui->textBoxTIDSIDTID->getUShort();
        bool useSID = ui->checkBoxTIDSIDSearchSID->isChecked();
        u16 searchSID = ui->textBoxTIDSIDSID->getUShort();
        u32 year = ui->textBoxTIDSIDYear->getUInt();
        u32 minDelay = ui->textBoxTIDSIDMinDelay->getUInt();
        u32 maxDelay = ui->textBoxTIDSIDMaxDelay->getUInt();
        bool infinite = ui->checkBoxTIDSIDInfiniteSearch->isChecked();
        minDelay += (year - 2000);
        maxDelay += (year - 2000);

        ui->progressBarTIDSID->setValue(0);
        ui->progressBarTIDSID->setMaximum(
            static_cast<int>(256 * 24 * (infinite ? 0xE8FFFF : (maxDelay - minDelay + 1))));

        auto *search = new PokeFinderCore::TIDSIDSearcher(tid, useSID, searchSID, year, minDelay, maxDelay, infinite);

        connect(search, &PokeFinderCore::TIDSIDSearcher::finished, this, [=] {
            ui->pushButtonTIDSIDSearch->setEnabled(true);
            ui->pushButtonTIDSIDCancel->setEnabled(false);
        });
        connect(search, &PokeFinderCore::TIDSIDSearcher::updateProgress, this, &IDs4::updateProgressTIDSID);
        connect(
            ui->pushButtonTIDSIDCancel, &QPushButton::clicked, search, &PokeFinderCore::TIDSIDSearcher::cancelSearch);

        search->startSearch();
    }

    void IDs4::on_pushButtonSeedFinderSearch_clicked()
    {
        if (!ui->pushButtonShinyPIDSearch->isEnabled() || !ui->pushButtonTIDSIDSearch->isEnabled())
        {
            return;
        }

        seedFinder->removeRows(0, seedFinder->rowCount());

        u16 tid = ui->textBoxSeedFinderTID->getUShort();
        QDateTime dateTime = ui->dateTimeEdit->dateTime();
        u32 month = dateTime.date().month();
        u32 day = dateTime.date().day();
        u32 year = dateTime.date().year();
        u32 hour = dateTime.time().hour();
        u32 minute = dateTime.time().minute();
        u32 minDelay = ui->textBoxSeedFinderMinDelay->getUInt();
        u32 maxDelay = ui->textBoxSeedFinderMaxDelay->getUInt();

        minDelay += (year - 2000);
        maxDelay += (year - 2000);

        for (u8 second = 0; second < 60; second++)
        {
            for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
            {
                u32 seed = (((((month * day) + (minute + second)) & 0xFF) << 24) | (hour << 16)) + efgh;

                PokeFinderCore::MersenneTwister mt(seed, 1);
                u32 y = mt.nextUInt();

                u16 id = y & 0xFFFF;
                u16 sid = y >> 16;

                if (tid == id)
                {
                    u32 delay = efgh + 2000 - year;
                    auto frame = QList<QStandardItem *>()
                        << new QStandardItem(QString::number(seed, 16).toUpper().rightJustified(8, '0'))
                        << new QStandardItem(QString::number(id)) << new QStandardItem(QString::number(sid))
                        << new QStandardItem(QString::number(delay)) << new QStandardItem(QString::number(second));
                    seedFinder->appendRow(frame);
                }
            }
        }
    }
}
