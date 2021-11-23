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

#include "Stationary4.hpp"
#include "ui_Stationary4.h"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Generators/StationaryGenerator4.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Gen4/Searchers/StationarySearcher4.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/States/StationaryState.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen4/Profile/ProfileManager4.hpp>
#include <Forms/Gen4/Tools/SeedtoTime4.hpp>
#include <Forms/Models/Gen4/StationaryModel4.hpp>
#include <QSettings>
#include <QThread>
#include <QTimer>

Stationary4::Stationary4(QWidget *parent) : QWidget(parent), ui(new Ui::Stationary4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
    updateProfiles();
}

Stationary4::~Stationary4()
{
    QSettings setting;
    setting.beginGroup("stationary4");
    setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("minAdvance", ui->textBoxSearcherMinAdvance->text());
    setting.setValue("maxAdvance", ui->textBoxSearcherMaxAdvance->text());
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Stationary4::updateProfiles()
{
    profiles = ProfileLoader4::getProfiles();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("stationary4/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Stationary4::setupModels()
{
    generatorModel = new StationaryGeneratorModel4(ui->tableViewGenerator, Method::Method1);
    searcherModel = new StationarySearcherModel4(ui->tableViewSearcher, Method::Method1);

    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherMinDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMinAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvance->setValues(InputType::Advance32Bit);

    ui->comboBoxSearcherLead->setup({ Lead::Search, Lead::Synchronize, Lead::CuteCharm, Lead::None });

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    ui->toolButtonGeneratorLead->addAction(tr("None"), Lead::None);
    ui->toolButtonGeneratorLead->addMenu(tr("Synchronize"), Translator::getNatures());
    ui->toolButtonGeneratorLead->addMenu(
        tr("Cute Charm"),
        { tr("♂ Lead"), tr("♀ Lead (50% ♂ Target)"), tr("♀ Lead (75% ♂ Target)"), tr("♀ Lead (25% ♂ Target)"),
          tr("♀ Lead (87.5% ♂ Target)") },
        { Lead::CuteCharmFemale, Lead::CuteCharm50M, Lead::CuteCharm75M, Lead::CuteCharm25M, Lead::CuteCharm875M });

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=] { ui->tableViewGenerator->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, [=] { ui->tableViewGenerator->outputModel(true); });

    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(seedToTime, &QAction::triggered, this, &Stationary4::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, [=] { ui->tableViewSearcher->outputModel(); });
    connect(outputCSVSearcher, &QAction::triggered, [=] { ui->tableViewSearcher->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Stationary4::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Stationary4::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Stationary4::search);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &Stationary4::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &Stationary4::tableViewSearcherContextMenu);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Stationary4::profileManager);

    QSettings setting;
    setting.beginGroup("stationary4");
    if (setting.contains("minDelay"))
    {
        ui->textBoxSearcherMinDelay->setText(setting.value("minDelay").toString());
    }
    if (setting.contains("maxDelay"))
    {
        ui->textBoxSearcherMaxDelay->setText(setting.value("maxDelay").toString());
    }
    if (setting.contains("minAdvance"))
    {
        ui->textBoxSearcherMinAdvance->setText(setting.value("minAdvance").toString());
    }
    if (setting.contains("maxAdvance"))
    {
        ui->textBoxSearcherMaxAdvance->setText(setting.value("maxAdvance").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void Stationary4::generate()
{
    generatorModel->clearModel();
    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
    generatorModel->setMethod(method);

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), {});

    StationaryGenerator4 generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter);
    generator.setOffset(offset);

    if (ui->toolButtonGeneratorLead->text() == tr("Sychronize"))
    {
        generator.setLead(Lead::Synchronize);
        generator.setSynchNature(ui->toolButtonGeneratorLead->getData());
    }
    else
    {
        generator.setLead(static_cast<Lead>(ui->toolButtonGeneratorLead->getData()));
    }

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Stationary4::search()
{
    auto method = static_cast<Method>(ui->comboBoxSearcherMethod->getCurrentInt());
    searcherModel->clearModel();
    searcherModel->setMethod(method);

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    std::array<u8, 6> min = ui->filterSearcher->getMinIVs();
    std::array<u8, 6> max = ui->filterSearcher->getMaxIVs();

    StateFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false, min, max,
                       ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers(), {});

    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();

    auto *searcher = new StationarySearcher4(tid, sid, genderRatio, method, filter);
    searcher->setDelay(ui->textBoxSearcherMinDelay->getUInt(), ui->textBoxSearcherMaxDelay->getUInt());
    searcher->setState(ui->textBoxSearcherMinAdvance->getUInt(), ui->textBoxSearcherMaxAdvance->getUInt());
    searcher->setLead(static_cast<Lead>(ui->comboBoxSearcherLead->getCurrentInt()));

    int maxProgress = 1;
    for (u8 i = 0; i < 6; i++)
    {
        maxProgress *= max[i] - min[i] + 1;
    }
    ui->progressBar->setRange(0, maxProgress);

    auto *thread = QThread::create([=] { searcher->startSearch(min, max); });
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

void Stationary4::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));

        bool flag = currentProfile.getVersion() & Game::HGSS;

        ui->comboBoxGeneratorMethod->clear();
        ui->comboBoxGeneratorMethod->addItem(tr("Method 1"), Method::Method1);
        ui->comboBoxGeneratorMethod->addItem(flag ? tr("Method K") : tr("Method J"), flag ? Method::MethodK : Method::MethodJ);
        ui->comboBoxGeneratorMethod->addItem(tr("Wondercard IVs"), Method::WondercardIVs);

        ui->comboBoxSearcherMethod->clear();
        ui->comboBoxSearcherMethod->addItem(tr("Method 1"), Method::Method1);
        ui->comboBoxSearcherMethod->addItem(flag ? tr("Method K") : tr("Method J"), flag ? Method::MethodK : Method::MethodJ);
        ui->comboBoxSearcherMethod->addItem(tr("Wondercard IVs"), Method::WondercardIVs);
    }
}

void Stationary4::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    auto *time = new SeedtoTime4(searcherModel->data(searcherModel->index(index.row(), 0), Qt::DisplayRole).toString(), currentProfile);
    time->show();
    time->raise();
}

void Stationary4::tableViewGeneratorContextMenu(QPoint pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void Stationary4::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() > 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void Stationary4::profileManager()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::updateProfiles, this, [=] { emit alertProfiles(4); });
    manager->show();
}
