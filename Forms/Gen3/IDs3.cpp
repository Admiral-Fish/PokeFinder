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

void IDs3::changeEvent(QEvent *event)
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

void IDs3::setupModels()
{
    ui->textBoxPIDFRLGE->setValues(0, 32, false);
    ui->textBoxTIDFRLGE->setValues(0, 48, true);
    ui->textBoxSIDFRLGE->setValues(0, 48, true);
    ui->textBoxMinFrameFRLGE->setValues(0, 32, true);
    ui->textBoxMaxFrameFRLGE->setValues(0, 32, true);

    ui->textBoxPIDRS->setValues(0, 32, false);
    ui->textBoxTIDRS->setValues(0, 48, true);
    ui->textBoxSIDRS->setValues(0, 48, true);
    ui->textBoxInitSeedRS->setValues(0, 48, false);
    ui->textBoxMinFrameRS->setValues(0, 32, true);
    ui->textBoxMaxFrameRS->setValues(0, 32, true);

    ui->textBoxPIDXD->setValues(0, 32, false);
    ui->textBoxTIDXD->setValues(0, 48, true);
    ui->textBoxSIDXD->setValues(0, 48, true);
    ui->textBoxSeedXD->setValues(0, 48, false);
    ui->textBoxMinFrameXD->setValues(0, 32, true);
    ui->textBoxMaxFrameXD->setValues(0, 32, true);

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

void IDs3::on_pushButtonFindFRLGE_clicked()
{
    frlge->removeRows(0, frlge->rowCount());

    u32 tid = ui->textBoxTIDFRLGE->text().toUInt();
    bool usePID = ui->checkBoxPIDFRLGE->isChecked();
    bool useSID = ui->checkBoxSIDFRLGE->isChecked();
    u32 pid = ui->textBoxPIDFRLGE->text().toUInt(nullptr, 16);
    u32 searchSID = ui->textBoxSIDFRLGE->text().toUInt();

    LCRNG rng = PokeRNG(tid);

    u32 minFrame = ui->textBoxMinFrameFRLGE->text().toUInt();
    u32 maxResults = ui->textBoxMaxFrameFRLGE->text().toUInt();

    rng.advanceFrames(minFrame - 1);

    u32 sid = rng.nextUShort();

    u32 max = minFrame + maxResults;
    for (u32 frame = minFrame; frame <= max; ++frame)
    {
        sid = rng.nextUShort();

        if ((!usePID || Utilities::shiny(pid, tid, sid)) && (!useSID || searchSID == sid))
            frlge->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid)) << new QStandardItem(QString::number(sid)));
    }
}

void IDs3::on_pushButtonFindRS_clicked()
{
    rs->removeRows(0, rs->rowCount());

    u32 seed;
    bool usePID = ui->checkBoxPIDRS->isChecked();
    bool useSID = ui->checkBoxSIDRS->isChecked();
    bool useTID = ui->checkBoxTIDRS->isChecked();
    u32 pid = ui->textBoxPIDRS->text().toUInt(nullptr, 16);
    u32 searchSID = ui->textBoxSIDRS->text().toUInt();
    u32 searchTID = ui->textBoxTIDRS->text().toUInt();

    u32 minFrame = ui->textBoxMinFrameRS->text().toUInt();
    u32 maxResults = ui->textBoxMaxFrameRS->text().toUInt();

    if (ui->radioButtonInitSeedRS->isChecked())
        seed = ui->textBoxInitSeedRS->text().toUInt(nullptr, 16);
    else
        seed = Utilities::calcGen3Seed(ui->dateTimeEdit->date(), static_cast<u32>(ui->dateTimeEdit->time().hour()), static_cast<u32>(ui->dateTimeEdit->time().minute()));

    LCRNG rng = PokeRNG(seed);

    rng.advanceFrames(minFrame);

    u32 tid = rng.nextUShort();
    u32 sid;

    u32 max = minFrame + maxResults;
    for (u32 frame = minFrame; frame <= max; ++frame)
    {
        sid = tid;
        tid = rng.nextUShort();

        if ((!usePID || Utilities::shiny(pid, tid, sid)) && (!useTID || searchTID == tid) && (!useSID || searchSID == sid))
            rs->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid)) << new QStandardItem(QString::number(sid)));
    }
}

