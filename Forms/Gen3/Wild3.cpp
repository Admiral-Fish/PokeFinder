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

#include "Wild3.hpp"
#include "ui_Wild3.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/WildGenerator3.hpp>
#include <Core/Gen3/ProfileLoader3.hpp>
#include <Core/Gen3/Searchers/WildSearcher3.hpp>
#include <Core/Parents/Frames/WildFrame.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen3/Profile/ProfileManager3.hpp>
#include <Forms/Gen3/Tools/SeedTime3.hpp>
#include <Models/Gen3/WildModel3.hpp>
#include <QClipboard>
#include <QSettings>
#include <QThread>
#include <QTimer>

Wild3::Wild3(QWidget *parent) : QWidget(parent), ui(new Ui::Wild3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<WildFrame>>("QVector<WildFrame>");
}

Wild3::~Wild3()
{
    QSettings setting;
    setting.beginGroup("wild3");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Wild3::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild3::profilesIndexChanged);

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
    int val = setting.value("wild3/profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Wild3::setupModels()
{
    generatorModel = new WildGeneratorModel3(ui->tableViewGenerator);
    searcherModel = new WildSearcherModel3(ui->tableViewSearcher);
    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialFrame->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorMaxResults->setValues(InputType::Frame32Bit);

    ui->comboBoxGeneratorMethod->setup({ Method::MethodH1, Method::MethodH2, Method::MethodH4 });
    ui->comboBoxSearcherMethod->setup({ Method::MethodH1, Method::MethodH2, Method::MethodH4 });

    ui->filterSearcher->disableControls(Controls::UseDelay | Controls::DisableFilter);

    generatorLead();

    ui->comboBoxSearcherLead->setup({ Lead::Search, Lead::Synchronize, Lead::CuteCharm, Lead::None });

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, this, [=] { ui->tableViewGenerator->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, this, [=] { ui->tableViewGenerator->outputModel(true); });

    QAction *copySeedToClipboard = searcherMenu->addAction(tr("Copy Seed to Clipboard"));
    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(copySeedToClipboard, &QAction::triggered, this, &Wild3::copySeedToClipboard);
    connect(seedToTime, &QAction::triggered, this, &Wild3::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, [=] { ui->tableViewSearcher->outputModel(); });
    connect(outputCSVSearcher, &QAction::triggered, [=] { ui->tableViewSearcher->outputModel(true); });

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Wild3::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Wild3::search);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &Wild3::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &Wild3::tableViewSearcherContextMenu);
    connect(ui->pushButtonGeneratorLead, &QPushButton::clicked, this, &Wild3::generatorLead);
    connect(ui->comboBoxGeneratorEncounter, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Wild3::generatorEncounterIndexChanged);
    connect(ui->comboBoxSearcherEncounter, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Wild3::searcherEncounterIndexChanged);
    connect(ui->comboBoxGeneratorLocation, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Wild3::generatorLocationIndexChanged);
    connect(ui->comboBoxSearcherLocation, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild3::searcherLocationIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild3::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild3::searcherPokemonIndexChanged);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Wild3::profileManager);

    generatorEncounterIndexChanged(0);
    searcherEncounterIndexChanged(0);

    QSettings setting;
    if (setting.contains("wild3/geometry"))
    {
        this->restoreGeometry(setting.value("wild3/geometry").toByteArray());
    }
}

void Wild3::updateLocationsGenerator()
{
    auto encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt());
    encounterGenerator = Encounters3::getEncounters(encounter, currentProfile);

    QVector<u8> locs;
    for (const auto &area : encounterGenerator)
    {
        locs.append(area.getLocation());
    }

    QStringList locations = Translator::getLocations(locs, currentProfile.getVersion());

    ui->comboBoxGeneratorLocation->clear();
    ui->comboBoxGeneratorLocation->addItems(locations);
}

void Wild3::updateLocationsSearcher()
{
    auto encounter = static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt());
    encounterSearcher = Encounters3::getEncounters(encounter, currentProfile);

    QVector<u8> locs;
    for (const auto &area : encounterSearcher)
    {
        locs.append(area.getLocation());
    }

    QStringList locations = Translator::getLocations(locs, currentProfile.getVersion());

    ui->comboBoxSearcherLocation->clear();
    ui->comboBoxSearcherLocation->addItems(locations);
}

