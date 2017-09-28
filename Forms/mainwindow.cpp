/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generate_clicked()
{
    bool pass;
    QMessageBox error;
    QString input;

    uint32_t seed = 0;
    input = ui->initialSeed->text();
    if (input != "")
    {
        seed = input.toUInt(&pass, 16);
        if (!pass)
        {
            error.setText("Please enter seed in valid hexadecimal format.");
            error.exec();
            return;
        }
    }

    uint32_t startingFrame = 1;
    input = ui->startingFrame->text();
    if (input != "")
    {
        startingFrame = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter starting frame in valid decimal format.");
            error.exec();
            return;
        }
        else if (startingFrame > 0xffffffff)
        {
            error.setText("Please enter a value lower then 4294967296.");
            error.exec();
            return;
        }
    }

    uint32_t maxResults = 100000;
    input = ui->maxResults->text();
    if (input != "")
    {
        maxResults = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter max results in valid decimal format.");
            error.exec();
            return;
        }
        else if (maxResults > 0xffffffff)
        {
            error.setText("Please enter a value lower then 4294967296.");
            error.exec();
            return;
        }
    }

    uint32_t tid = 12345;
    input = ui->id->text();
    if (input != "")
    {
        tid = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter Trainer ID in valid decimal format.");
            error.exec();
            return;
        }
        else if (tid > 0xffff)
        {
            error.setText("Please enter a value lower then 65536.");
            error.exec();
            return;
        }
    }

    uint32_t sid = 54321;
    input = ui->sid->text();
    if (input != "")
    {
        sid = input.toUInt(&pass, 10);
        if (!pass)
        {
            error.setText("Please enter Trainer SID in valid decimal format.");
            error.exec();
            return;
        }
        else if (sid > 0xffff)
        {
            error.setText("Please enter a value lower then 65536.");
            error.exec();
            return;
        }
    }

    //ui->tableView->clearContents();
    GeneratorGen3 generator = GeneratorGen3(maxResults, startingFrame, seed, tid, sid);
    int method = ui->comboBoxMethod->currentIndex();

    if (method == 0)
        generator.frameType = Method1;
    else if (method == 1)
        generator.frameType = Method2;
    else if (method == 2)
        generator.frameType = Method4;
    else if (method == 3)
        generator.frameType = XDColo;
    else
        generator.frameType = Channel;

    std::vector<FrameGen3> frames = generator.Generate();
    int size = frames.size();
    //ui->tableView->setRowCount(size);

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"Frame", "Time", "PID", "!!!", "Nature", "Ability", "HP", "Atk", "Def", "SpA", "SpD", "Spe", "Hidden", "Power", "12.5% Female", "25% Female", "50% Female", "75% Female"});

    for (int i = 0; i < size; i++)
    {
        model->appendRow(frames[i].GetTableRow());
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}
