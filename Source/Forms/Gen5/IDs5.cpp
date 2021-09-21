/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "IDs5.hpp"
#include "ui_IDs5.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Generators/IDGenerator5.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Searchers/IDSearcher5.hpp>
#include <Core/Parents/Filters/IDFilter.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/Utilities.hpp>
#include <Forms/Gen5/Profile/ProfileManager5.hpp>
#include <Forms/Models/Gen5/IDModel5.hpp>
#include <QDate>
#include <QSettings>
#include <QThread>
#include <QTimer>

IDs5::IDs5(QWidget *parent) : QWidget(parent), ui(new Ui::IDs5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();
}

IDs5::~IDs5()
{
    QSettings setting;
    setting.beginGroup("ids5");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void IDs5::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IDs5::profileIndexChanged);

    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("ids5/profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
    }
}

bool IDs5::hasProfiles() const
{
    return !profiles.empty();
}

void IDs5::setupModels()
{
    model = new IDModel5(ui->tableView);
    menu = new QMenu(ui->tableView);

    ui->tableView->setModel(model);

    ui->textBoxPID->setValues(InputType::Seed32Bit);
    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);

    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxSeedFinderTID->setValues(InputType::TIDSID);
    ui->textBoxSeedFinderMaxAdvances->setValues(InputType::Advance32Bit);

    QAction *outputTXTGenerator = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=]() { ui->tableView->outputModel(false); });
    connect(outputCSVGenerator, &QAction::triggered, [=]() { ui->tableView->outputModel(true); });

    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &IDs5::search);
    connect(ui->pushButtonFind, &QPushButton::clicked, this, &IDs5::find);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &IDs5::profileManager);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &IDs5::tableViewContextMenu);

    QSettings setting;
    setting.beginGroup("id5");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void IDs5::search()
{
    model->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonFind->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 pid = ui->textBoxPID->getUInt();
    bool usePID = ui->checkBoxPID->isChecked();
    bool useXOR = ui->checkBoxXOR->isChecked();

    std::vector<u16> tid;
    if (ui->checkBoxTID->isChecked())
    {
        tid.emplace_back(ui->textBoxTID->getUShort());
    }

    std::vector<u16> sid;
    if (ui->checkBoxSID->isChecked())
    {
        sid.emplace_back(ui->textBoxSID->getUShort());
    }

    Date start = ui->dateEditStart->getDate();
    Date end = ui->dateEditEnd->getDate();

    IDFilter filter(tid, sid, {});
    IDGenerator5 generator(0, ui->textBoxMaxAdvances->getUInt(), filter);

    auto *searcher = new IDSearcher5(currentProfile, pid, usePID, useXOR);

    int maxProgress = Keypresses::getKeyPresses(currentProfile.getKeypresses(), currentProfile.getSkipLR()).size();
    maxProgress *= (start.daysTo(end) + 1);
    ui->progressBar->setRange(0, maxProgress);

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(generator, threads, start, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout,
            [=]
            {
                model->addItems(searcher->getResults());
                ui->progressBar->setValue(searcher->getProgress());
            });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed,
            [=]
            {
                ui->pushButtonSearch->setEnabled(true);
                ui->pushButtonFind->setEnabled(true);
                ui->pushButtonCancel->setEnabled(false);
                model->addItems(searcher->getResults());
                ui->progressBar->setValue(searcher->getProgress());
                delete searcher;
            });

    thread->start();
    timer->start(1000);
}

void IDs5::find()
{
    model->clearModel();

    u16 tid = ui->textBoxSeedFinderTID->getUShort();
    Date date = ui->dateEdit->getDate();
    u8 hour = ui->spinBoxHour->value();
    u8 minute = ui->spinBoxMinute->value();
    u8 minSecond = ui->spinBoxMinSecond->value();
    u8 maxSecond = ui->spinBoxMaxSecond->value();
    u32 maxAdvance = ui->textBoxSeedFinderMaxAdvances->getUInt();

    IDFilter filter({ tid }, {}, {});
    IDGenerator5 generator(0, maxAdvance, filter);

    auto buttons = Keypresses::getKeyPresses(currentProfile.getKeypresses(), currentProfile.getSkipLR());
    auto values = Keypresses::getValues(buttons);

    SHA1 sha(currentProfile);
    sha.setTimer0(currentProfile.getTimer0Min(), currentProfile.getVCount());
    sha.setDate(date);
    sha.precompute();

    bool flag = currentProfile.getVersion() & Game::BW;

    std::vector<IDState5> results;
    for (size_t i = 0; i < values.size(); i++)
    {
        sha.setButton(values[i]);

        for (u8 second = minSecond; second <= maxSecond; second++)
        {
            sha.setTime(hour, minute, second, currentProfile.getDSType());
            u64 seed = sha.hashSeed();

            generator.setInitialAdvances(flag ? Utilities::initialAdvancesBWID(seed) : Utilities::initialAdvancesBW2ID(seed));
            auto states = generator.generate(seed);

            DateTime dt(date, Time(hour, minute, second));
            for (auto &state : states)
            {
                state.setDateTime(dt);
                state.setKeypress(buttons[i]);
            }

            results.insert(results.end(), states.begin(), states.end());
        }
    }

    model->addItems(results);
}

void IDs5::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileMACAddressValue->setText(QString::number(currentProfile.getMac(), 16));
        ui->labelProfileDSTypeValue->setText(QString::fromStdString(currentProfile.getDSTypeString()));
        ui->labelProfileVCountValue->setText(QString::number(currentProfile.getVCount(), 16));
        ui->labelProfileTimer0Value->setText(QString::number(currentProfile.getTimer0Min(), 16) + "-"
                                             + QString::number(currentProfile.getTimer0Max(), 16));
        ui->labelProfileGxStatValue->setText(QString::number(currentProfile.getGxStat()));
        ui->labelProfileVFrameValue->setText(QString::number(currentProfile.getVFrame()));
        ui->labelProfileKeypressesValue->setText(QString::fromStdString(currentProfile.getKeypressesString()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));
    }
}

void IDs5::tableViewContextMenu(const QPoint &pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void IDs5::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::updateProfiles, this, [=] { emit alertProfiles(5); });
    manager->show();
}