void Wild3::updatePokemonGenerator()
{
    auto area = encounterGenerator.at(ui->comboBoxGeneratorLocation->currentIndex());
    QVector<u16> species = area.getUniqueSpecies();

    QStringList names = area.getSpecieNames();

    ui->comboBoxGeneratorPokemon->clear();
    ui->comboBoxGeneratorPokemon->addItem("-");
    for (int i = 0; i < species.size(); i++)
    {
        ui->comboBoxGeneratorPokemon->addItem(names.at(i), species.at(i));
    }
}

void Wild3::updatePokemonSearcher()
{
    auto area = encounterSearcher.at(ui->comboBoxSearcherLocation->currentIndex());
    QVector<u16> species = area.getUniqueSpecies();

    QStringList names = area.getSpecieNames();

    ui->comboBoxSearcherPokemon->clear();
    ui->comboBoxSearcherPokemon->addItem("-");
    for (int i = 0; i < species.size(); i++)
    {
        ui->comboBoxSearcherPokemon->addItem(names.at(i), species.at(i));
    }
}

void Wild3::updateProgress(const QVector<WildFrame> &frames, int progress)
{
    searcherModel->addItems(frames);
    ui->progressBar->setValue(progress);
}

void Wild3::generate()
{
    generatorModel->clearModel();

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialFrame = ui->textBoxGeneratorInitialFrame->getUInt();
    u32 maxResults = ui->textBoxGeneratorMaxResults->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    FrameFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), ui->filterGenerator->getEncounterSlots());

    WildGenerator3 generator(initialFrame, maxResults, tid, sid, genderRatio, method, filter);
    generator.setEncounter(static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt()));
    generator.setEncounterArea(encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()]);
    generator.setOffset(offset);

    if (ui->pushButtonGeneratorLead->text() == tr("Cute Charm"))
    {
        generator.setLead(static_cast<Lead>(ui->comboBoxGeneratorLead->currentData().toInt()));
    }
    else
    {
        if (ui->comboBoxGeneratorLead->currentIndex() == 0)
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

void Wild3::search()
{
    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    QVector<u8> min = ui->filterSearcher->getMinIVs();
    QVector<u8> max = ui->filterSearcher->getMaxIVs();

    FrameFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false, min, max,
                       ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers(), ui->filterSearcher->getEncounterSlots());

    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();
    auto type = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());

    auto *searcher = new WildSearcher3(tid, sid, genderRatio, type, filter);
    searcher->setEncounter(static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt()));
    searcher->setLead(static_cast<Lead>(ui->comboBoxSearcherLead->currentData().toInt()));
    searcher->setEncounterArea(encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()]);

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

void Wild3::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles.at(index);

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(currentProfile.getVersionString());

        bool flag = currentProfile.getVersion() & Game::FRLG;
        ui->comboBoxGeneratorEncounter->clear();
        ui->comboBoxSearcherEncounter->clear();

        ui->comboBoxGeneratorEncounter->addItem(tr("Grass"), Encounter::Grass);
        ui->comboBoxSearcherEncounter->addItem(tr("Grass"), Encounter::Grass);
        if (!flag)
        {
            ui->comboBoxGeneratorEncounter->addItem(tr("Safari Zone"), Encounter::SafariZone);
            ui->comboBoxSearcherEncounter->addItem(tr("Safari Zone"), Encounter::SafariZone);
        }
        ui->comboBoxGeneratorEncounter->addItem(tr("Rock Smash"), Encounter::RockSmash);
        ui->comboBoxSearcherEncounter->addItem(tr("Rock Smash"), Encounter::RockSmash);
        ui->comboBoxGeneratorEncounter->addItem(tr("Surfing"), Encounter::Surfing);
        ui->comboBoxSearcherEncounter->addItem(tr("Surfing"), Encounter::Surfing);
        ui->comboBoxGeneratorEncounter->addItem(tr("Old Rod"), Encounter::OldRod);
        ui->comboBoxSearcherEncounter->addItem(tr("Old Rod"), Encounter::OldRod);
        ui->comboBoxGeneratorEncounter->addItem(tr("Good Rod"), Encounter::GoodRod);
        ui->comboBoxSearcherEncounter->addItem(tr("Good Rod"), Encounter::GoodRod);
        ui->comboBoxGeneratorEncounter->addItem(tr("Super Rod"), Encounter::SuperRod);
        ui->comboBoxSearcherEncounter->addItem(tr("Super Rod"), Encounter::SuperRod);

        updateLocationsSearcher();
        updateLocationsGenerator();
    }
}

