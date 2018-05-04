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
    saveSettings();

    delete ui;
    delete dppt;
    delete dpptCalibrate;
    delete hgss;
    delete hgssCalibrate;
}

void SeedtoTime4::closeWindow()
{
    this->close();
    delete this;
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
    loadSettings();

    ui->textBoxSeedDPPt->setValues(0, 32, false);
    ui->textBoxSeedHGSS->setValues(0, 32, false);

    ui->tableViewDPPt->setModel(dppt);
    ui->tableViewDPPtCalibrate->setModel(dpptCalibrate);

    ui->tableViewHGSS->setModel(hgss);
    ui->tableViewHGSSCalibrate->setModel(hgssCalibrate);
}

void SeedtoTime4::saveSettings()
{
    QSettings settings;
    settings.setValue("dpptYear", ui->lineEditYearDPPt->text());
    settings.setValue("minusDelayDPPt", ui->lineEditMinusDelayDPPt->text());
    settings.setValue("plusDelayDPPt", ui->lineEditPlusDelayDPPt->text());
    settings.setValue("minusSecondsDPPt", ui->lineEditMinusSecondsDPPt->text());
    settings.setValue("plusSecondsDPPt", ui->lineEditPlusSecondsDPPt->text());
    settings.setValue("hgssYear", ui->lineEditYearHGSS->text());
    settings.setValue("minusDelayHGSS", ui->lineEditMinusDelayHGSS->text());
    settings.setValue("plusDelayHGSS", ui->lineEditPlusDelayHGSS->text());
    settings.setValue("minusSecondsHGSS", ui->lineEditMinusSecondsHGSS->text());
    settings.setValue("plusSecondsHGSS", ui->lineEditPlusSecondsHGSS->text());
}

void SeedtoTime4::loadSettings()
{
    QSettings setting;
    if (setting.contains("dpptYear")) ui->lineEditYearDPPt->setText(setting.value("dpptYear").toString());
    if (setting.contains("minusDelayDPPt")) ui->lineEditMinusDelayDPPt->setText(setting.value("minusDelayDPPt").toString());
    if (setting.contains("plusDelayDPPt")) ui->lineEditPlusDelayDPPt->setText(setting.value("plusDelayDPPt").toString());
    if (setting.contains("minusSecondsDPPt")) ui->lineEditMinusSecondsDPPt->setText(setting.value("minusSecondsDPPt").toString());
    if (setting.contains("plusSecondsDPPt")) ui->lineEditPlusSecondsDPPt->setText(setting.value("plusSecondsDPPt").toString());
    if (setting.contains("hgssYear")) ui->lineEditYearHGSS->setText(setting.value("hgssYear").toString());
    if (setting.contains("minusDelayHGSS")) ui->lineEditMinusDelayHGSS->setText(setting.value("minusDelayHGSS").toString());
    if (setting.contains("plusDelayHGSS")) ui->lineEditPlusDelayHGSS->setText(setting.value("plusDelayHGSS").toString());
    if (setting.contains("minusSecondsHGSS")) ui->lineEditMinusSecondsHGSS->setText(setting.value("minusSecondsHGSS").toString());
    if (setting.contains("plusSecondsHGSS")) ui->lineEditPlusSecondsHGSS->setText(setting.value("plusSecondsHGSS").toString());
}

vector<DateTime> SeedtoTime4::generate(u32 seed, u32 year, bool forceSecond, int forcedSecond, Game version)
{
    if (year < 2000 || year > 2099)
    {
        QMessageBox error;
        error.setText(tr("Please enter a year between 2000 and 2099"));
        error.exec();
        return vector<DateTime>();
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
        return vector<DateTime>();
    }

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
                        results.push_back(DateTime(dateTime, delay, version));
                    }
                }
            }
        }
    }
    return results;
}

vector<DateTime> SeedtoTime4::calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond, DateTime target)
{
    QDateTime time = target.getDateTime();

    int delay = target.getDelay();

    vector<DateTime> results;
    for (int i = minusSecond; i >= 0; i--)
    {
        for (int j = minusDelay; j > 0; j--)
        {
            QDateTime offset = time.addSecs(-1 * i);
            DateTime result = DateTime(offset, delay - j, target.getVersion());
            results.push_back(result);
        }
    }

    results.push_back(target);

    for (int i = 0; i <= plusSecond; i++)
    {
        for (int j = 1; j <= plusDelay; j++)
        {
            QDateTime offset = time.addSecs(i);
            DateTime result = DateTime(offset, delay + j, target.getVersion());
            results.push_back(result);
        }
    }
    return results;
}

void SeedtoTime4::on_pushButtonGenerateDPPt_clicked()
{
    u32 seed = ui->textBoxSeedDPPt->text().toUInt(NULL, 16);
    u32 year = ui->lineEditYearDPPt->text().toUInt();

    bool forceSecond = ui->checkBoxSecondsDPPt->isChecked();
    int forcedSecond = ui->lineEditSecondsDPPt->text().toInt();

    dppt->clear();

    vector<DateTime> results = generate(seed, year, forceSecond, forcedSecond, Diamond);
    ui->labelCoinFlips->setText(tr("Coin Flips: ") + Utilities::coinFlips(seed, 15));

    dppt->setModel(results);
}

