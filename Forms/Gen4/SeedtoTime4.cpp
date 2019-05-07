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

#include <QMessageBox>
#include <QSettings>
#include "SeedtoTime4.hpp"
#include "ui_SeedtoTime4.h"
#include <Core/Util/Utilities.hpp>
#include <Forms/Gen4/RoamerMap.hpp>
#include <Forms/Gen4/SearchCalls.hpp>
#include <Forms/Gen4/SearchCoinFlips.hpp>

SeedtoTime4::SeedtoTime4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SeedtoTime4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

SeedtoTime4::SeedtoTime4(const QString &seed, const Profile4 &profile, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SeedtoTime4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();

    if (profile.getVersion() & Game::HGSS)
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->textBoxHGSSSeed->setText(seed);
        on_pushButtonHGSSGenerate_clicked();
    }
    else
    {
        ui->textBoxDPPtSeed->setText(seed);
        on_pushButtonDPPtGenerate_clicked();
    }
}

SeedtoTime4::~SeedtoTime4()
{
    QSettings setting;
    setting.beginGroup("seedToTime4");
    setting.setValue("dpptYear", ui->lineEditDPPtYear->text());
    setting.setValue("minusDelayDPPt", ui->lineEditDPPtDelayMinus->text());
    setting.setValue("plusDelayDPPt", ui->lineEditDPPtDelayPlus->text());
    setting.setValue("minusSecondsDPPt", ui->lineEditDPPtSecondMinus->text());
    setting.setValue("plusSecondsDPPt", ui->lineEditDPPtSecondPlus->text());
    setting.setValue("hgssYear", ui->lineEditHGSSYear->text());
    setting.setValue("minusDelayHGSS", ui->lineEditHGSSDelayMinus->text());
    setting.setValue("plusDelayHGSS", ui->lineEditHGSSDelayPlus->text());
    setting.setValue("minusSecondsHGSS", ui->lineEditHGSSSecondMinus->text());
    setting.setValue("plusSecondsHGSS", ui->lineEditHGSSSecondPlus->text());
    setting.setValue("size", this->size());
    setting.endGroup();

    delete ui;
}

void SeedtoTime4::setupModels()
{
    dppt = new SeedtoTime4Model(ui->tableViewDPPtSearch, false);
    dpptCalibrate = new SeedtoTime4Model(ui->tableViewDPPtCalibrate, true);
    hgss = new SeedtoTime4Model(ui->tableViewHGSSSearch, false, Game::HeartGold);
    hgssCalibrate = new SeedtoTime4Model(ui->tableViewHGSSCalibrate, true, Game::HeartGold);

    ui->textBoxDPPtSeed->setValues(InputType::Seed32Bit);
    ui->textBoxHGSSSeed->setValues(InputType::Seed32Bit);

    ui->tableViewDPPtSearch->setModel(dppt);
    ui->tableViewDPPtCalibrate->setModel(dpptCalibrate);
    ui->tableViewHGSSSearch->setModel(hgss);
    ui->tableViewHGSSCalibrate->setModel(hgssCalibrate);

    QSettings setting;
    setting.beginGroup("seedToTime4");
    if (setting.contains("dpptYear")) ui->lineEditDPPtYear->setText(setting.value("dpptYear").toString());
    if (setting.contains("minusDelayDPPt")) ui->lineEditDPPtDelayMinus->setText(setting.value("minusDelayDPPt").toString());
    if (setting.contains("plusDelayDPPt")) ui->lineEditDPPtDelayPlus->setText(setting.value("plusDelayDPPt").toString());
    if (setting.contains("minusSecondsDPPt")) ui->lineEditDPPtSecondMinus->setText(setting.value("minusSecondsDPPt").toString());
    if (setting.contains("plusSecondsDPPt")) ui->lineEditDPPtSecondPlus->setText(setting.value("plusSecondsDPPt").toString());
    if (setting.contains("hgssYear")) ui->lineEditHGSSYear->setText(setting.value("hgssYear").toString());
    if (setting.contains("minusDelayHGSS")) ui->lineEditHGSSDelayMinus->setText(setting.value("minusDelayHGSS").toString());
    if (setting.contains("plusDelayHGSS")) ui->lineEditHGSSDelayPlus->setText(setting.value("plusDelayHGSS").toString());
    if (setting.contains("minusSecondsHGSS")) ui->lineEditHGSSSecondMinus->setText(setting.value("minusSecondsHGSS").toString());
    if (setting.contains("plusSecondsHGSS")) ui->lineEditHGSSSecondPlus->setText(setting.value("plusSecondsHGSS").toString());
    if (setting.contains("size")) this->resize(setting.value("size").toSize());
    setting.endGroup();
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

    u8 ab = seed >> 24;
    u8 cd = (seed >> 16) & 0xFF;
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

    QVector<bool> roamer = { ui->checkBoxHGSSRaikou->isChecked(), ui->checkBoxHGSSEntei->isChecked(), ui->checkBoxHGSSLati->isChecked() };
    QVector<u8> routes = { static_cast<u8>(ui->lineEditHGSSRaikou->text().toUInt()), static_cast<u8>(ui->lineEditHGSSEntei->text().toUInt()), static_cast<u8>(ui->lineEditHGSSLati->text().toUInt()) };

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
                    if (ab == ((month * day + minute + second) & 0xFF))
                    {
                        if (!forceSecond || second == forcedSecond)
                        {
                            QDateTime dateTime = QDateTime(QDate(static_cast<int>(year), month, day), QTime(static_cast<int>(hour), minute, second));
                            results.append(DateTime(dateTime, delay, version, roamer, routes));
                        }
                    }
                }
            }
        }
    }
    return results;
}

