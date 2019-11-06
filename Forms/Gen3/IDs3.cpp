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

#include "IDs3.hpp"
#include "ui_IDs3.h"
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/Utilities.hpp>
#include <QSettings>

namespace PokeFinderForms
{
    IDs3::IDs3(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::IDs3)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);

        setupModels();
    }

    IDs3::~IDs3()
    {
        QSettings setting;
        setting.setValue("ids3/geometry", this->saveGeometry());

        delete ui;
    }

    void IDs3::setupModels()
    {
        xdcolo = new QStandardItemModel(ui->tableViewXDColo);
        frlge = new QStandardItemModel(ui->tableViewFRLGE);
        rs = new QStandardItemModel(ui->tableViewFRLGE);

        ui->textBoxFRLGEPID->setValues(InputType::Seed32Bit);
        ui->textBoxFRLGETID->setValues(InputType::TIDSID);
        ui->textBoxFRLGESID->setValues(InputType::TIDSID);
        ui->textBoxFRLGEStartingFrame->setValues(InputType::Frame32Bit);
        ui->textBoxFRLGEMaxResults->setValues(InputType::Frame32Bit);

        ui->textBoxRSPID->setValues(InputType::Seed32Bit);
        ui->textBoxRSTID->setValues(InputType::TIDSID);
        ui->textBoxRSSID->setValues(InputType::TIDSID);
        ui->textBoxRSInitialSeed->setValues(InputType::Seed16Bit);
        ui->textBoxRSStartingFrame->setValues(InputType::Frame32Bit);
        ui->textBoxRSMaxResults->setValues(InputType::Frame32Bit);

        ui->textBoxXDColoPID->setValues(InputType::Seed32Bit);
        ui->textBoxXDColoTID->setValues(InputType::TIDSID);
        ui->textBoxXDColoSID->setValues(InputType::TIDSID);
        ui->textBoxXDColoSeed->setValues(InputType::Seed32Bit);
        ui->textBoxXDColoStartingFrame->setValues(InputType::Frame32Bit);
        ui->textBoxXDColoMaxResults->setValues(InputType::Frame32Bit);

        ui->dateTimeEdit->setDisplayFormat(QLocale::system().dateTimeFormat(QLocale::ShortFormat));

        xdcolo->setHorizontalHeaderLabels(QStringList() << tr("Frame") << tr("TID") << tr("SID"));
        ui->tableViewXDColo->setModel(xdcolo);

        frlge->setHorizontalHeaderLabels(QStringList() << tr("Frame") << tr("TID") << tr("SID"));
        ui->tableViewFRLGE->setModel(frlge);

        rs->setHorizontalHeaderLabels(QStringList() << tr("Frame") << tr("TID") << tr("SID"));
        ui->tableViewRS->setModel(rs);

        connect(ui->pushButtonXDColoSearch, &QPushButton::clicked, this, &IDs3::xdColoSearch);
        connect(ui->pushButtonFRLGESearch, &QPushButton::clicked, this, &IDs3::frlgeSearch);
        connect(ui->pushButtonRSSearch, &QPushButton::clicked, this, &IDs3::rsSearch);
        connect(ui->checkBoxRSDeadBattery, &QCheckBox::clicked, this, &IDs3::rsDeadBattery);
        connect(ui->radioButtonRSDate, &QRadioButton::toggled, this, &IDs3::rsDate);
        connect(ui->radioButtonRSInitialSeed, &QRadioButton::toggled, this, &IDs3::rsInitialSeed);

        QSettings setting;
        if (setting.contains("ids3/geometry"))
        {
            this->restoreGeometry(setting.value("ids3/geometry").toByteArray());
        }
    }

    void IDs3::xdColoSearch()
    {
        frlge->removeRows(0, frlge->rowCount());

        u16 tid = ui->textBoxFRLGETID->getUShort();
        bool usePID = ui->checkBoxFRLGEPID->isChecked();
        bool useSID = ui->checkBoxFRLGESID->isChecked();
        u32 pid = ui->textBoxFRLGEPID->getUInt();
        u16 searchSID = ui->textBoxFRLGESID->getUShort();
        u32 minFrame = ui->textBoxFRLGEStartingFrame->getUInt();
        u32 maxResults = ui->textBoxFRLGEMaxResults->getUInt();

        PokeFinderCore::PokeRNG rng(tid, minFrame - 1);
        rng.nextUShort();

        u32 max = minFrame + maxResults;
        for (u32 frame = minFrame; frame <= max; ++frame)
        {
            u16 sid = rng.nextUShort();

            if ((!usePID || PokeFinderCore::Utilities::shiny(pid, tid, sid)) && (!useSID || searchSID == sid))
            {
                frlge->appendRow(QList<QStandardItem *>()
                    << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid))
                    << new QStandardItem(QString::number(sid)));
            }
        }
    }

    void IDs3::frlgeSearch()
    {
        rs->removeRows(0, rs->rowCount());

        u32 seed;
        bool usePID = ui->checkBoxRSPID->isChecked();
        bool useSID = ui->checkBoxRSSID->isChecked();
        bool useTID = ui->checkBoxRSTID->isChecked();
        u32 pid = ui->textBoxRSPID->getUInt();
        u16 searchSID = ui->textBoxRSSID->getUShort();
        u16 searchTID = ui->textBoxRSTID->getUShort();
        u32 minFrame = ui->textBoxRSStartingFrame->getUInt();
        u32 maxResults = ui->textBoxRSMaxResults->getUInt();

        if (ui->radioButtonRSInitialSeed->isChecked())
        {
            seed = ui->textBoxRSInitialSeed->getUInt();
        }
        else
        {
            seed = PokeFinderCore::Utilities::calcGen3Seed(
                ui->dateTimeEdit->date(), ui->dateTimeEdit->time().hour(), ui->dateTimeEdit->time().minute());
        }

        PokeFinderCore::PokeRNG rng(seed, minFrame);

        u16 tid = rng.nextUShort(), sid;

        u32 max = minFrame + maxResults;
        for (u32 frame = minFrame; frame <= max; ++frame)
        {
            sid = tid;
            tid = rng.nextUShort();

            if ((!usePID || PokeFinderCore::Utilities::shiny(pid, tid, sid)) && (!useTID || searchTID == tid)
                && (!useSID || searchSID == sid))
            {
                rs->appendRow(QList<QStandardItem *>()
                    << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid))
                    << new QStandardItem(QString::number(sid)));
            }
        }
    }

    void IDs3::rsSearch()
    {
        xdcolo->removeRows(0, xdcolo->rowCount());

        u32 seed = ui->textBoxXDColoSeed->getUInt();
        bool usePID = ui->checkBoxXDColoPID->isChecked();
        bool useSID = ui->checkBoxXDColoSID->isChecked();
        bool useTID = ui->checkBoxXDColoTID->isChecked();
        u32 pid = ui->textBoxXDColoPID->getUInt();
        u16 searchSID = ui->textBoxXDColoSID->getUShort();
        u16 searchTID = ui->textBoxXDColoTID->getUShort();
        u32 minFrame = ui->textBoxXDColoStartingFrame->getUInt();
        u32 maxResults = ui->textBoxXDColoMaxResults->getUInt();

        PokeFinderCore::XDRNG rng(seed, minFrame + 1);
        u16 sid = rng.nextUShort(), tid;

        u32 max = minFrame + maxResults;
        for (u32 frame = minFrame; frame <= max; ++frame)
        {
            tid = sid;
            sid = rng.nextUShort();

            if ((!usePID || PokeFinderCore::Utilities::shiny(pid, tid, sid)) && (!useTID || searchTID == tid)
                && (!useSID || searchSID == sid))
            {
                xdcolo->appendRow(QList<QStandardItem *>()
                    << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid))
                    << new QStandardItem(QString::number(sid)));
            }
        }
    }

    void IDs3::rsDeadBattery(bool checked)
    {
        ui->radioButtonRSDate->setEnabled(!checked);
        ui->radioButtonRSInitialSeed->setEnabled(!checked);
        ui->dateTimeEdit->setEnabled(!checked ? ui->radioButtonRSDate->isChecked() : false);
        ui->textBoxRSInitialSeed->setEnabled(!checked ? ui->radioButtonRSInitialSeed->isChecked() : false);
    }

    void IDs3::rsDate(bool checked) { ui->dateTimeEdit->setEnabled(checked); }

    void IDs3::rsInitialSeed(bool checked) { ui->textBoxRSInitialSeed->setEnabled(checked); }
}
