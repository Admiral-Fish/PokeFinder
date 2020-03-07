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

#include "Stationary4.hpp"
#include "ui_Stationary4.h"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Generators/StationaryGenerator4.hpp>
#include <Core/Gen4/ProfileLoader4.hpp>
#include <Core/Gen4/Searchers/StationarySearcher4.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/Parents/Frames/StationaryFrame.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen4/Profile/ProfileManager4.hpp>
#include <Forms/Gen4/Tools/SeedtoTime4.hpp>
#include <Models/Gen4/StationaryModel4.hpp>
#include <QSettings>
#include <QThread>
#include <QTimer>

Stationary4::Stationary4(QWidget *parent) : QWidget(parent), ui(new Ui::Stationary4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<StationaryFrame>>("QVector<StationaryFrame>");
}

Stationary4::~Stationary4()
{
    QSettings setting;
    setting.beginGroup("stationary4");
    setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("minFrame", ui->textBoxSearcherMinFrame->text());
    setting.setValue("maxFrame", ui->textBoxSearcherMaxFrame->text());
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Stationary4::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Stationary4::profileIndexChanged);

    profiles = ProfileLoader4::getProfiles();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getName());
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
    ui->textBoxGeneratorInitialFrame->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorMaxResults->setValues(InputType::Frame32Bit);

    ui->textBoxSearcherMinDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMinFrame->setValues(InputType::Frame32Bit);
    ui->textBoxSearcherMaxFrame->setValues(InputType::Frame32Bit);

    ui->comboBoxSearcherLead->setup({ Lead::Search, Lead::Synchronize, Lead::CuteCharm, Lead::None });

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    ui->comboBoxGeneratorLead->addItem(tr("None"));
    ui->comboBoxGeneratorLead->addItems(Translator::getNatures());

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

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Stationary4::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Stationary4::search);
    connect(ui->pushButtonGeneratorLead, &QPushButton::clicked, this, &Stationary4::generatorLead);
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
    if (setting.contains("minFrame"))
    {
        ui->textBoxSearcherMinFrame->setText(setting.value("minFrame").toString());
    }
    if (setting.contains("maxFrame"))
    {
        ui->textBoxSearcherMaxFrame->setText(setting.value("maxFrame").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void Stationary4::updateProgress(const QVector<StationaryFrame> &frames, int progress)
{
    searcherModel->addItems(frames);
    ui->progressBar->setValue(progress);
}

void Stationary4::generate()
{
    generatorModel->clearModel();
    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
    generatorModel->setMethod(method);

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialFrame = ui->textBoxGeneratorInitialFrame->getUInt();
    u32 maxResults = ui->textBoxGeneratorMaxResults->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    FrameFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), {});

    StationaryGenerator4 generator(initialFrame, maxResults, tid, sid, genderRatio, method, filter);
    generator.setOffset(offset);

    if (ui->pushButtonGeneratorLead->text() == tr("Cute Charm"))
    {
        generator.setLead(static_cast<Lead>(ui->comboBoxGeneratorLead->currentData().toInt()));
    }
    else
    {
        int num = ui->comboBoxGeneratorLead->currentIndex();
        if (num == 0)
        {
            generator.setLead(Lead::None);
        }
        else
        {
            generator.setLead(Lead::Synchronize);
            generator.setSynchNature(static_cast<u8>(ui->comboBoxGeneratorLead->currentIndex() - 1));
        }
    }

    auto frames = generator.generate(seed);
    generatorModel->addItems(frames);
}

void Stationary4::search()
{
    auto method = static_cast<Method>(ui->comboBoxSearcherMethod->getCurrentInt());
    searcherModel->clearModel();
    searcherModel->setMethod(method);

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    QVector<u8> min = ui->filterSearcher->getMinIVs();
    QVector<u8> max = ui->filterSearcher->getMaxIVs();

    FrameFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false, min, max,
                       ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers(), {});

    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();

    auto *searcher = new StationarySearcher4(tid, sid, genderRatio, method, filter);
    searcher->setDelay(ui->textBoxSearcherMinDelay->getUInt(), ui->textBoxSearcherMaxDelay->getUInt());
    searcher->setFrame(ui->textBoxSearcherMinFrame->getUInt(), ui->textBoxSearcherMaxFrame->getUInt());
    searcher->setLead(static_cast<Lead>(ui->comboBoxSearcherLead->getCurrentInt()));

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

void Stationary4::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles.at(index);

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(currentProfile.getVersionString());

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

void Stationary4::generatorLead()
{
    ui->comboBoxGeneratorLead->clear();
    QString text = ui->pushButtonGeneratorLead->text();
    if (text == tr("Synchronize"))
    {
        ui->pushButtonGeneratorLead->setText(tr("Cute Charm"));
        ui->comboBoxGeneratorLead->setEnabled(true);

        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead"), Lead::CuteCharmFemale);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (50% ♂ Target)"), Lead::CuteCharm50M);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (75% ♂ Target)"), Lead::CuteCharm75M);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (25% ♂ Target)"), Lead::CuteCharm25M);
        ui->comboBoxGeneratorLead->addItem(tr("♀ Lead (87.5% ♂ Target)"), Lead::CuteCharm875M);
    }
    else
    {
        ui->pushButtonGeneratorLead->setText(tr("Synchronize"));

        ui->comboBoxGeneratorLead->addItem("None");
        ui->comboBoxGeneratorLead->addItems(Translator::getNatures());
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
