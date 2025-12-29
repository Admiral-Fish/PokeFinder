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

#include "Static4.hpp"
#include "ui_Static4.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Generators/StaticGenerator4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/Searchers/StaticSearcher4.hpp>
#include <Core/Gen4/StaticTemplate4.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen4/Profile/ProfileManager4.hpp>
#include <Form/Gen4/Tools/SeedToTime4.hpp>
#include <Model/Gen4/StaticModel4.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QSettings>
#include <QThread>
#include <QTimer>

Static4::Static4(QWidget *parent) : QWidget(parent), ui(new Ui::Static4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new StaticGeneratorModel4(ui->tableViewGenerator, Method::Method1);
    searcherModel = new StaticSearcherModel4(ui->tableViewSearcher);
    proxyModel = new SortFilterProxyModel(ui->tableViewSearcher, searcherModel);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(proxyModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorOffset->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherMinDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMinAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvance->setValues(InputType::Advance32Bit);

    ui->filterGenerator->disableControls(Controls::EncounterSlots | Controls::Height | Controls::Weight);
    ui->filterSearcher->disableControls(Controls::DisableFilter | Controls::EncounterSlots | Controls::Height | Controls::Weight);

    ui->comboMenuGeneratorLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuGeneratorLead->addMenu(tr("Cute Charm"),
                                        { { tr("♂ Lead"), toInt(Lead::CuteCharmM) }, { tr("♀ Lead"), toInt(Lead::CuteCharmF) } });
    ui->comboMenuGeneratorLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboMenuSearcherLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuSearcherLead->addMenu(tr("Cute Charm"),
                                       { { tr("♂ Lead"), toInt(Lead::CuteCharmM) }, { tr("♀ Lead"), toInt(Lead::CuteCharmF) } });
    ui->comboMenuSearcherLead->addAction(tr("Synchronize"), toInt(Lead::Synchronize));

    auto *seedToTime = new QAction(tr("Generate times for seed"), ui->tableViewSearcher);
    connect(seedToTime, &QAction::triggered, this, &Static4::seedToTime);
    ui->tableViewSearcher->addAction(seedToTime);

    ui->comboBoxGeneratorShiny->setup({ toInt(Shiny::Never), toInt(Shiny::Random) });
    ui->comboBoxSearcherShiny->setup({ toInt(Shiny::Never), toInt(Shiny::Random) });

    connect(ui->tabRNGSelector, &TabWidget::transferFilters, this, &Static4::transferFilters);
    connect(ui->tabRNGSelector, &TabWidget::transferSettings, this, &Static4::transferSettings);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Static4::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Static4::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Static4::profileManager);
    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Static4::profileIndexChanged);
    connect(ui->comboBoxGeneratorCategory, &QComboBox::currentIndexChanged, this, &Static4::generatorCategoryIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, &QComboBox::currentIndexChanged, this, &Static4::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherCategory, &QComboBox::currentIndexChanged, this, &Static4::searcherCategoryIndexChanged);
    connect(ui->comboBoxSearcherPokemon, &QComboBox::currentIndexChanged, this, &Static4::searcherPokemonIndexChanged);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &StaticGeneratorModel4::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &StaticSearcherModel4::setShowStats);

    updateProfiles();
    generatorCategoryIndexChanged(0);
    searcherCategoryIndexChanged(0);

    QSettings setting;
    setting.beginGroup("static4");
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

Static4::~Static4()
{
    QSettings setting;
    setting.beginGroup("static4");
    setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("minAdvance", ui->textBoxSearcherMinAdvance->text());
    setting.setValue("maxAdvance", ui->textBoxSearcherMaxAdvance->text());
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Static4::updateProfiles()
{
    profiles = ProfileLoader4::getProfiles();
    profiles.insert(profiles.begin(), Profile4("None", Game::Diamond, 12345, 54321, false));

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("static4/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Static4::generate()
{
    generatorModel->clearModel();

    const StaticTemplate4 *staticTemplate
        = Encounters4::getStaticEncounter(ui->comboBoxGeneratorCategory->currentIndex(), ui->comboBoxGeneratorPokemon->getCurrentInt());
    generatorModel->setMethod(staticTemplate->getMethod());

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGeneratorOffset->getUInt();
    auto lead = ui->comboMenuGeneratorLead->getEnum<Lead>();

    auto filter = ui->filterGenerator->getFilter<StateFilter>();
    StaticGenerator4 generator(initialAdvances, maxAdvances, offset, staticTemplate->getMethod(), lead, *staticTemplate, *currentProfile,
                               filter);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Static4::generatorCategoryIndexChanged(int index)
{
    if (index >= 0)
    {
        int size;
        const StaticTemplate4 *templates = Encounters4::getStaticEncounters(index, &size);

        ui->comboBoxGeneratorPokemon->clear();
        for (int i = 0; i < size; i++)
        {
            if ((currentProfile->getVersion() & templates[i].getVersion()) != Game::None)
            {
                ui->comboBoxGeneratorPokemon->addItem(
                    QString::fromStdString(Translator::getSpecie(templates[i].getSpecie(), templates[i].getForm())),
                    QVariant::fromValue(i));
            }
        }
    }
}

void Static4::generatorPokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        const StaticTemplate4 *staticTemplate
            = Encounters4::getStaticEncounter(ui->comboBoxGeneratorCategory->currentIndex(), ui->comboBoxGeneratorPokemon->getCurrentInt());
        ui->spinBoxGeneratorLevel->setValue(staticTemplate->getLevel());
        if (staticTemplate->getMethod() == Method::Method1)
        {
            ui->comboMenuGeneratorLead->clearSelection();
            ui->labelGeneratorLead->setVisible(false);
            ui->comboMenuGeneratorLead->setVisible(false);
        }
        else
        {
            ui->labelGeneratorLead->setVisible(true);
            ui->comboMenuGeneratorLead->setVisible(true);

            bool flag = staticTemplate->getInfo()->getFixedGender();
            ui->comboMenuGeneratorLead->hideAction(toInt(Lead::CuteCharmF), flag);
            ui->comboMenuGeneratorLead->hideAction(toInt(Lead::CuteCharmM), flag);
        }

        ui->comboBoxGeneratorShiny->setCurrentIndex(ui->comboBoxGeneratorShiny->findData(toInt(staticTemplate->getShiny())));
    }
}

void Static4::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));

        bool hgss = (currentProfile->getVersion() & Game::HGSS) != Game::None;

        // Game Corner
        ui->comboBoxGeneratorCategory->setItemHidden(3, !hgss);
        ui->comboBoxSearcherCategory->setItemHidden(3, !hgss);

        generatorCategoryIndexChanged(ui->comboBoxGeneratorCategory->currentIndex());
        searcherCategoryIndexChanged(ui->comboBoxSearcherCategory->currentIndex());
    }
}

