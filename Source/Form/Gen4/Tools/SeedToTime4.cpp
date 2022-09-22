/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "SeedToTime4.hpp"
#include "ui_SeedToTime4.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen4/HGSSRoamer.hpp>
#include <Core/Util/Utilities.hpp>
#include <Form/Gen4/Tools/RoamerMap.hpp>
#include <Form/Gen4/Tools/SearchCalls.hpp>
#include <Form/Gen4/Tools/SearchCoinFlips.hpp>
#include <Model/Gen4/SeedTimeModel4.hpp>
#include <QMessageBox>
#include <QSettings>

SeedToTime4::SeedToTime4(QWidget *parent) : QWidget(parent), ui(new Ui::SeedToTime4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    dpptModel = new SeedTimeModel4(ui->tableViewDPPtSearch);
    dpptCalibrateModel = new SeedTimeCalibrateModel4(ui->tableViewDPPtCalibrate, Game::DPPt);
    hgssModel = new SeedTimeModel4(ui->tableViewHGSSSearch);
    hgssCalibrateModel = new SeedTimeCalibrateModel4(ui->tableViewHGSSCalibrate, Game::HGSS);

    ui->textBoxDPPtSeed->setValues(InputType::Seed32Bit);
    ui->textBoxDPPtYear->setValues(2000, 2099, 4, 10);
    ui->textBoxDPPtSecond->setValues(0, 59, 2, 10);
    ui->textBoxHGSSSeed->setValues(InputType::Seed32Bit);
    ui->textBoxHGSSYear->setValues(2000, 2099, 4, 10);
    ui->textBoxHGSSSecond->setValues(0, 59, 2, 10);

    ui->tableViewDPPtSearch->setModel(dpptModel);
    ui->tableViewDPPtCalibrate->setModel(dpptCalibrateModel);
    ui->tableViewHGSSSearch->setModel(hgssModel);
    ui->tableViewHGSSCalibrate->setModel(hgssCalibrateModel);

    connect(ui->pushButtonDPPtGenerate, &QPushButton::clicked, this, &SeedToTime4::dpptGenerate);
    connect(ui->pushButtonDPPtCalibrate, &QPushButton::clicked, this, &SeedToTime4::dpptCalibrate);
    connect(ui->pushButtonHGSSGenerate, &QPushButton::clicked, this, &SeedToTime4::hgssGenerate);
    connect(ui->pushButtonHGSSCalibrate, &QPushButton::clicked, this, &SeedToTime4::hgssCalibrate);
    connect(ui->pushButtonDPPtSearchFlips, &QPushButton::clicked, this, &SeedToTime4::searchFlips);
    connect(ui->pushButtonHGSSSearchCalls, &QPushButton::clicked, this, &SeedToTime4::searchCalls);
    connect(ui->pushButtonHGSSMap, &QPushButton::clicked, this, &SeedToTime4::map);

    QSettings setting;
    setting.beginGroup("SeedToTime4");
    if (setting.contains("dpptYear"))
    {
        ui->textBoxDPPtYear->setText(setting.value("dpptYear").toString());
    }
    if (setting.contains("minusDelayDPPt"))
    {
        ui->lineEditDPPtDelayMinus->setText(setting.value("minusDelayDPPt").toString());
    }
    if (setting.contains("plusDelayDPPt"))
    {
        ui->lineEditDPPtDelayPlus->setText(setting.value("plusDelayDPPt").toString());
    }
    if (setting.contains("minusSecondsDPPt"))
    {
        ui->lineEditDPPtSecondMinus->setText(setting.value("minusSecondsDPPt").toString());
    }
    if (setting.contains("plusSecondsDPPt"))
    {
        ui->lineEditDPPtSecondPlus->setText(setting.value("plusSecondsDPPt").toString());
    }
    if (setting.contains("hgssYear"))
    {
        ui->textBoxHGSSYear->setText(setting.value("hgssYear").toString());
    }
    if (setting.contains("minusDelayHGSS"))
    {
        ui->lineEditHGSSDelayMinus->setText(setting.value("minusDelayHGSS").toString());
    }
    if (setting.contains("plusDelayHGSS"))
    {
        ui->lineEditHGSSDelayPlus->setText(setting.value("plusDelayHGSS").toString());
    }
    if (setting.contains("minusSecondsHGSS"))
    {
        ui->lineEditHGSSSecondMinus->setText(setting.value("minusSecondsHGSS").toString());
    }
    if (setting.contains("plusSecondsHGSS"))
    {
        ui->lineEditHGSSSecondPlus->setText(setting.value("plusSecondsHGSS").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

SeedToTime4::SeedToTime4(u32 seed, Game version, QWidget *parent) : SeedToTime4(parent)
{
    if ((version & Game::HGSS) != Game::None)
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->textBoxHGSSSeed->setText(QString::number(seed, 16));
        hgssGenerate();
    }
    else
    {
        ui->textBoxDPPtSeed->setText(QString::number(seed, 16));
        dpptGenerate();
    }
}

SeedToTime4::~SeedToTime4()
{
    QSettings setting;
    setting.beginGroup("SeedToTime4");
    setting.setValue("dpptYear", ui->textBoxDPPtYear->text());
    setting.setValue("minusDelayDPPt", ui->lineEditDPPtDelayMinus->text());
    setting.setValue("plusDelayDPPt", ui->lineEditDPPtDelayPlus->text());
    setting.setValue("minusSecondsDPPt", ui->lineEditDPPtSecondMinus->text());
    setting.setValue("plusSecondsDPPt", ui->lineEditDPPtSecondPlus->text());
    setting.setValue("hgssYear", ui->textBoxHGSSYear->text());
    setting.setValue("minusDelayHGSS", ui->lineEditHGSSDelayMinus->text());
    setting.setValue("plusDelayHGSS", ui->lineEditHGSSDelayPlus->text());
    setting.setValue("minusSecondsHGSS", ui->lineEditHGSSSecondMinus->text());
    setting.setValue("plusSecondsHGSS", ui->lineEditHGSSSecondPlus->text());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

std::vector<SeedTimeCalibrate4> SeedToTime4::calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond, Game version,
                                                       const SeedTime4 &target)
{
    DateTime time = target.getDateTime();
    u32 delay = target.getDelay();

    std::array<bool, 3> roamer
        = { ui->checkBoxHGSSRaikou->isChecked(), ui->checkBoxHGSSEntei->isChecked(), ui->checkBoxHGSSLati->isChecked() };
    std::array<u8, 3> routes
        = { static_cast<u8>(ui->lineEditHGSSRaikou->text().toUInt()), static_cast<u8>(ui->lineEditHGSSEntei->text().toUInt()),
            static_cast<u8>(ui->lineEditHGSSLati->text().toUInt()) };

    std::vector<SeedTimeCalibrate4> results;
    results.reserve((plusDelay - minusDelay + 1) * (plusSecond - minusSecond + 1));
    for (int secondOffset = minusSecond; secondOffset <= plusSecond; secondOffset++)
    {
        DateTime offset = time.addSecs(secondOffset);
        for (int delayOffset = minusDelay; delayOffset <= plusDelay; delayOffset++)
        {
            results.emplace_back(offset, delay + delayOffset, version, roamer, routes);
        }
    }

    return results;
}

std::vector<SeedTime4> SeedToTime4::generate(u32 seed, u32 year, bool forceSecond, int forcedSecond)
{
    u8 ab = seed >> 24;
    u8 cd = (seed >> 16) & 0xFF;
    u32 efgh = seed & 0xFFFF;

    // Allow overflow seeds by setting hour to 23 and adjusting for delay
    u32 hour = cd > 23 ? 23 : cd;
    u32 delay = cd > 23 ? (efgh + (2000 - year)) + ((cd - 23) * 0x10000) : efgh + (2000 - year);

    std::vector<SeedTime4> results;
    for (int month = 1; month <= 12; month++)
    {
        int maxDays = Date::daysInMonth(month, year);
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
                            results.emplace_back(DateTime(year, month, day, hour, minute, second), delay);
                        }
                    }
                }
            }
        }
    }
    return results;
}

