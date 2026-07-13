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

#include "SHA1CacheFinder.hpp"
#include "ui_SHA1CacheFinder.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/IVCache.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/SHA1CacheSearcher.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

static const QString settingPrefix = QStringLiteral("sha1cachefinder");

SHA1CacheFinder::SHA1CacheFinder(QWidget *parent) : QWidget(parent), ui(new Ui::SHA1CacheFinder)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->profileDisplay->setup(settingPrefix, Game::Gen5);

    connect(ui->profileDisplay, &ProfileDisplay5::profileChanged, this, &SHA1CacheFinder::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay5::profilesChanged, this, &SHA1CacheFinder::profilesChanged);
    connect(ui->pushButtonOutputFile, &QPushButton::clicked, this, &SHA1CacheFinder::updateOutputFile);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &SHA1CacheFinder::search);

    updateProfiles();

    QSettings setting;
    setting.beginGroup(settingPrefix);
    if (setting.contains("startDate"))
    {
        ui->dateEditStartDate->setDate(setting.value("startDate").toDate());
    }
    if (setting.contains("endDate"))
    {
        ui->dateEditEndDate->setDate(setting.value("endDate").toDate());
    }
    setting.endGroup();
}

SHA1CacheFinder::~SHA1CacheFinder()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("startDate", ui->dateEditStartDate->date());
    setting.setValue("endDate", ui->dateEditEndDate->date());
    setting.endGroup();

    delete ui;
}

bool SHA1CacheFinder::hasProfiles() const
{
    return ui->profileDisplay->hasProfiles();
}

void SHA1CacheFinder::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void SHA1CacheFinder::profileChanged(const Profile5 &profile)
{
    currentProfile = &profile;
}

void SHA1CacheFinder::search()
{
    if (ui->lineEditOutputFile->text().isEmpty())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing output file"), tr("Please select a file to save the results to"));
        msg.exec();
        return;
    }

    Date start = ui->dateEditStartDate->getDate();
    Date end = ui->dateEditEndDate->getDate();
    if (start > end)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid date range"), tr("Start date is after end date"));
        msg.exec();
        return;
    }

    IVCache ivCache(currentProfile->getIVCache());
    if (!ivCache.isValid())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid IV Cache"), tr("Profile does not have a valid IV Cache"));
        msg.exec();
        return;
    }

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    auto *searcher = new SHA1CacheSearcher(ivCache, *currentProfile, start, end);
    searcher->setMaxProgress(searcher->getMaxProgress());

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
        if (!searcher->cancelled())
        {
            searcher->writeResults(ui->lineEditOutputFile->text().toStdString());
        }
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void SHA1CacheFinder::updateOutputFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), "sha1cache (*.sha1cache)");
    ui->lineEditOutputFile->setText(fileName);
}
