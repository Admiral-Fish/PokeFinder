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

#include "PokeWalker.hpp"
#include "ui_PokeWalker.h"
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Generators/PokeWalkerGenerator.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Gen4/Searchers/PokeWalkerSearcher.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/States/PokeWalkerState.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen4/Profile/ProfileManager4.hpp>
#include <Forms/Gen4/Tools/SeedtoTime4.hpp>
#include <Forms/Models/Gen4/PokeWalkerModel.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

PokeWalker::PokeWalker(QWidget *parent) : QWidget(parent), ui(new Ui::PokeWalker)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();
}

PokeWalker::~PokeWalker()
{
    QSettings setting;
    setting.beginGroup("pokewalker");
    setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("minAdvanceIV", ui->textBoxSearcherIVMinAdvance->text());
    setting.setValue("maxAdvanceIV", ui->textBoxSearcherIVMaxAdvance->text());
    setting.setValue("minAdvancePID", ui->textBoxSearcherPIDMinAdvance->text());
    setting.setValue("maxAdvancePID", ui->textBoxSearcherPIDMaxAdvance->text());
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void PokeWalker::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PokeWalker::profilesIndexChanged);

    profiles = ProfileLoader4::getProfiles();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("pokewalker/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void PokeWalker::setupModels()
{
    generatorModel = new PokeWalkerGeneratorModel(ui->tableViewGenerator, Method::PokeWalkerIVs);
    searcherModel = new PokeWalkerSearcherModel(ui->tableViewSearcher, Method::PokeWalkerIVs);

    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);

    ui->textBoxSearcherIVMinAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherIVMaxAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherPIDMinAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherPIDMaxAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMinDelay->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorMethod->setup({ Method::PokeWalkerIVs, Method::PokeWalkerPID });

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=] { ui->tableViewGenerator->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, [=] { ui->tableViewGenerator->outputModel(true); });

    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(seedToTime, &QAction::triggered, this, &PokeWalker::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, [=] { ui->tableViewSearcher->outputModel(); });
    connect(outputCSVSearcher, &QAction::triggered, [=] { ui->tableViewSearcher->outputModel(true); });

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &PokeWalker::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &PokeWalker::search);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &PokeWalker::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &PokeWalker::tableViewSearcherContextMenu);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &PokeWalker::profileManager);

    QSettings setting;
    setting.beginGroup("pokewalker4");
    if (setting.contains("minDelay"))
    {
        ui->textBoxSearcherMinDelay->setText(setting.value("minDelay").toString());
    }
    if (setting.contains("maxDelay"))
    {
        ui->textBoxSearcherMaxDelay->setText(setting.value("maxDelay").toString());
    }
    if (setting.contains("minAdvanceIV"))
    {
        ui->textBoxSearcherIVMinAdvance->setText(setting.value("minAdvanceIV").toString());
    }
    if (setting.contains("maxAdvanceIV"))
    {
        ui->textBoxSearcherIVMaxAdvance->setText(setting.value("maxAdvanceIV").toString());
    }
    if (setting.contains("minAdvancePID"))
    {
        ui->textBoxSearcherPIDMinAdvance->setText(setting.value("minAdvancePID").toString());
    }
    if (setting.contains("maxAdvancePID"))
    {
        ui->textBoxSearcherPIDMaxAdvance->setText(setting.value("maxAdvancePID").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void PokeWalker::generate()
{
    generatorModel->clearModel();

    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();

    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt());
    generatorModel->setMethod(method);

    StateFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), {});

    PokeWalkerGenerator generator(initialAdvances, maxAdvances, tid, sid, ui->filterGenerator->getGenderRatio(), method, filter);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void PokeWalker::search()
{
    searcherModel->clearModel();

    Method methodModel;
    switch (ui->comboBoxSearcherMethod->currentIndex())
    {
    case 0:
        methodModel = Method::PokeWalkerIVs;
        break;
    case 1:
        methodModel = Method::PokeWalkerPID;
        break;
    case 2:
        methodModel = Method::Gen4Combined;
        break;
    }
    searcherModel->setMethod(methodModel);

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();

    StateFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false,
                       ui->filterSearcher->getMinIVs(), ui->filterSearcher->getMaxIVs(), ui->filterSearcher->getNatures(),
                       ui->filterSearcher->getHiddenPowers(), {});

    u32 minDelay = ui->textBoxSearcherMinDelay->getUInt();
    u32 maxDelay = ui->textBoxSearcherMaxDelay->getUInt();
    u32 minAdvanceIV = ui->textBoxSearcherIVMinAdvance->getUInt();
    u32 maxAdvanceIV = ui->textBoxSearcherIVMaxAdvance->getUInt();
    u32 minAdvancePID = ui->textBoxSearcherPIDMinAdvance->getUInt();
    u32 maxAdvancePID = ui->textBoxSearcherPIDMaxAdvance->getUInt();

    PokeWalkerGenerator generatorIV(minAdvanceIV, maxAdvanceIV, tid, sid, genderRatio, Method::PokeWalkerIVs, filter);
    PokeWalkerGenerator generatorPID(minAdvanceIV, maxAdvanceIV, tid, sid, genderRatio, Method::PokeWalkerPID, filter);

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(static_cast<int>(256 * 24 * (maxDelay - minDelay + 1)));

    auto *searcher = new PokeWalkerSearcher(tid, sid, genderRatio, methodModel, filter);

    auto *thread = QThread::create(
        [=] { searcher->startSearch(minDelay, maxDelay, ui->comboBoxSearcherMethod->currentIndex(), generatorIV, generatorPID); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, [=] {
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void PokeWalker::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));
    }
}

void PokeWalker::tableViewGeneratorContextMenu(QPoint pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void PokeWalker::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() > 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void PokeWalker::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    QString seed = searcherModel->data(searcherModel->index(index.row(), 0)).toString();

    auto *time = new SeedtoTime4(seed, currentProfile);
    time->show();
    time->raise();
}

void PokeWalker::profileManager()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::updateProfiles, this, [=] { emit alertProfiles(4); });
    manager->show();
}
