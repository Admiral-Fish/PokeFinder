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

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    s3.setAttribute(Qt::WA_QuitOnClose, false);
    w3.setAttribute(Qt::WA_QuitOnClose, false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonStationary3_clicked()
{
    s3.show();
    s3.raise();
}

void MainWindow::on_actionResearcher_triggered()
{
    Researcher *r = new Researcher();
    r->setAttribute(Qt::WA_QuitOnClose, false);
    r->show();
}

void MainWindow::on_pushButtonWild3_clicked()
{
    w3.show();
    w3.raise();
}
