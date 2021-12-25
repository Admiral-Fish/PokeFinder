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

#include "HiddenGrotto.hpp"
#include "ui_HiddenGrotto.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/Filters/HiddenGrottoFilter.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Searchers/HiddenGrottoSearcher.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <Forms/Gen5/Profile/ProfileManager5.hpp>
#include <Forms/Models/Gen5/HiddenGrottoModel.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

HiddenGrotto::HiddenGrotto(QWidget *parent) : QWidget(parent), ui(new Ui::HiddenGrotto)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
    updateProfiles();
}

HiddenGrotto::~HiddenGrotto()
{
    QSettings setting;
    setting.beginGroup("hiddenGrotto");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void HiddenGrotto::updateProfiles()
{
    profiles.clear();
    auto completeProfiles = ProfileLoader5::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile5 &profile) { return (profile.getVersion() & Game::BW2) == Game::BW2; });

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("hiddenGrotto/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

bool HiddenGrotto::hasProfiles() const
{
    return !profiles.empty();
}

void HiddenGrotto::setupModels()
{
    generatorModel = new HiddenGrottoGeneratorModel5(ui->tableViewGenerator);
    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherModel = new HiddenGrottoSearcherModel5(ui->tableViewSearcher);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorGrottoPower->setItemData(0, 5);
    ui->comboBoxGeneratorGrottoPower->setItemData(1, 15);
    ui->comboBoxGeneratorGrottoPower->setItemData(2, 25);
    ui->comboBoxGeneratorGrottoPower->setItemData(3, 35);
    ui->comboBoxGeneratorGrottoPower->setItemData(4, 55);
    ui->checkListGeneratorSlot->setup({ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" });
    ui->checkListGeneratorGroup->setup({ "0", "1", "2", "3" });
    ui->checkListGeneratorGender->setup({ "♂", "♀" });
    ui->comboBoxGeneratorGenderRatio->setItemData(0, 60);
    ui->comboBoxGeneratorGenderRatio->setItemData(1, 30);
    ui->comboBoxGeneratorGenderRatio->setItemData(2, 10);
    ui->comboBoxGeneratorGenderRatio->setItemData(3, 5);

    ui->comboBoxSearcherGrottoPower->setItemData(0, 5);
    ui->comboBoxSearcherGrottoPower->setItemData(1, 15);
    ui->comboBoxSearcherGrottoPower->setItemData(2, 25);
    ui->comboBoxSearcherGrottoPower->setItemData(3, 35);
    ui->comboBoxSearcherGrottoPower->setItemData(4, 55);
    ui->checkListSearcherSlot->setup({ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" });
    ui->checkListSearcherGroup->setup({ "0", "1", "2", "3" });
    ui->checkListSearcherGender->setup({ "♂", "♀" });
    ui->comboBoxSearcherGenderRatio->setItemData(0, 60);
    ui->comboBoxSearcherGenderRatio->setItemData(1, 30);
    ui->comboBoxSearcherGenderRatio->setItemData(2, 10);
    ui->comboBoxSearcherGenderRatio->setItemData(3, 5);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    QAction *outputTXTGenerator = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(false); });
    connect(outputCSVGenerator, &QAction::triggered, [=]() { ui->tableViewGenerator->outputModel(true); });

    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(false); });
    connect(outputCSVSearcher, &QAction::triggered, [=]() { ui->tableViewSearcher->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &HiddenGrotto::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &HiddenGrotto::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &HiddenGrotto::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &HiddenGrotto::profileManager);
    connect(ui->pushButtonCalculateInitialAdvances, &QPushButton::clicked, this, &HiddenGrotto::calculateInitialAdvances);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &HiddenGrotto::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &HiddenGrotto::tableViewSearcherContextMenu);

    QSettings setting;
    setting.beginGroup("hiddenGrotto");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void HiddenGrotto::generate()
{
    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u8 genderRatio = ui->comboBoxGeneratorGenderRatio->currentData().toUInt();
    u8 powerLevel = ui->comboBoxGeneratorGrottoPower->currentData().toUInt();

    HiddenGrottoFilter filter(ui->checkListGeneratorGroup->getChecked(), ui->checkListGeneratorSlot->getChecked(),
                              ui->checkListGeneratorGender->getChecked());

    HiddenGrottoGenerator generator(initialAdvances, maxAdvances, genderRatio, powerLevel, filter);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void HiddenGrotto::search()
{
    searcherModel->clearModel();
    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    u8 genderRatio = ui->comboBoxSearcherGenderRatio->currentData().toUInt();
    u8 powerLevel = ui->comboBoxSearcherGrottoPower->currentData().toUInt();

    HiddenGrottoFilter filter(ui->checkListSearcherGroup->getChecked(), ui->checkListSearcherSlot->getChecked(),
                              ui->checkListSearcherGender->getChecked());

    HiddenGrottoGenerator generator(0, maxAdvances, genderRatio, powerLevel, filter);
    auto *searcher = new HiddenGrottoSearcher(currentProfile);

    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();

    int maxProgress = Keypresses::getKeyPresses(currentProfile.getKeypresses(), currentProfile.getSkipLR()).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= currentProfile.getTimer0Max() - currentProfile.getTimer0Min() + 1;
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
                searcherModel->addItems(searcher->getResults());
                ui->progressBar->setValue(searcher->getProgress());
            });

    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed,
            [=]
            {
                ui->pushButtonSearch->setEnabled(true);
                ui->pushButtonCancel->setEnabled(false);
                searcherModel->addItems(searcher->getResults());
                ui->progressBar->setValue(searcher->getProgress());
                delete searcher;
            });

    thread->start();
    timer->start(1000);
}

void HiddenGrotto::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
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

void HiddenGrotto::calculateInitialAdvances()
{
    ui->textBoxGeneratorInitialAdvances->setText(
        QString::number(Utilities::initialAdvancesBW2(ui->textBoxGeneratorSeed->getULong(), currentProfile.getMemoryLink())));
}

void HiddenGrotto::tableViewGeneratorContextMenu(QPoint pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void HiddenGrotto::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() > 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void HiddenGrotto::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::updateProfiles, this, [=] { emit alertProfiles(5); });
    manager->show();
}
