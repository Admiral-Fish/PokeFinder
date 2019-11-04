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

#include "SeedToTime3.hpp"
#include "ui_SeedToTime3.h"
#include <Core/RNG/LCRNG.hpp>
#include <QDateTime>
#include <QMessageBox>
#include <QSettings>

namespace PokeFinderForms
{
    SeedToTime3::SeedToTime3(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::SeedToTime3)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose);

        setupModels();
    }

    SeedToTime3::SeedToTime3(u32 seed, QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::SeedToTime3)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose);

        setupModels();

        ui->textBoxSeed->setText(QString::number(seed, 16));
        on_pushButtonFind_clicked();
    }

    SeedToTime3::~SeedToTime3()
    {
        QSettings setting;
        setting.beginGroup("seedToTime3");
        setting.setValue("year", ui->textBoxYear->text());
        setting.setValue("geometry", this->saveGeometry());
        setting.endGroup();
    }

    void SeedToTime3::setupModels()
    {
        model = new QStandardItemModel(ui->tableView);

        ui->textBoxSeed->setValues(InputType::Seed32Bit);
        ui->textBoxYear->setValues(0, 2037);

        model->setHorizontalHeaderLabels(QStringList() << tr("Time") << tr("Frame"));
        ui->tableView->setModel(model);

        QSettings setting;
        setting.beginGroup("seedToTime3");
        if (setting.contains("year"))
            ui->textBoxYear->setText(setting.value("year").toString());
        if (setting.contains("geometry"))
            this->restoreGeometry(setting.value("geometry").toByteArray());
        setting.endGroup();
    }

    u16 SeedToTime3::originSeed(u32 seed)
    {
        PokeFinderCore::PokeRNGR rng(seed);
        while (rng.getSeed() > 0xFFFF)
        {
            rng.nextUInt();
            frame++;
        }
        return rng.getSeed();
    }

    void SeedToTime3::seedToTime(u32 seed, u32 year)
    {
        model->removeRows(0, model->rowCount());

        u32 minDay = 0;
        u32 maxDay = 0;

        // For whatever reason the start date is different if the year is greater then 2000
        QDateTime start(QDate(year == 2000 ? 2000 : 2001, 1, 1), QTime(0, 0));

        // Hard cap upper year since game crashes above year 2037
        // Signed overflow error due to how the clock is setup
        if (year < 2000 || year > 2037)
        {
            QMessageBox error;
            error.setText(tr("Please enter a year between 2000 and 2037"));
            error.exec();
            return;
        }

        // Game decides to ignore a year of counting days
        for (u32 x = 2001; x < year; x++)
        {
            QDate temp(x, 1, 1);
            minDay += static_cast<u32>(temp.daysInYear());
            maxDay += static_cast<u32>(temp.daysInYear());
        }

        // Loop through the year generating seeds to check against user input
        for (u32 month = 1; month < 13; month++)
        {
            QDate temp(2000, month, 1);
            maxDay += static_cast<u32>(temp.daysInMonth());
            for (u32 day = minDay; day < maxDay; day++)
            {
                for (u32 hour = 0; hour < 24; hour++)
                {
                    for (u32 minute = 0; minute < 60; minute++)
                    {
                        // Formula to generate intial seed
                        u32 v = 1440 * day + 960 * (hour / 10) + 60 * (hour % 10) + 16 * (minute / 10) + (minute % 10)
                            + 0x5A0;
                        v = (v >> 16) ^ (v & 0xFFFF);

                        if (v == seed)
                        {
                            QDateTime finalTime = start.addDays(day).addSecs((hour * 60 * 60) + (minute * 60));
                            QString result = finalTime.toString(Qt::SystemLocaleShortDate);
                            model->appendRow(QList<QStandardItem *>()
                                << new QStandardItem(result) << new QStandardItem(QString::number(frame)));
                        }
                    }
                }
            }
            minDay += static_cast<u32>(temp.daysInMonth());
        }
    }

    void SeedToTime3::on_pushButtonFind_clicked()
    {
        u32 seed = ui->textBoxSeed->getUInt();
        u32 year = ui->textBoxYear->getUInt();
        frame = 1;
        if (seed > 0xFFFF)
        {
            seed = originSeed(seed);
            ui->textBoxSeed->setText(QString::number(seed, 16));
        }

        seedToTime(seed, year);
    }
}
