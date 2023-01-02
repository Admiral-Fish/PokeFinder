/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "GameCubeSeedFinder.hpp"
#include "ui_GameCubeSeedFinder.h"
#include <Core/Gen3/Searchers/ChannelSeedSearcher.hpp>
#include <Core/Gen3/Searchers/ColoSeedSearcher.hpp>
#include <Core/Gen3/Searchers/GalesSeedSearcher.hpp>
#include <Core/Util/Translator.hpp>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

static const std::map<QString, u8> patterns = { { "S->N->O->C", 11 }, { "O->N->S->C", 12 }, { "S->N->C->O", 13 }, { "C->N->S->O", 15 },
                                                { "O->N->C->S", 16 }, { "C->N->O->S", 17 }, { "N->S->O->C", 22 }, { "N->O->S->C", 24 },
                                                { "N->S->C->O", 26 }, { "N->C->S->O", 30 }, { "N->O->C->S", 32 }, { "N->C->O->S", 34 } };

GameCubeSeedFinder::GameCubeSeedFinder(QWidget *parent) : QWidget(parent), ui(new Ui::GameCubeSeedFinder), galesRound(1), coloRound(1)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->textBoxGalesTopLeft->setValues(1, 714, 3, 10);
    ui->textBoxGalesBottomLeft->setValues(1, 714, 3, 10);
    ui->textBoxGalesTopRight->setValues(1, 714, 3, 10);
    ui->textBoxGalesBottomRight->setValues(1, 714, 3, 10);

    for (const std::string &specie : Translator::getSpecies({ 150, 151, 386, 384, 385 }))
    {
        ui->comboBoxGalesYourLead->addItem(QString::fromStdString(specie));
    }
    for (const std::string &specie : Translator::getSpecies({ 144, 145, 146, 115, 380 }))
    {
        ui->comboBoxGalesEnemyLead->addItem(QString::fromStdString(specie));
    }

    for (const std::string &specie : Translator::getSpecies({ 257, 244, 260, 243, 154, 245, 376, 214 }))
    {
        ui->comboBoxColoPartyLead->addItem(QString::fromStdString(specie));
    }

    connect(ui->pushButtonGalesSearch, &QPushButton::clicked, this, &GameCubeSeedFinder::galesSearch);
    connect(ui->pushButtonGalesReset, &QPushButton::clicked, this, &GameCubeSeedFinder::galesReset);
    connect(ui->pushButtonColoSearch, &QPushButton::clicked, this, &GameCubeSeedFinder::coloSearch);
    connect(ui->pushButtonColoReset, &QPushButton::clicked, this, &GameCubeSeedFinder::coloReset);
    connect(ui->pushButtonChannelSearch, &QPushButton::clicked, this, &GameCubeSeedFinder::channelSearch);
    connect(ui->pushButtonChannelAdd, &QPushButton::clicked, this, &GameCubeSeedFinder::channelAdd);
    connect(ui->pushButtonChannelRemove, &QPushButton::clicked, this, &GameCubeSeedFinder::channelRemove);

    QSettings setting;
    if (setting.contains("gameCubeSeedFinder/geometry"))
    {
        this->restoreGeometry(setting.value("gameCubeSeedFinder/geometry").toByteArray());
    }
}

GameCubeSeedFinder::~GameCubeSeedFinder()
{
    QSettings setting;
    setting.setValue("gameCubeSeedFinder/geometry", this->saveGeometry());

    delete ui;
}

void GameCubeSeedFinder::updateChannel(const std::vector<u32> &seeds)
{
    if (seeds.empty())
    {
        ui->labelChannelResult->setText(tr("Result: Invalid"));
    }
    else
    {
        QString seed = "";
        for (size_t i = 0; i < seeds.size(); i++)
        {
            seed += QString::number(seeds[i], 16).toUpper();
            if (i != seeds.size() - 1)
            {
                seed += ", ";
            }
        }

        ui->labelChannelResult->setText(tr("Result: ") + seed);
        QMessageBox info(QMessageBox::Question, tr("Seed found"), tr("Your seed(s) is %1. Copy to clipboard?").arg(seed),
                         QMessageBox::Yes | QMessageBox::No);
        if (info.exec() == QMessageBox::Yes)
        {
            QApplication::clipboard()->setText(seed);
        }
    }
}

