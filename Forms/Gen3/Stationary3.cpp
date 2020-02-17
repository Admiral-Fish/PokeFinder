/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Stationary3.hpp"
#include "ui_Stationary3.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Generators/StationaryGenerator3.hpp>
#include <Core/Gen3/ProfileLoader3.hpp>
#include <Core/Gen3/Searchers/StationarySearcher3.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen3/Profile/ProfileManager3.hpp>
#include <Forms/Gen3/Tools/SeedTime3.hpp>
#include <Models/Gen3/StationaryModel3.hpp>
#include <QClipboard>
#include <QSettings>
#include <QThread>
#include <QTimer>

Stationary3::Stationary3(QWidget *parent) : QWidget(parent), ui(new Ui::Stationary3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame>>("QVector<Frame>");
}

Stationary3::~Stationary3()
{
    QSettings setting;
    setting.beginGroup("stationary3");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Stationary3::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Stationary3::profilesIndexChanged);

    profiles = { Profile3() };
    for (const auto &profile : ProfileLoader3::getProfiles())
    {
        if (!(profile.getVersion() & Game::GC))
        {
            profiles.append(profile);
        }
    }

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getName());
    }

    QSettings setting;
    int val = setting.value("stationary3/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Stationary3::setupModels()
{
    generatorModel = new StationaryGeneratorModel3(ui->tableViewGenerator);
    searcherModel = new StationarySearcherModel3(ui->tableViewSearcher);
    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialFrame->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorMaxResults->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorDelay->setValues(InputType::Frame32Bit);

    ui->comboBoxGeneratorNature->setup(Translator::getNatures());
    ui->comboBoxSearcherNature->setup(Translator::getNatures());

    ui->comboBoxGeneratorHiddenPower->setup(Translator::getHiddenPowers());
    ui->comboBoxSearcherHiddenPower->setup(Translator::getHiddenPowers());

    ui->comboBoxGeneratorMethod->setup({ Method::Method1, Method::Method1Reverse, Method::Method2, Method::Method4 });
    ui->comboBoxSearcherMethod->setup({ Method::Method1, Method::Method1Reverse, Method::Method2, Method::Method4 });

    ui->comboBoxGeneratorGender->setup({ 255, 0, 1 });
    ui->comboBoxSearcherGender->setup({ 255, 0, 1 });

    ui->comboBoxGeneratorAbility->setup({ 255, 0, 1 });
    ui->comboBoxSearcherAbility->setup({ 255, 0, 1 });

    ui->comboBoxGeneratorGenderRatio->setup({ 255, 127, 191, 63, 31, 0, 254 });
    ui->comboBoxSearcherGenderRatio->setup({ 255, 127, 191, 63, 31, 0, 254 });

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, this, [=] { ui->tableViewGenerator->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, this, [=] { ui->tableViewGenerator->outputModel(true); });

    QAction *copySeedToClipboard = searcherMenu->addAction(tr("Copy Seed to Clipboard"));
    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(copySeedToClipboard, &QAction::triggered, this, &Stationary3::copySeedToClipboard);
    connect(seedToTime, &QAction::triggered, this, &Stationary3::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, this, [=] { ui->tableViewSearcher->outputModel(); });
    connect(outputCSVSearcher, &QAction::triggered, this, [=] { ui->tableViewSearcher->outputModel(true); });

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Stationary3::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Stationary3::search);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &Stationary3::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &Stationary3::tableViewSearcherContextMenu);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Stationary3::profileManager);

    QSettings setting;
    if (setting.contains("stationary3/geometry"))
    {
        this->restoreGeometry(setting.value("stationary3/geometry").toByteArray());
    }
}

void Stationary3::updateProgress(const QVector<Frame> &frames, int progress)
{
    searcherModel->addItems(frames);
    ui->progressBar->setValue(progress);
}

void Stationary3::generate()
{
    generatorModel->clearModel();

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialFrame = ui->textBoxGeneratorInitialFrame->getUInt();
    u32 maxResults = ui->textBoxGeneratorMaxResults->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->comboBoxGeneratorGenderRatio->getCurrentByte();
    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
    u32 offset = 0;
    if (ui->checkBoxGeneratorDelay->isChecked())
    {
        offset = ui->textBoxGeneratorDelay->getUInt();
    }

    StationaryGenerator3 generator(initialFrame, maxResults, tid, sid, genderRatio, method);
    generator.setSeed(seed);
    generator.setOffset(offset);

    FrameFilter filter(ui->comboBoxGeneratorGender->getCurrentByte(), ui->comboBoxGeneratorAbility->getCurrentByte(),
                       ui->checkBoxGeneratorShinyOnly->isChecked(), ui->checkBoxGeneratorDisableFilters->isChecked(),
                       ui->ivFilterGenerator->getLower(), ui->ivFilterGenerator->getUpper(), ui->comboBoxGeneratorNature->getChecked(),
                       ui->comboBoxGeneratorHiddenPower->getChecked(), QVector<bool>());

    auto frames = generator.generate(filter);
    generatorModel->addItems(frames);
}

void Stationary3::search()
{
    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    QVector<u8> min = ui->ivFilterSearcher->getLower();
    QVector<u8> max = ui->ivFilterSearcher->getUpper();

    FrameFilter filter(ui->comboBoxSearcherGender->getCurrentByte(), ui->comboBoxSearcherAbility->getCurrentByte(),
                       ui->checkBoxSearcherShinyOnly->isChecked(), false, min, max, ui->comboBoxSearcherNature->getChecked(),
                       ui->comboBoxSearcherHiddenPower->getChecked(), QVector<bool>());

    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->comboBoxSearcherGenderRatio->getCurrentByte();
    auto method = static_cast<Method>(ui->comboBoxSearcherMethod->getCurrentInt());

    auto *searcher = new StationarySearcher3(tid, sid, genderRatio, method, filter);

    int maxProgress = 1;
    for (u8 i = 0; i < 6; i++)
    {
        maxProgress *= max.at(i) - min.at(i) + 1;
    }
    ui->progressBar->setRange(0, maxProgress);

    auto *thread = QThread::create([=] { searcher->startSearch(min, max); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, [=] { updateProgress(searcher->getResults(), searcher->getProgress()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        updateProgress(searcher->getResults(), searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void Stationary3::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles.at(index);

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(currentProfile.getVersionString());
    }
}

void Stationary3::tableViewGeneratorContextMenu(QPoint pos)
{
    if (generatorModel->rowCount() > 0)
    {

        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void Stationary3::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() > 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void Stationary3::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    index = searcherModel->index(index.row(), 0);
    u32 seed = searcherModel->data(index).toString().toUInt(nullptr, 16);

    auto *seedToTime = new SeedTime3(seed);
    seedToTime->show();
    seedToTime->raise();
}

void Stationary3::copySeedToClipboard()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    index = searcherModel->index(index.row(), 0);

    QApplication::clipboard()->setText(searcherModel->data(index).toString());
}

void Stationary3::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}