QVector<DateTime> SeedtoTime4::calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond, const DateTime &target)
{
    QDateTime time = target.getDateTime();
    u32 delay = target.getDelay();

    QVector<int> secondRange;
    QVector<int> delayRange;

    for (int i = minusDelay; i > 0; i--)
    {
        delayRange.append(-i);
    }
    for (int i = 0; i <= plusDelay; i++)
    {
        delayRange.append(i);
    }

    for (int i = minusSecond; i > 0; i--)
    {
        secondRange.append(-i);
    }
    for (int i = 0; i <= plusSecond; i++)
    {
        secondRange.append(i);
    }

    QVector<DateTime> results;
    for (int i : secondRange)
    {
        for (int j : delayRange)
        {
            QDateTime offset = time.addSecs(i);
            DateTime result = DateTime(offset, delay + j, target.getVersion(), target.getInfo());
            results.append(result);
        }
    }

    return results;
}

void SeedtoTime4::on_pushButtonDPPtGenerate_clicked()
{
    u32 seed = ui->textBoxDPPtSeed->getUInt();
    u32 year = ui->lineEditDPPtYear->text().toUInt();

    bool forceSecond = ui->checkBoxDPPtSecond->isChecked();
    int forcedSecond = ui->lineEditDPPtSecond->text().toInt();

    dppt->clearModel();

    QVector<DateTime> results = generate(seed, year, forceSecond, forcedSecond, Game::Diamond);
    ui->labelDPPtCoinFlips->setText(tr("Coin Flips: ") + Utilities::coinFlips(seed, 15));

    dppt->addItems(results);
}

void SeedtoTime4::on_pushButtonHGSSGenerate_clicked()
{
    hgss->clearModel();

    u32 seed = ui->textBoxHGSSSeed->getUInt();
    u32 year = ui->lineEditHGSSYear->text().toUInt();

    bool forceSecond = ui->checkBoxHGSSSecond->isChecked();
    int forcedSecond = ui->lineEditHGSSSecond->text().toInt();

    QVector<bool> roamer = { ui->checkBoxHGSSRaikou->isChecked(), ui->checkBoxHGSSEntei->isChecked(), ui->checkBoxHGSSLati->isChecked() };
    QVector<u8> routes = { static_cast<u8>(ui->lineEditHGSSRaikou->text().toUInt()), static_cast<u8>(ui->lineEditHGSSEntei->text().toUInt()), static_cast<u8>(ui->lineEditHGSSLati->text().toUInt()) };

    HGSSRoamer info(seed, roamer, routes);

    QVector<DateTime> results = generate(seed, year, forceSecond, forcedSecond, Game::HeartGold);
    ui->labelHGSSElmCalls->setText(tr("Elm Calls: ") + Utilities::getCalls(seed, 15, info));
    QString str = info.getRoutes();
    str = str.isEmpty() ? tr("No roamers") : str;
    ui->labelHGSSRoamers->setText(tr("Roamers: ") + str);

    hgss->addItems(results);
}