void IDs3::on_pushButtonFindXD_clicked()
{
    xdcolo->removeRows(0, xdcolo->rowCount());

    u32 seed = ui->textBoxSeedXD->text().toUInt(nullptr, 16);
    bool usePID = ui->checkBoxPIDXD->isChecked();
    bool useSID = ui->checkBoxSIDXD->isChecked();
    bool useTID = ui->checkBoxTIDXD->isChecked();
    u32 pid = ui->textBoxPIDXD->text().toUInt(nullptr, 16);
    u32 searchSID = ui->textBoxSIDXD->text().toUInt();
    u32 searchTID = ui->textBoxTIDXD->text().toUInt();

    u32 minFrame = ui->textBoxMinFrameXD->text().toUInt();
    u32 maxResults = ui->textBoxMaxFrameXD->text().toUInt();

    LCRNG rng = XDRNG(seed);

    rng.advanceFrames(minFrame + 1);

    u32 sid = rng.nextUShort();
    u32 tid;

    u32 max = minFrame + maxResults;
    for (u32 frame = minFrame; frame <= max; ++frame)
    {
        tid = sid;
        sid = rng.nextUShort();

        if ((!usePID || Utilities::shiny(pid, tid, sid)) && (!useTID || searchTID == tid) && (!useSID || searchSID == sid))
            xdcolo->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(frame)) << new QStandardItem(QString::number(tid)) << new QStandardItem(QString::number(sid)));
    }
}

void IDs3::on_checkBoxBattery_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked)
    {
        ui->radioButtonDateRS->setEnabled(true);
        ui->radioButtonInitSeedRS->setEnabled(true);
        on_radioButtonDateRS_toggled(ui->radioButtonDateRS->isChecked());
        on_radioButtonInitSeedRS_toggled(ui->radioButtonInitSeedRS->isChecked());
    }
    else
    {
        ui->radioButtonDateRS->setEnabled(false);
        ui->radioButtonInitSeedRS->setEnabled(false);
        on_radioButtonDateRS_toggled(false);
        on_radioButtonInitSeedRS_toggled(false);
    }
}

void IDs3::on_checkBoxPIDRS_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->textBoxPIDRS->setEnabled(true);
    else
        ui->textBoxPIDRS->setEnabled(false);
}

void IDs3::on_checkBoxTIDRS_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->textBoxTIDRS->setEnabled(true);
    else
        ui->textBoxTIDRS->setEnabled(false);
}

void IDs3::on_checkBoxSIDRS_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->textBoxSIDRS->setEnabled(true);
    else
        ui->textBoxSIDRS->setEnabled(false);
}

void IDs3::on_checkBoxPIDXD_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->textBoxPIDXD->setEnabled(true);
    else
        ui->textBoxPIDXD->setEnabled(false);
}

void IDs3::on_checkBoxTIDXD_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->textBoxTIDXD->setEnabled(true);
    else
        ui->textBoxTIDXD->setEnabled(false);
}

void IDs3::on_checkBoxSIDXD_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->textBoxSIDXD->setEnabled(true);
    else
        ui->textBoxSIDXD->setEnabled(false);
}

void IDs3::on_checkBoxPIDFRLGE_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->textBoxPIDFRLGE->setEnabled(true);
    else
        ui->textBoxPIDFRLGE->setEnabled(false);
}

void IDs3::on_checkBoxSIDFRLGE_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->textBoxSIDFRLGE->setEnabled(true);
    else
        ui->textBoxSIDFRLGE->setEnabled(false);
}

void IDs3::on_radioButtonDateRS_toggled(bool checked)
{
    if (checked)
        ui->dateTimeEdit->setEnabled(true);
    else
        ui->dateTimeEdit->setEnabled(false);
}

void IDs3::on_radioButtonInitSeedRS_toggled(bool checked)
{
    if (checked)
        ui->textBoxInitSeedRS->setEnabled(true);
    else
        ui->textBoxInitSeedRS->setEnabled(false);
}
