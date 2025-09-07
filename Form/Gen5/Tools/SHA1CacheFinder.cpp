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

SHA1CacheFinder::SHA1CacheFinder(QWidget *parent) : QWidget(parent), ui(new Ui::SHA1CacheFinder)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &SHA1CacheFinder::profileIndexChanged);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &SHA1CacheFinder::profileManager);
    connect(ui->pushButtonOutputFile, &QPushButton::clicked, this, &SHA1CacheFinder::updateOutputFile);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &SHA1CacheFinder::search);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("sha1cachefinder");
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
    setting.beginGroup("sha1cachefinder");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("startDate", ui->dateEditStartDate->date());
    setting.setValue("endDate", ui->dateEditEndDate->date());
    setting.endGroup();

    delete ui;
}

bool SHA1CacheFinder::hasProfiles() const
{
    return !profiles.empty();
}

void SHA1CacheFinder::updateProfiles()
{
    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("sha1cachefinder/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void SHA1CacheFinder::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileMACAddressValue->setText(QString::number(currentProfile->getMac(), 16));
        ui->labelProfileDSTypeValue->setText(QString::fromStdString(currentProfile->getDSTypeString()));
        ui->labelProfileVCountValue->setText(QString::number(currentProfile->getVCount(), 16));
        ui->labelProfileTimer0Value->setText(QString::number(currentProfile->getTimer0Min(), 16) + "-"
                                             + QString::number(currentProfile->getTimer0Max(), 16));
        ui->labelProfileGxStatValue->setText(QString::number(currentProfile->getGxStat()));
        ui->labelProfileVFrameValue->setText(QString::number(currentProfile->getVFrame()));
        ui->labelProfileKeypressesValue->setText(QString::fromStdString(currentProfile->getKeypressesString()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));
    }
}

void SHA1CacheFinder::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
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

    int maxProgress = Keypresses::getKeypresses(*currentProfile).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= (currentProfile->getTimer0Max() - currentProfile->getTimer0Min() + 1);
    searcher->setMaxProgress(maxProgress);

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

void SHA1CacheFinder::updateOutputFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), "sha1cache (*.sha1cache)");
    ui->lineEditOutputFile->setText(fileName);
}