void SeedtoTime4::on_pushButtonDPPtCalibrate_clicked()
{
    int minusDelay = ui->lineEditDPPtDelayMinus->text().toInt();
    int plusDelay = ui->lineEditDPPtDelayPlus->text().toInt();

    int minusSecond = ui->lineEditDPPtSecondMinus->text().toInt();
    int plusSecond = ui->lineEditDPPtSecondPlus->text().toInt();

    QModelIndex index = ui->tableViewDPPtSearch->currentIndex();

    if (!index.isValid())
    {
        QMessageBox error;
        error.setText("Please select a result from Seed to Time.");
        error.exec();
        return;
    }

    dpptCalibrate->clearModel();

    DateTime target = dppt->getItem(index.row());
    QVector<DateTime> results = calibrate(minusDelay, plusDelay, minusSecond, plusSecond, target);

    dpptCalibrate->addItems(results);

    int count = (results.size() - 1) / 2;
    QModelIndex scroll = dpptCalibrate->index(count, 0);
    ui->tableViewDPPtCalibrate->setCurrentIndex(scroll);
    ui->tableViewDPPtCalibrate->scrollTo(scroll);
    ui->tableViewDPPtCalibrate->setFocus();
}

void SeedtoTime4::on_pushButtonHGSSCalibrate_clicked()
{
    int minusDelay = ui->lineEditHGSSDelayMinus->text().toInt();
    int plusDelay = ui->lineEditHGSSDelayPlus->text().toInt();

    int minusSecond = ui->lineEditHGSSSecondMinus->text().toInt();
    int plusSecond = ui->lineEditHGSSSecondPlus->text().toInt();

    QModelIndex index = ui->tableViewHGSSSearch->currentIndex();

    if (!index.isValid())
    {
        QMessageBox error;
        error.setText("Please select a result from Seed to Time.");
        error.exec();
        return;
    }

    hgssCalibrate->clearModel();

    DateTime target = hgss->getItem(index.row());
    QVector<DateTime> results = calibrate(minusDelay, plusDelay, minusSecond, plusSecond, target);

    hgssCalibrate->addItems(results);

    int count = (results.size() - 1) / 2;
    QModelIndex scroll = hgssCalibrate->index(count, 0);
    ui->tableViewHGSSCalibrate->setCurrentIndex(scroll);
    ui->tableViewHGSSCalibrate->scrollTo(scroll);
    ui->tableViewHGSSCalibrate->setFocus();
}

void SeedtoTime4::on_pushButtonDPPtSearchFlips_clicked()
{
    if (dpptCalibrate->rowCount() == 0)
    {
        return;
    }

    QScopedPointer<SearchCoinFlips> search(new SearchCoinFlips(dpptCalibrate->getModel()));
    if (search->exec() == QDialog::Rejected)
    {
        return;
    }

    QVector<bool> results = search->possibleResults();

    ui->tableViewDPPtCalibrate->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableViewDPPtCalibrate->clearSelection();

    for (auto i = 0; i < results.size(); i++)
    {
        if (results.at(i))
        {
            ui->tableViewDPPtCalibrate->selectRow(i);
        }
    }

    ui->tableViewDPPtCalibrate->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewDPPtCalibrate->setFocus();
}

void SeedtoTime4::on_pushButtonHGSSSearchCalls_clicked()
{
    if (hgssCalibrate->rowCount() == 0)
    {
        return;
    }

    QVector<bool> roamer = { ui->checkBoxHGSSRaikou->isChecked(), ui->checkBoxHGSSEntei->isChecked(), ui->checkBoxHGSSLati->isChecked() };
    QVector<u8> routes = { static_cast<u8>(ui->lineEditHGSSRaikou->text().toUInt()), static_cast<u8>(ui->lineEditHGSSEntei->text().toUInt()), static_cast<u8>(ui->lineEditHGSSLati->text().toUInt()) };

    QScopedPointer<SearchCalls> search(new SearchCalls(hgssCalibrate->getModel(), roamer, routes));
    if (search->exec() == QDialog::Rejected)
    {
        return;
    }

    QVector<bool> results = search->possibleResults();

    ui->tableViewHGSSCalibrate->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableViewHGSSCalibrate->clearSelection();

    for (auto i = 0; i < results.size(); i++)
    {
        if (results.at(i))
        {
            ui->tableViewHGSSCalibrate->selectRow(i);
        }
    }

    ui->tableViewHGSSCalibrate->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewHGSSCalibrate->setFocus();
}

void SeedtoTime4::on_pushButtonHGSSMap_clicked()
{
    auto *map = new RoamerMap();
    map->show();
    map->raise();
}
