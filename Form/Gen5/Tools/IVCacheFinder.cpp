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

#include "IVCacheFinder.hpp"
#include "ui_IVCacheFinder.h"
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/IVCacheSearcher.hpp>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

IVCacheFinder::IVCacheFinder(QWidget *parent) : QWidget(parent), ui(new Ui::IVCacheFinder)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->textEditInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textEditMaxAdvances->setValues(InputType::Advance32Bit);

    connect(ui->pushButtonOutputFile, &QPushButton::clicked, this, &IVCacheFinder::updateOutputFile); 
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &IVCacheFinder::search);
}

IVCacheFinder::~IVCacheFinder()
{
    delete ui;
}

void IVCacheFinder::search()
{
    if (ui->lineEditOutputFile->text().isEmpty())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing output file"), tr("Please select a file to save the results to"));
        msg.exec();
        return;
    }

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    auto *searcher = new IVCacheSearcher(ui->textEditInitialAdvances->getUInt(), ui->textEditMaxAdvances->getUInt());
    searcher->setMaxProgress(0x100000000);

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] { ui->progressBar->setValue(searcher->getProgress()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        ui->progressBar->setValue(searcher->getProgress());
        searcher->writeResults(ui->lineEditOutputFile->text().toStdString());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void IVCacheFinder::updateOutputFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), "ivcache (*.ivcache)");
    ui->lineEditOutputFile->setText(fileName);
}