void GameCubeSeedFinder::updateChannelProgress(int progress)
{
    ui->progressBarChannel->setValue(progress);
}

void GameCubeSeedFinder::updateColo(const std::vector<u32> &seeds)
{
    coloSeeds = seeds;
    ui->labelColoRound->setText(tr("Round #") + QString::number(++coloRound));
    if (coloSeeds.size() == 1)
    {
        QString seed = QString::number(coloSeeds[0], 16).toUpper();
        ui->labelColoResults->setText(tr("Seed: ") + seed);
        QMessageBox info(QMessageBox::Question, tr("Seed found"), tr("Your seed is %1. Copy to clipboard?").arg(seed),
                         QMessageBox::Yes | QMessageBox::No);
        if (info.exec() == QMessageBox::Yes)
        {
            QApplication::clipboard()->setText(seed);
        }
    }
    else
    {
        ui->labelColoResults->setText(tr("Possible Results: %1").arg(coloSeeds.size()));
    }
}

void GameCubeSeedFinder::updateColoProgress(int progress)
{
    ui->progressBarColo->setValue(progress);
}

void GameCubeSeedFinder::updateGales(const std::vector<u32> &seeds)
{
    galeSeeds = seeds;
    ui->labelGalesRound->setText(tr("Round #") + QString::number(++galesRound));
    if (galeSeeds.size() == 1)
    {
        QString seed = QString::number(galeSeeds[0], 16).toUpper();
        ui->labelGalesResults->setText(tr("Seed: ") + seed);
        QMessageBox info(QMessageBox::Question, tr("Seed found"), tr("Your seed is %1. Copy to clipboard?").arg(seed),
                         QMessageBox::Yes | QMessageBox::No);
        if (info.exec() == QMessageBox::Yes)
        {
            QApplication::clipboard()->setText(seed);
        }
    }
    else
    {
        ui->labelGalesResults->setText(tr("Possible Results: %1").arg(galeSeeds.size()));
    }
}

void GameCubeSeedFinder::updateGalesProgress(int progress)
{
    ui->progressBarGales->setValue(progress);
}

void GameCubeSeedFinder::channelAdd()
{
    QString text = ui->buttonGroupChannel->checkedButton()->text();
    ui->textEditInputs->append(text);
}

void GameCubeSeedFinder::channelRemove()
{
    QString input = ui->textEditInputs->toPlainText();
    QStringList inputs = input.split("\n");
    inputs.removeLast();

    ui->textEditInputs->clear();
    ui->textEditInputs->insertPlainText(inputs.join("\n"));
}

