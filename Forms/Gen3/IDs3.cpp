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

#include "IDs3.hpp"
#include "ui_IDs3.h"

IDs3::IDs3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IDs3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

IDs3::~IDs3()
{
    delete ui;
    delete xdcolo;
    delete frlge;
    delete rs;
}

void IDs3::setupModels()
{
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
    ui->tableViewXDColo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    frlge->setHorizontalHeaderLabels(QStringList() << tr("Frame") << tr("TID") << tr("SID"));
    ui->tableViewFRLGE->setModel(frlge);
    ui->tableViewFRLGE->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    rs->setHorizontalHeaderLabels(QStringList() << tr("Frame") << tr("TID") << tr("SID"));
    ui->tableViewRS->setModel(rs);
    ui->tableViewRS->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void IDs3::on_pushButtonFRLGESearch_clicked()
{
    frlge->removeRows(0, frlge->rowCount());

    u16 tid = ui->textBoxFRLGETID->text().toUShort();
    bool usePID = ui->checkBoxFRLGEPID->isChecked();
    bool useSID = ui->checkBoxFRLGESID->isChecked();
    u32 pid = ui->textBoxFRLGEPID->text().toUInt(nullptr, 16);
    u16 searchSID = ui->textBoxFRLGESID->text().toUShort();
    u32 minFrame = ui->textBoxFRLGEStartingFrame->text().toUInt();
    u32 maxResults = ui->textBoxFRLGEMaxResults->text().toUInt();

    PokeRNG rng(tid, minFrame - 1);
    u16 sid = rng.nextUShort();

    u32 max = minFrame + maxResults;
    for (u32 frame = minFrame; frame <= max; ++frame)
    {
        sid = rng.nextUShort();

        if ((!usePID || Utilities::shiny(pid, tid, sid)) && (!useSID || searchSID == sid))
        {
            frlge->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid)) << new QStandardItem(QString::number(sid)));
        }
    }
}

void IDs3::on_pushButtonRSSearch_clicked()
{
    rs->removeRows(0, rs->rowCount());

    u32 seed;
    bool usePID = ui->checkBoxRSPID->isChecked();
    bool useSID = ui->checkBoxRSSID->isChecked();
    bool useTID = ui->checkBoxRSTID->isChecked();
    u32 pid = ui->textBoxRSPID->text().toUInt(nullptr, 16);
    u16 searchSID = ui->textBoxRSSID->text().toUShort();
    u16 searchTID = ui->textBoxRSTID->text().toUShort();
    u32 minFrame = ui->textBoxRSStartingFrame->text().toUInt();
    u32 maxResults = ui->textBoxRSMaxResults->text().toUInt();

    if (ui->radioButtonRSInitialSeed->isChecked())
    {
        seed = ui->textBoxRSInitialSeed->text().toUInt(nullptr, 16);
    }
    else
    {
        seed = Utilities::calcGen3Seed(ui->dateTimeEdit->date(), ui->dateTimeEdit->time().hour(), ui->dateTimeEdit->time().minute());
    }

    PokeRNG rng(seed, minFrame);

    u16 tid = rng.nextUShort(), sid;

    u32 max = minFrame + maxResults;
    for (u32 frame = minFrame; frame <= max; ++frame)
    {
        sid = tid;
        tid = rng.nextUShort();

        if ((!usePID || Utilities::shiny(pid, tid, sid)) && (!useTID || searchTID == tid) && (!useSID || searchSID == sid))
        {
            rs->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid)) << new QStandardItem(QString::number(sid)));
        }
    }
}

void IDs3::on_pushButtonXDColoSearch_clicked()
{
    xdcolo->removeRows(0, xdcolo->rowCount());

    u32 seed = ui->textBoxXDColoSeed->text().toUInt(nullptr, 16);
    bool usePID = ui->checkBoxXDColoPID->isChecked();
    bool useSID = ui->checkBoxXDColoSID->isChecked();
    bool useTID = ui->checkBoxXDColoTID->isChecked();
    u32 pid = ui->checkBoxXDColoPID->text().toUInt(nullptr, 16);
    u16 searchSID = ui->textBoxXDColoSID->text().toUShort();
    u16 searchTID = ui->textBoxXDColoTID->text().toUShort();
    u32 minFrame = ui->textBoxXDColoStartingFrame->text().toUInt();
    u32 maxResults = ui->textBoxXDColoMaxResults->text().toUInt();

    XDRNG rng(seed, minFrame + 1);
    u16 sid = rng.nextUShort(), tid;

    u32 max = minFrame + maxResults;
    for (u32 frame = minFrame; frame <= max; ++frame)
    {
        tid = sid;
        sid = rng.nextUShort();

        if ((!usePID || Utilities::shiny(pid, tid, sid)) && (!useTID || searchTID == tid) && (!useSID || searchSID == sid))
        {
            xdcolo->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid)) << new QStandardItem(QString::number(sid)));
        }
    }
}

void IDs3::on_checkBoxRSDeadBattery_clicked(bool checked)
{
    ui->radioButtonRSDate->setEnabled(!checked);
    ui->radioButtonRSInitialSeed->setEnabled(!checked);
    ui->dateTimeEdit->setEnabled(!checked ? ui->radioButtonRSDate->isChecked() : false);
    ui->textBoxRSInitialSeed->setEnabled(!checked ? ui->radioButtonRSInitialSeed->isChecked() : false);
}

void IDs3::on_radioButtonRSDate_toggled(bool checked)
{
    ui->dateTimeEdit->setEnabled(checked);
}

void IDs3::on_radioButtonRSInitialSeed_toggled(bool checked)
{
    ui->textBoxRSInitialSeed->setEnabled(checked);
}
