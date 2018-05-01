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

#include "SeedtoTime4.hpp"
#include "ui_SeedtoTime4.h"

SeedtoTime4::SeedtoTime4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SeedtoTime4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

SeedtoTime4::~SeedtoTime4()
{
    delete ui;
    delete dppt;
}

void SeedtoTime4::changeEvent(QEvent *event)
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

void SeedtoTime4::setupModels()
{
    ui->tableViewDPPt->setModel(dppt);
}

void SeedtoTime4::on_pushButtonGenerateDPPt_clicked()
{
    u32 seed = ui->textBoxSeedDPPt->text().toUInt(NULL, 16);
    u32 year = ui->lineEditYearDPPt->text().toUInt();

    bool forceSecond = ui->checkBoxSecondsDPPt->isChecked();
    int forcedSecond = ui->lineEditSecondsDPPt->text().toInt();

    if (year < 2000 || year > 2099)
    {
        QMessageBox error;
        error.setText(tr("Please enter a year between 2000 and 2099"));
        error.exec();
        return;
    }

    u32 ab = seed >> 24;
    u32 cd = (seed >> 16) & 0xFF;
    u32 efgh = seed & 0xFFFF;

    u32 delay = efgh + (2000 - year);
    u32 hour = cd;

    if (hour > 23)
    {
        QMessageBox error;
        error.setText(tr("Seed is invalid. Please enter a valid seed."));
        error.exec();
        return;
    }

    dppt->clear();
    dppt->setFlags();

    ui->labelCoinFlips->setText("Coin Flips: " + Utilities::coinFlips(seed, 15));

    vector<DateTime> results;
    for (int month = 0; month < 13; month++)
    {
        int maxDays = QDate(year, month, 1).daysInMonth();
        for (int day = 1; day <= maxDays; day++)
        {
            for (int minute = 0; minute < 60; minute++)
            {
                for (int second = 0; second < 60; second++)
                {
                    if (ab != ((month * day + minute + second) & 0xFF))
                        continue;

                    if (!forceSecond || second == forcedSecond)
                    {
                        QDateTime dateTime = QDateTime(QDate(year, month, day), QTime(hour, minute, second));
                        results.push_back(DateTime(dateTime, delay, Diamond));
                    }
                }
            }
        }
    }
    dppt->setModel(results);
}

void SeedtoTime4::on_checkBoxSecondsDPPt_clicked(bool checked)
{
    ui->lineEditSecondsDPPt->setEnabled(checked);
}

void SeedtoTime4::on_pushButtonSearchFlips_clicked()
{

}

void SeedtoTime4::on_pushButtonCalibrateDPPt_clicked()
{

}