void GameCubeSeedFinder::channelSearch()
{
    QStringList inputs = ui->textEditInputs->toPlainText().split("\n");
    if (inputs.size() < 10)
    {
        QMessageBox info(QMessageBox::Warning, tr("Missing info"), tr("You must have at least 10 entries"), QMessageBox::Ok);
        info.exec();
        return;
    }

    std::vector<u8> criteria;
    std::transform(inputs.begin(), inputs.end(), std::back_inserter(criteria), [](const QString &input) { return patterns.at(input); });

    ui->pushButtonChannelSearch->setEnabled(false);
    ui->pushButtonChannelCancel->setEnabled(true);
    ui->progressBarChannel->setRange(0, 0x60000000);

    auto *searcher = new ChannelSeedSearcher(criteria);

    QSettings setting;
    int threads = setting.value("settings/threads", QThread::idealThreadCount()).toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads); });
    connect(ui->pushButtonChannelCancel, &QPushButton::clicked, [=] { searcher->cancelSearch(); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    auto *timer = new QTimer();
    timer->callOnTimeout(this, [=] { updateChannelProgress(searcher->getProgress()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonChannelSearch->setEnabled(true);
        ui->pushButtonChannelCancel->setEnabled(false);
        updateChannelProgress(searcher->getProgress());
        updateChannel(searcher->getResults());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void GameCubeSeedFinder::coloReset()
{
    if (ui->pushButtonColoSearch->isEnabled())
    {
        coloSeeds.clear();
        coloSeeds.shrink_to_fit();
        coloRound = 1;
        ui->labelColoRound->setText(tr("Round #") + QString::number(coloRound));
    }
}

void GameCubeSeedFinder::coloSearch()
{
    if (coloPath.isEmpty())
    {
        QMessageBox msg(QMessageBox::Question, tr("Colo Precalc"),
                        tr("Would you like to use the Colo Precalc file? This decision will be remembered until you close this window."),
                        QMessageBox::Yes | QMessageBox::No);
        if (msg.exec() == QMessageBox::Yes)
        {
            QString path = QFileDialog::getOpenFileName(this, tr("Select Colo Precalc"), QDir::currentPath(), "precalc (*.precalc)");

            QFile f(path);
            if (f.open(QIODevice::ReadOnly))
            {
                if (qChecksum(f.readAll()) == 0x97B)
                {
                    coloPath = path;
                }
                else
                {
                    QMessageBox error(QMessageBox::Warning, tr("Invalid Precalc File"),
                                      tr("An invalid Precalc file was provided. Please try again."), QMessageBox::Ok);
                    error.exec();
                    return;
                }
            }
            else
            {
                QMessageBox error(QMessageBox::Warning, tr("Unable to open file"), tr("Unable to open file. Please try again."),
                                  QMessageBox::Ok);
                error.exec();
                return;
            }
        }
        else
        {
            coloPath = ' ';
        }
    }

    u8 partyLead = ui->comboBoxColoPartyLead->currentIndex();
    u8 trainer = ui->comboBoxColoTrainer->currentIndex();

    if (coloRound == 1 && !coloPath.isEmpty())
    {
        QFile f(coloPath);
        if (f.open(QIODevice::ReadOnly))
        {
            u32 seedSizes[24];
            f.read(reinterpret_cast<char *>(seedSizes), sizeof(seedSizes));

            u8 index = partyLead + 8 * trainer;
            u32 offset = 0;
            for (u8 i = 0; i < index; i++)
            {
                offset += seedSizes[i];
            }

            std::vector<u32> seeds;
            seeds.resize(seedSizes[index]);

            f.seek(offset * sizeof(u32) + sizeof(u32) * 24);
            f.read(reinterpret_cast<char *>(seeds.data()), sizeof(u32) * seedSizes[index]);

            updateColo(seeds);
        }

        return;
    }

    auto *searcher = new ColoSeedSearcher({ partyLead, trainer });

    ui->pushButtonColoSearch->setEnabled(false);
    ui->pushButtonColoCancel->setEnabled(true);

    QThread *thread;
    if (coloRound == 1)
    {
        QSettings setting;
        int threads = setting.value("settings/threads", QThread::idealThreadCount()).toInt();

        ui->progressBarColo->setRange(0, 0x10000);
        thread = QThread::create([=] { searcher->startSearch(threads); });
    }
    else
    {
        ui->progressBarColo->setRange(0, coloSeeds.size());
        thread = QThread::create([=] { searcher->startSearch(coloSeeds); });
    }
    connect(ui->pushButtonColoCancel, &QPushButton::clicked, [=] { searcher->cancelSearch(); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    auto *timer = new QTimer();
    timer->callOnTimeout(this, [=] { updateColoProgress(searcher->getProgress()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonColoSearch->setEnabled(true);
        ui->pushButtonColoCancel->setEnabled(false);
        updateColoProgress(searcher->getProgress());
        updateColo(searcher->getResults());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void GameCubeSeedFinder::galesReset()
{
    if (ui->pushButtonGalesSearch->isEnabled())
    {
        galeSeeds.clear();
        galeSeeds.shrink_to_fit();
        galesRound = 1;
        ui->labelGalesRound->setText(tr("Round #") + QString::number(galesRound));
    }
}

void GameCubeSeedFinder::galesSearch()
{
    if (galesPath.isEmpty())
    {
        QMessageBox msg(QMessageBox::Question, tr("Gales Precalc"),
                        tr("Would you like to use the Gales Precalc file? This decision will be remembered until you close this window."),
                        QMessageBox::Yes | QMessageBox::No);
        if (msg.exec() == QMessageBox::Yes)
        {
            QString path = QFileDialog::getOpenFileName(this, tr("Select Gales Precalc"), QDir::currentPath(), "precalc (*.precalc)");

            QFile f(path);
            if (f.open(QIODevice::ReadOnly))
            {
                if (qChecksum(f.readAll()) == 0xd75b)
                {
                    galesPath = path;
                }
                else
                {
                    QMessageBox error(QMessageBox::Warning, tr("Invalid Precalc File"),
                                      tr("An invalid Precalc file was provided. Please try again."), QMessageBox::Ok);
                    error.exec();
                    return;
                }
            }
            else
            {
                QMessageBox error(QMessageBox::Warning, tr("Unable to open file"), tr("Unable to open file. Please try again."),
                                  QMessageBox::Ok);
                error.exec();
                return;
            }
        }
        else
        {
            galesPath = ' ';
        }
    }

    u8 playerIndex = ui->comboBoxGalesYourLead->currentIndex();
    u8 enemyIndex = ui->comboBoxGalesEnemyLead->currentIndex();
    u16 playerHP1 = ui->textBoxGalesTopLeft->getUShort();
    u16 playerHP2 = ui->textBoxGalesBottomLeft->getUShort();
    u16 enemyHP1 = ui->textBoxGalesTopRight->getUShort();
    u16 enemyHP2 = ui->textBoxGalesBottomRight->getUShort();

    if (galesRound == 1 && !galesPath.isEmpty())
    {
        QFile f(galesPath);
        if (f.open(QIODevice::ReadOnly))
        {
            u32 seedSizes[25];
            f.read(reinterpret_cast<char *>(seedSizes), sizeof(seedSizes));

            u8 index = playerIndex * 5 + enemyIndex;
            u32 offset = 0;
            for (u8 i = 0; i < index; i++)
            {
                offset += seedSizes[i];
            }

            std::vector<u32> seeds;
            seeds.resize(seedSizes[index]);

            f.seek(offset * sizeof(u32) + sizeof(u32) * 25);
            f.read(reinterpret_cast<char *>(seeds.data()), sizeof(u32) * seedSizes[index]);

            updateGales(seeds);
        }

        return;
    }

    ui->pushButtonGalesSearch->setEnabled(false);
    ui->pushButtonGalesCancel->setEnabled(true);

    GalesCriteria criteria = { { enemyHP1, enemyHP2 }, { playerHP1, playerHP2 }, enemyIndex, playerIndex };
    auto *searcher = new GalesSeedSearcher(criteria);

    QThread *thread;
    if (galesRound == 1)
    {
        QSettings setting;
        int threads = setting.value("settings/threads", QThread::idealThreadCount()).toInt();

        ui->progressBarGales->setRange(0, 0x10000);
        thread = QThread::create([=] { searcher->startSearch(threads); });
    }
    else
    {
        ui->progressBarGales->setRange(0, galeSeeds.size());
        thread = QThread::create([=] { searcher->startSearch(galeSeeds); });
    }
    connect(ui->pushButtonGalesCancel, &QPushButton::clicked, [=] { searcher->cancelSearch(); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    auto *timer = new QTimer();
    timer->callOnTimeout(this, [=] { updateGalesProgress(searcher->getProgress()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonGalesSearch->setEnabled(true);
        ui->pushButtonGalesCancel->setEnabled(false);
        updateGalesProgress(searcher->getProgress());
        updateGales(searcher->getResults());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}