void SeedtoTime4::on_checkBoxSecondsDPPt_clicked(bool checked)
{
    ui->lineEditSecondsDPPt->setEnabled(checked);
}

void SeedtoTime4::on_checkBoxSecondsHGSS_clicked(bool checked)
{
    ui->lineEditSecondsHGSS->setEnabled(checked);
}

void SeedtoTime4::on_pushButtonSearchFlips_clicked()
{
    if (dpptCalibrate->rowCount() == 0)
        return;

    SearchCoinFlips *search = new SearchCoinFlips(dpptCalibrate->getData());
    if (search->exec() == QDialog::Rejected)
        return;

    vector<bool> results = search->possibleResults();

    ui->tableViewDPPtCalibrate->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableViewDPPtCalibrate->clearSelection();

    for (int i = 0; i < results.size(); i++)
    {
        if (results[i])
            ui->tableViewDPPtCalibrate->selectRow(i);
    }

    ui->tableViewDPPtCalibrate->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewDPPtCalibrate->setFocus();

    delete search;
}

void SeedtoTime4::on_pushButtonCalibrateDPPt_clicked()
{
    int minusDelay = ui->lineEditMinusDelayDPPt->text().toInt();
    int plusDelay = ui->lineEditPlusDelayDPPt->text().toInt();

    int minusSecond = ui->lineEditMinusSecondsDPPt->text().toInt();
    int plusSecond = ui->lineEditPlusSecondsDPPt->text().toInt();

    QModelIndex index = ui->tableViewDPPt->currentIndex();

    if (!index.isValid())
    {
        QMessageBox error;
        error.setText("Please select a result from Seed to Time.");
        error.exec();
        return;
    }

    dpptCalibrate->clear();

    DateTime target = dppt->getData(index.row());
    vector<DateTime> results = calibrate(minusDelay, plusDelay, minusSecond, plusSecond, target);

    dpptCalibrate->setModel(results);

    int count = (results.size() - 1) / 2;
    QModelIndex scroll = dpptCalibrate->index(count, 0);
    ui->tableViewDPPtCalibrate->setCurrentIndex(scroll);
    ui->tableViewDPPtCalibrate->scrollTo(scroll);
    ui->tableViewDPPtCalibrate->setFocus();
}

void SeedtoTime4::on_pushButtonGenerateHGSS_clicked()
{
    u32 seed = ui->textBoxSeedHGSS->text().toUInt(NULL, 16);
    u32 year = ui->lineEditYearHGSS->text().toUInt();

    bool forceSecond = ui->checkBoxSecondsHGSS->isChecked();
    int forcedSecond = ui->lineEditSecondsHGSS->text().toInt();

    hgss->clear();

    vector<DateTime> results = generate(seed, year, forceSecond, forcedSecond, HeartGold);
    ui->labelElmCalls->setText(tr("Elm Calls: ") + Utilities::elmCalls(seed, 15));

    hgss->setModel(results);
}

void SeedtoTime4::on_pushButtonSearchCalls_clicked()
{
    if (hgssCalibrate->rowCount() == 0)
        return;

    SearchElmCalls *search = new SearchElmCalls(hgssCalibrate->getData());
    if (search->exec() == QDialog::Rejected)
        return;

    vector<bool> results = search->possibleResults();

    ui->tableViewHGSSCalibrate->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableViewHGSSCalibrate->clearSelection();

    for (int i = 0; i < results.size(); i++)
    {
        if (results[i])
            ui->tableViewHGSSCalibrate->selectRow(i);
    }

    ui->tableViewHGSSCalibrate->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewHGSSCalibrate->setFocus();

    delete search;
}

void SeedtoTime4::on_pushButtonCalibrateHGSS_clicked()
{
    int minusDelay = ui->lineEditMinusDelayHGSS->text().toInt();
    int plusDelay = ui->lineEditPlusDelayHGSS->text().toInt();

    int minusSecond = ui->lineEditMinusSecondsHGSS->text().toInt();
    int plusSecond = ui->lineEditPlusSecondsHGSS->text().toInt();

    QModelIndex index = ui->tableViewHGSS->currentIndex();

    if (!index.isValid())
    {
        QMessageBox error;
        error.setText("Please select a result from Seed to Time.");
        error.exec();
        return;
    }

    hgssCalibrate->clear();

    DateTime target = hgss->getData(index.row());
    vector<DateTime> results = calibrate(minusDelay, plusDelay, minusSecond, plusSecond, target);

    hgssCalibrate->setModel(results);

    int count = (results.size() - 1) / 2;
    QModelIndex scroll = hgssCalibrate->index(count, 0);
    ui->tableViewHGSSCalibrate->setCurrentIndex(scroll);
    ui->tableViewHGSSCalibrate->scrollTo(scroll);
    ui->tableViewHGSSCalibrate->setFocus();
}