void Wild3::tableViewGeneratorContextMenu(QPoint pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void Wild3::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() > 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void Wild3::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    index = searcherModel->index(index.row(), 0);

    u32 seed = searcherModel->data(index).toString().toUInt(nullptr, 16);
    auto *seedToTime = new SeedTime3(seed);
    seedToTime->show();
    seedToTime->raise();
}

void Wild3::copySeedToClipboard()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    index = searcherModel->index(index.row(), 0);

    QApplication::clipboard()->setText(searcherModel->data(index).toString());
}

void Wild3::generatorLead()
{
    ui->comboBoxGeneratorLead->clear();
    QString text = ui->pushButtonGeneratorLead->text();
    if (text == tr("Synchronize"))
    {
        ui->pushButtonGeneratorLead->setText(tr("Cute Charm"));

        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead (50% ♀ Target)"), Lead::CuteCharm50F);
        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead (75% ♀ Target)"), Lead::CuteCharm75F);
        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead (25% ♀ Target)"), Lead::CuteCharm25F);
        ui->comboBoxGeneratorLead->addItem(tr("♂ Lead (12.5% ♀ Target)"), Lead::CuteCharm125F);
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

void Wild3::generatorEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        QStringList t;
        Encounter encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt());

        switch (encounter)
        {
        case Encounter::Grass:
        case Encounter::SafariZone:
            t = QStringList({ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" });
            break;
        case Encounter::RockSmash:
        case Encounter::Surfing:
        case Encounter::SuperRod:
            t = QStringList({ "0", "1", "2", "3", "4" });
            break;
        case Encounter::OldRod:
            t = QStringList({ "0", "1" });
            break;
        case Encounter::GoodRod:
            t = QStringList({ "0", "1", "2" });
            break;
        default:
            break;
        }

        ui->filterGenerator->setEncounterSlots(t);
        updateLocationsGenerator();
    }
}

void Wild3::searcherEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        QStringList t;
        Encounter encounter = static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt());

        switch (encounter)
        {
        case Encounter::Grass:
        case Encounter::SafariZone:
            t = QStringList({ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" });
            break;
        case Encounter::RockSmash:
        case Encounter::Surfing:
        case Encounter::SuperRod:
            t = QStringList({ "0", "1", "2", "3", "4" });
            break;
        case Encounter::OldRod:
            t = QStringList({ "0", "1" });
            break;
        case Encounter::GoodRod:
            t = QStringList({ "0", "1", "2" });
            break;
        default:
            break;
        }
        ui->filterSearcher->setEncounterSlots(t);
        updateLocationsSearcher();
    }
}

void Wild3::generatorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        updatePokemonGenerator();
    }
}

void Wild3::searcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        updatePokemonSearcher();
    }
}

void Wild3::generatorPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterGenerator->resetEncounterSlots();
    }
    else
    {
        u16 num = static_cast<u16>(ui->comboBoxGeneratorPokemon->currentData().toUInt());
        QVector<bool> flags = encounterGenerator.at(ui->comboBoxGeneratorLocation->currentIndex()).getSlots(num);

        ui->filterGenerator->toggleEncounterSlots(flags);
    }
}

void Wild3::searcherPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterSearcher->resetEncounterSlots();
    }
    else
    {
        u16 num = static_cast<u16>(ui->comboBoxSearcherPokemon->currentData().toUInt());
        QVector<bool> flags = encounterSearcher.at(ui->comboBoxSearcherLocation->currentIndex()).getSlots(num);

        ui->filterSearcher->toggleEncounterSlots(flags);
    }
}

void Wild3::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}