void Static4::profileManager()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}

void Static4::search()
{
    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    std::array<u8, 6> min = ui->filterSearcher->getMinIVs();
    std::array<u8, 6> max = ui->filterSearcher->getMaxIVs();

    u32 minAdvance = ui->textBoxSearcherMinAdvance->getUInt();
    u32 maxAdvance = ui->textBoxSearcherMaxAdvance->getUInt();
    u32 minDelay = ui->textBoxSearcherMinDelay->getUInt();
    u32 maxDelay = ui->textBoxSearcherMaxDelay->getUInt();
    auto lead = ui->comboMenuSearcherLead->getEnum<Lead>();
    const StaticTemplate4 *staticTemplate
        = Encounters4::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex(), ui->comboBoxSearcherPokemon->getCurrentInt());

    auto filter = ui->filterSearcher->getFilter<StateFilter>();
    auto *searcher
        = new StaticSearcher4(minAdvance, maxAdvance, minDelay, maxDelay, staticTemplate->getMethod(), lead, *currentProfile, filter);

    int maxProgress = 1;
    for (u8 i = 0; i < 6; i++)
    {
        maxProgress *= max[i] - min[i] + 1;
    }
    searcher->setMaxProgress(maxProgress);

    auto *thread = QThread::create([=] { searcher->startSearch(min, max, staticTemplate); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    timer->callOnTimeout(this, [=] {
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void Static4::searcherCategoryIndexChanged(int index)
{
    if (index >= 0)
    {
        int size;
        const StaticTemplate4 *templates = Encounters4::getStaticEncounters(index, &size);

        ui->comboBoxSearcherPokemon->clear();
        for (int i = 0; i < size; i++)
        {
            if ((currentProfile->getVersion() & templates[i].getVersion()) != Game::None)
            {
                ui->comboBoxSearcherPokemon->addItem(
                    QString::fromStdString(Translator::getSpecie(templates[i].getSpecie(), templates[i].getForm())),
                    QVariant::fromValue(i));
            }
        }
    }
}

void Static4::searcherPokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        const StaticTemplate4 *staticTemplate
            = Encounters4::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex(), ui->comboBoxSearcherPokemon->getCurrentInt());
        ui->spinBoxSearcherLevel->setValue(staticTemplate->getLevel());
        if (staticTemplate->getMethod() == Method::Method1)
        {
            ui->comboMenuSearcherLead->clearSelection();
            ui->labelSearcherLead->setVisible(false);
            ui->comboMenuSearcherLead->setVisible(false);
        }
        else
        {
            ui->labelSearcherLead->setVisible(true);
            ui->comboMenuSearcherLead->setVisible(true);

            bool flag = staticTemplate->getInfo()->getFixedGender();
            ui->comboMenuSearcherLead->hideAction(toInt(Lead::CuteCharmF), flag);
            ui->comboMenuSearcherLead->hideAction(toInt(Lead::CuteCharmM), flag);
        }

        ui->comboBoxSearcherShiny->setCurrentIndex(ui->comboBoxSearcherShiny->findData(toInt(staticTemplate->getShiny())));
    }
}

void Static4::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    const auto &state = searcherModel->getItem(index.row());

    auto *time = new SeedToTime4(state.getSeed(), currentProfile->getVersion());
    time->show();
}

void Static4::transferFilters(int index)
{
    if (index == 0)
    {
        ui->filterSearcher->copyFrom(ui->filterGenerator);
    }
    else
    {
        ui->filterGenerator->copyFrom(ui->filterSearcher);
    }
}

void Static4::transferSettings(int index)
{
    if (index == 0)
    {
        ui->comboBoxSearcherCategory->setCurrentIndex(ui->comboBoxGeneratorCategory->currentIndex());
        ui->comboBoxSearcherPokemon->setCurrentIndex(ui->comboBoxGeneratorPokemon->currentIndex());
    }
    else
    {
        ui->comboBoxGeneratorCategory->setCurrentIndex(ui->comboBoxSearcherCategory->currentIndex());
        ui->comboBoxGeneratorPokemon->setCurrentIndex(ui->comboBoxSearcherPokemon->currentIndex());
    }
}
