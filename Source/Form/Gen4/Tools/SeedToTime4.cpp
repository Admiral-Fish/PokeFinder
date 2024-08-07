/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen4/Tools/SeedToTimeCalculator4.hpp>
#include <Core/Util/Utilities.hpp>
#include <Form/Gen4/Tools/RoamerMap.hpp>
#include <Form/Gen4/Tools/SearchCalls.hpp>
#include <Form/Gen4/Tools/SearchCoinFlips.hpp>
#include <Model/Gen4/SeedToTimeModel4.hpp>
#include <QMessageBox>
#include <QSettings>

SeedToTime4::SeedToTime4(QWidget *parent) : QWidget(parent), ui(new Ui::SeedToTime4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    dpptModel = new SeedToTimeModel4(ui->tableViewDPPtSearch);
    dpptCalibrateModel = new SeedToTimeCalibrateModel4(ui->tableViewDPPtCalibrate, true);
    hgssModel = new SeedToTimeModel4(ui->tableViewHGSSSearch);
    hgssCalibrateModel = new SeedToTimeCalibrateModel4(ui->tableViewHGSSCalibrate, false);

    ui->textBoxDPPtSeed->setValues(InputType::Seed32Bit);
    ui->textBoxDPPtYear->setValues(2000, 2099, 4, 10);
    ui->textBoxDPPtSecond->setValues(0, 59, 2, 10);
    ui->textBoxDPPtDelayCalibration->setValues(InputType::Advance32Bit);
    ui->textBoxDPPtSecondCalibration->setValues(0, 500, 3, 10);

    ui->textBoxHGSSSeed->setValues(InputType::Seed32Bit);
    ui->textBoxHGSSYear->setValues(2000, 2099, 4, 10);
    ui->textBoxHGSSSecond->setValues(0, 59, 2, 10);
    ui->textBoxHGSSDelayCalibration->setValues(InputType::Advance32Bit);
    ui->textBoxHGSSSecondCalibration->setValues(0, 500, 3, 10);
    ui->textBoxHGSSRaikou->setValues(0, 46, 2, 10);
    ui->textBoxHGSSEntei->setValues(0, 46, 2, 10);
    ui->textBoxHGSSLati->setValues(0, 28, 2, 10);

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
    connect(ui->checkBoxHGSSEntei, &QCheckBox::clicked, ui->textBoxHGSSEntei, &QLineEdit::setEnabled);
    connect(ui->checkBoxHGSSLati, &QCheckBox::clicked, ui->textBoxHGSSLati, &QLineEdit::setEnabled);
    connect(ui->checkBoxHGSSRaikou, &QCheckBox::clicked, ui->textBoxHGSSRaikou, &QLineEdit::setEnabled);
    connect(ui->checkBoxDPPtSecond, &QCheckBox::clicked, ui->textBoxDPPtSecond, &TextBox::setEnabled);
    connect(ui->checkBoxHGSSSecond, &QCheckBox::clicked, ui->textBoxHGSSSecond, &TextBox::setEnabled);
    connect(ui->pushButtonHGSSMap, &QPushButton::clicked, this, &SeedToTime4::map);

    QSettings setting;
    setting.beginGroup("SeedToTime4");
    if (setting.contains("dpptYear"))
    {
        ui->textBoxDPPtYear->setText(setting.value("dpptYear").toString());
    }
    if (setting.contains("delayCalibrationDPPt"))
    {
        ui->textBoxDPPtDelayCalibration->setText(setting.value("delayCalibrationDPPt").toString());
    }
    if (setting.contains("secondCalibrationDPPt"))
    {
        ui->textBoxDPPtSecondCalibration->setText(setting.value("secondCalibrationDPPt").toString());
    }
    if (setting.contains("hgssYear"))
    {
        ui->textBoxHGSSYear->setText(setting.value("hgssYear").toString());
    }
    if (setting.contains("delayCalibrationHGSS"))
    {
        ui->textBoxHGSSDelayCalibration->setText(setting.value("delayCalibrationHGSS").toString());
    }
    if (setting.contains("secondCalibrationHGSS"))
    {
        ui->textBoxHGSSSecondCalibration->setText(setting.value("secondCalibrationHGSS").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

SeedToTime4::SeedToTime4(u32 seed, Game version, QWidget *parent) : SeedToTime4(parent)
{
    if ((version & Game::Gen4) != Game::None)
    {
        ui->textBoxHGSSSeed->setText(QString::number(seed, 16));
        hgssGenerate();

        ui->textBoxDPPtSeed->setText(QString::number(seed, 16));
        dpptGenerate();
    }
    else if ((version & Game::HGSS) != Game::None)
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
    setting.setValue("delayCalibrationDPPt", ui->textBoxDPPtDelayCalibration->text());
    setting.setValue("secondCalibrationDPPt", ui->textBoxDPPtSecondCalibration->text());
    setting.setValue("hgssYear", ui->textBoxHGSSYear->text());
    setting.setValue("delayCalibrationHGSS", ui->textBoxHGSSDelayCalibration->text());
    setting.setValue("secondCalibrationHGSS", ui->textBoxHGSSSecondCalibration->text());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void SeedToTime4::dpptCalibrate()
{
    int delayCalibration = ui->textBoxDPPtDelayCalibration->getInt();
    int secondCalibration = ui->textBoxDPPtSecondCalibration->getInt();

    QModelIndex index = ui->tableViewDPPtSearch->currentIndex();
    if (!index.isValid())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid selection"), tr("Select a valid row"));
        msg.exec();
        return;
    }

    const SeedTime4 &target = dpptModel->getItem(index.row());

    dpptCalibrateModel->clearModel();
    auto results = SeedToTimeCalculator4::calibrate(delayCalibration, secondCalibration, target);
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
    u16 year = ui->textBoxDPPtYear->getUShort();

    bool forceSecond = ui->checkBoxDPPtSecond->isChecked();
    u8 forcedSecond = ui->textBoxDPPtSecond->getUChar();

    dpptModel->clearModel();

    auto results = SeedToTimeCalculator4::calculateTimes(seed, year, forceSecond, forcedSecond);
    ui->labelDPPtCoinFlips->setText(tr("Coin Flips: %1").arg(QString::fromStdString(Utilities4::coinFlips(seed))));

    dpptModel->addItems(results);
}

void SeedToTime4::hgssCalibrate()
{
    int delayCalibration = ui->textBoxHGSSDelayCalibration->getInt();
    int secondCalibration = ui->textBoxHGSSSecondCalibration->getInt();

    QModelIndex index = ui->tableViewHGSSSearch->currentIndex();
    if (!index.isValid())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid selection"), tr("Select a valid row"));
        msg.exec();
        return;
    }

    std::array<bool, 3> roamers
        = { ui->checkBoxHGSSRaikou->isChecked(), ui->checkBoxHGSSEntei->isChecked(), ui->checkBoxHGSSLati->isChecked() };
    std::array<u8, 3> routes = { ui->textBoxHGSSRaikou->getUChar(), ui->textBoxHGSSEntei->getUChar(), ui->textBoxHGSSLati->getUChar() };
    const SeedTime4 &target = hgssModel->getItem(index.row());

    hgssCalibrateModel->clearModel();
    auto results = SeedToTimeCalculator4::calibrate(delayCalibration, secondCalibration, roamers, routes, target);
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
    u16 year = ui->textBoxHGSSYear->getUShort();

    bool forceSecond = ui->checkBoxHGSSSecond->isChecked();
    u8 forcedSecond = ui->textBoxHGSSSecond->getUChar();

    std::array<bool, 3> roamers
        = { ui->checkBoxHGSSRaikou->isChecked(), ui->checkBoxHGSSEntei->isChecked(), ui->checkBoxHGSSLati->isChecked() };
    std::array<u8, 3> routes = { ui->textBoxHGSSRaikou->getUChar(), ui->textBoxHGSSEntei->getUChar(), ui->textBoxHGSSLati->getUChar() };

    HGSSRoamer roamer(seed, roamers, routes);

    ui->labelHGSSElmCalls->setText(tr("Elm Calls: %1").arg(QString::fromStdString(Utilities4::getCalls(seed, roamer.getSkips()))));
    std::string str = roamer.getRouteString();
    ui->labelHGSSRoamers->setText(tr("Roamers: %1").arg(str.empty() ? tr("No roamers") : QString::fromStdString(str)));

    auto results = SeedToTimeCalculator4::calculateTimes(seed, year, forceSecond, forcedSecond);
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
    if (search->exec() == QDialog::Accepted)
    {
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
}

void SeedToTime4::searchFlips()
{
    if (dpptCalibrateModel->rowCount() == 0)
    {
        return;
    }

    std::unique_ptr<SearchCoinFlips> search(new SearchCoinFlips(dpptCalibrateModel->getModel()));
    if (search->exec() == QDialog::Accepted)
    {
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
}
