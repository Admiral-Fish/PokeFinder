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

SeedtoTime4::SeedtoTime4(QString seed, Profile4 profile, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SeedtoTime4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();

    Game version = profile.getVersion();

    if (version == Game::HeartGold || version == Game::SoulSilver)
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->textBoxSeedHGSS->setText(seed);
        on_pushButtonGenerateHGSS_clicked();
    }
    else
    {
        ui->textBoxSeedDPPt->setText(seed);
        on_pushButtonGenerateDPPt_clicked();
    }
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

void SeedtoTime4::changeEvent(QEvent *event)
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

QVector<DateTime> SeedtoTime4::generate(u32 seed, u32 year, bool forceSecond, int forcedSecond, Game version)
{
    if (year < 2000 || year > 2099)
    {
        QMessageBox error;
        error.setText(tr("Please enter a year between 2000 and 2099"));
        error.exec();
        return QVector<DateTime>();
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
        return QVector<DateTime>();
    }

    QVector<bool> roamer = { ui->checkBoxR->isChecked(), ui->checkBoxE->isChecked(), ui->checkBoxL->isChecked() };
    QVector<u16> routes = { ui->lineEditR->text().toUShort(), ui->lineEditE->text().toUShort(), ui->lineEditL->text().toUShort() };

    QVector<DateTime> results;
    for (int month = 0; month < 13; month++)
    {
        int maxDays = QDate(static_cast<int>(year), month, 1).daysInMonth();
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
                        QDateTime dateTime = QDateTime(QDate(static_cast<int>(year), month, day), QTime(static_cast<int>(hour), minute, second));
                        results.append(DateTime(dateTime, delay, version, roamer, routes));
                    }
                }
            }
        }
    }
    return results;
}

QVector<DateTime> SeedtoTime4::calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond, DateTime target)
{
    QDateTime time = target.getDateTime();

    u32 delay = target.getDelay();

    QVector<bool> roamer = { ui->checkBoxR->isChecked(), ui->checkBoxE->isChecked(), ui->checkBoxL->isChecked() };
    QVector<u32> routes = { ui->lineEditR->text().toUInt(), ui->lineEditE->text().toUInt(), ui->lineEditL->text().toUInt() };

    QVector<DateTime> results;
    for (int i = minusSecond; i >= 0; i--)
    {
        for (int j = minusDelay; j > 0; j--)
        {
            QDateTime offset = time.addSecs(-1 * i);
            DateTime result = DateTime(offset, delay - static_cast<u32>(j), target.getVersion(), target.getInfo());
            results.push_back(result);
        }
    }

    results.push_back(target);

    for (int i = 0; i <= plusSecond; i++)
    {
        for (int j = 1; j <= plusDelay; j++)
        {
            QDateTime offset = time.addSecs(i);
            DateTime result = DateTime(offset, delay + static_cast<u32>(j), target.getVersion(), target.getInfo());
            results.push_back(result);
        }
    }
    return results;
}

void SeedtoTime4::on_pushButtonGenerateDPPt_clicked()
{
    u32 seed = ui->textBoxSeedDPPt->text().toUInt(nullptr, 16);
    u32 year = ui->lineEditYearDPPt->text().toUInt();

    bool forceSecond = ui->checkBoxSecondsDPPt->isChecked();
    int forcedSecond = ui->lineEditSecondsDPPt->text().toInt();

    dppt->clear();

    QVector<DateTime> results = generate(seed, year, forceSecond, forcedSecond, Game::Diamond);
    ui->labelCoinFlips->setText(tr("Coin Flips: ") + Utilities::coinFlips(seed, 15));

    dppt->setModel(results);
}

void SeedtoTime4::on_pushButtonGenerateHGSS_clicked()
{
    hgss->clear();

    u32 seed = ui->textBoxSeedHGSS->text().toUInt(nullptr, 16);
    u32 year = ui->lineEditYearHGSS->text().toUInt();

    bool forceSecond = ui->checkBoxSecondsHGSS->isChecked();
    int forcedSecond = ui->lineEditSecondsHGSS->text().toInt();

    QVector<bool> roamer = { ui->checkBoxR->isChecked(), ui->checkBoxE->isChecked(), ui->checkBoxL->isChecked() };
    QVector<u16> routes = { ui->lineEditR->text().toUShort(), ui->lineEditE->text().toUShort(), ui->lineEditL->text().toUShort() };

    HGSSRoamer info(seed, roamer, routes);

    QVector<DateTime> results = generate(seed, year, forceSecond, forcedSecond, Game::HeartGold);
    ui->labelElmCalls->setText(tr("Elm Calls: ") + Utilities::getCalls(seed, 15, info));
    QString str = info.getRoutes();
    str = str == "" ? tr("No roamers") : str;
    ui->labelRoamers->setText(tr("Roamers: ") + str);

    hgss->setModel(results);
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
    QVector<DateTime> results = calibrate(minusDelay, plusDelay, minusSecond, plusSecond, target);

    dpptCalibrate->setModel(results);

    int count = (results.size() - 1) / 2;
    QModelIndex scroll = dpptCalibrate->index(count, 0);
    ui->tableViewDPPtCalibrate->setCurrentIndex(scroll);
    ui->tableViewDPPtCalibrate->scrollTo(scroll);
    ui->tableViewDPPtCalibrate->setFocus();
}

void SeedtoTime4::on_pushButtonCalibrateHGSS_clicked()
{
    hgssCalibrate->clear();

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

    DateTime target = hgss->getData(index.row());
    QVector<DateTime> results = calibrate(minusDelay, plusDelay, minusSecond, plusSecond, target);

    hgssCalibrate->setModel(results);

    int count = (results.size() - 1) / 2;
    QModelIndex scroll = hgssCalibrate->index(count, 0);
    ui->tableViewHGSSCalibrate->setCurrentIndex(scroll);
    ui->tableViewHGSSCalibrate->scrollTo(scroll);
    ui->tableViewHGSSCalibrate->setFocus();
}

void SeedtoTime4::on_pushButtonSearchFlips_clicked()
{
    if (dpptCalibrate->rowCount() == 0)
        return;

    SearchCoinFlips *search = new SearchCoinFlips(dpptCalibrate->getData());
    if (search->exec() == QDialog::Rejected)
    {
        delete search;
        return;
    }

    QVector<bool> results = search->possibleResults();

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

void SeedtoTime4::on_pushButtonSearchCalls_clicked()
{
    if (hgssCalibrate->rowCount() == 0)
        return;

    QVector<bool> roamer = { ui->checkBoxR->isChecked(), ui->checkBoxE->isChecked(), ui->checkBoxL->isChecked() };
    QVector<u32> routes = { ui->lineEditR->text().toUInt(), ui->lineEditE->text().toUInt(), ui->lineEditL->text().toUInt() };

    SearchCalls *search = new SearchCalls(hgssCalibrate->getData(), roamer, routes);
    if (search->exec() == QDialog::Rejected)
    {
        delete search;
        return;
    }

    QVector<bool> results = search->possibleResults();

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

void SeedtoTime4::on_checkBoxSecondsDPPt_clicked(bool checked)
{
    ui->lineEditSecondsDPPt->setEnabled(checked);
}

void SeedtoTime4::on_checkBoxSecondsHGSS_clicked(bool checked)
{
    ui->lineEditSecondsHGSS->setEnabled(checked);
}

void SeedtoTime4::on_pushButtonMap_clicked()
{
    RoamerMap *map = new RoamerMap();
    map->show();
    map->raise();
}