void SeedToTime4::dpptCalibrate()
{
    int minusDelay = -ui->lineEditDPPtDelayMinus->text().toInt();
    int plusDelay = ui->lineEditDPPtDelayPlus->text().toInt();

    int minusSecond = -ui->lineEditDPPtSecondMinus->text().toInt();
    int plusSecond = ui->lineEditDPPtSecondPlus->text().toInt();

    QModelIndex index = ui->tableViewDPPtSearch->currentIndex();
    if (!index.isValid())
    {
        if (!index.isValid())
        {
            QMessageBox msg(QMessageBox::Warning, tr("Invalid selection"), tr("Select a valid row"));
            msg.exec();
            return;
        }
    }

    dpptCalibrateModel->clearModel();

    const SeedTime4 &target = dpptModel->getItem(index.row());
    auto results = calibrate(minusDelay, plusDelay, minusSecond, plusSecond, Game::DPPt, target);

    dpptCalibrateModel->addItems(results);

    int count = (results.size() - 1) / 2;
    QModelIndex scroll = dpptCalibrateModel->index(count, 0);
    ui->tableViewDPPtCalibrate->setCurrentIndex(scroll);
    ui->tableViewDPPtCalibrate->scrollTo(scroll);
    ui->tableViewDPPtCalibrate->setFocus();
}

void SeedToTime4::dpptGenerate()
{
    u32 seed = ui->textBoxDPPtSeed->getUInt();
    u32 year = ui->textBoxDPPtYear->getUInt();

    bool forceSecond = ui->checkBoxDPPtSecond->isChecked();
    int forcedSecond = ui->textBoxDPPtSecond->getInt();

    dpptModel->clearModel();

    auto results = generate(seed, year, forceSecond, forcedSecond);
    ui->labelDPPtCoinFlips->setText(tr("Coin Flips: ") + QString::fromStdString(Utilities4::coinFlips(seed)));

    dpptModel->addItems(results);
}

void SeedToTime4::hgssCalibrate()
{
    int minusDelay = -ui->lineEditHGSSDelayMinus->text().toInt();
    int plusDelay = ui->lineEditHGSSDelayPlus->text().toInt();

    int minusSecond = -ui->lineEditHGSSSecondMinus->text().toInt();
    int plusSecond = ui->lineEditHGSSSecondPlus->text().toInt();

    QModelIndex index = ui->tableViewHGSSSearch->currentIndex();
    if (!index.isValid())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid selection"), tr("Select a valid row"));
        msg.exec();
        return;
    }

    hgssCalibrateModel->clearModel();

    const SeedTime4 &target = hgssModel->getItem(index.row());
    auto results = calibrate(minusDelay, plusDelay, minusSecond, plusSecond, Game::HGSS, target);

    hgssCalibrateModel->addItems(results);

    int count = (results.size() - 1) / 2;
    QModelIndex scroll = hgssCalibrateModel->index(count, 0);
    ui->tableViewHGSSCalibrate->setCurrentIndex(scroll);
    ui->tableViewHGSSCalibrate->scrollTo(scroll);
    ui->tableViewHGSSCalibrate->setFocus();
}

void SeedToTime4::hgssGenerate()
{
    hgssModel->clearModel();

    u32 seed = ui->textBoxHGSSSeed->getUInt();
    u32 year = ui->textBoxHGSSYear->getUInt();

    bool forceSecond = ui->checkBoxHGSSSecond->isChecked();
    int forcedSecond = ui->textBoxHGSSSecond->getInt();

    std::array<bool, 3> roamers
        = { ui->checkBoxHGSSRaikou->isChecked(), ui->checkBoxHGSSEntei->isChecked(), ui->checkBoxHGSSLati->isChecked() };
    std::array<u8, 3> routes
        = { static_cast<u8>(ui->lineEditHGSSRaikou->text().toUInt()), static_cast<u8>(ui->lineEditHGSSEntei->text().toUInt()),
            static_cast<u8>(ui->lineEditHGSSLati->text().toUInt()) };

    HGSSRoamer roamer(seed, roamers, routes);

    ui->labelHGSSElmCalls->setText(tr("Elm Calls: ") + QString::fromStdString(Utilities4::getCalls(seed, roamer.getSkips())));
    std::string str = roamer.getRouteString();
    ui->labelHGSSRoamers->setText(tr("Roamers: ") + (str.empty() ? tr("No roamers") : QString::fromStdString(str)));

    auto results = generate(seed, year, forceSecond, forcedSecond);
    hgssModel->addItems(results);
}

void SeedToTime4::map() const
{
    auto *roamerMap = new RoamerMap();
    roamerMap->show();
}

void SeedToTime4::searchCalls()
{
    if (hgssCalibrateModel->rowCount() == 0)
    {
        return;
    }

    std::unique_ptr<SearchCalls> search(new SearchCalls(hgssCalibrateModel->getModel()));
    if (search->exec() == QDialog::Rejected)
    {
        return;
    }

    ui->tableViewHGSSCalibrate->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableViewHGSSCalibrate->clearSelection();

    auto results = search->getResults();
    for (size_t i = 0; i < results.size(); i++)
    {
        if (results[i])
        {
            ui->tableViewHGSSCalibrate->selectRow(i);
        }
    }

    ui->tableViewHGSSCalibrate->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewHGSSCalibrate->setFocus();
}

void SeedToTime4::searchFlips()
{
    if (dpptCalibrateModel->rowCount() == 0)
    {
        return;
    }

    std::unique_ptr<SearchCoinFlips> search(new SearchCoinFlips(dpptCalibrateModel->getModel()));
    if (search->exec() == QDialog::Rejected)
    {
        return;
    }

    ui->tableViewDPPtCalibrate->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableViewDPPtCalibrate->clearSelection();

    auto results = search->getResults();
    for (size_t i = 0; i < results.size(); i++)
    {
        if (results[i])
        {
            ui->tableViewDPPtCalibrate->selectRow(i);
        }
    }

    ui->tableViewDPPtCalibrate->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewDPPtCalibrate->setFocus();
}
