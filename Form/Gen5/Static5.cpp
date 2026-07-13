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

#include "Static5.hpp"
#include "ui_Static5.h"
#include <Form/Util/AdvanceFinder.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/StaticGenerator5.hpp>
#include <Core/Gen5/IVCache.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/SHA1Cache.hpp>
#include <Core/Gen5/Searchers/IVSearcher5.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Form/Gen5/Tools/AdjacentSeeds.hpp>
#include <Model/Gen5/StaticModel5.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QThread>
#include <QTimer>

static const QString settingPrefix = QStringLiteral("static5");

Static5::Static5(QWidget *parent) : QWidget(parent), ui(new Ui::Static5), ivCache(nullptr), shaCache(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->profileDisplay->setup(settingPrefix, Game::Gen5);

    generatorModel = new StaticGeneratorModel5(ui->tableViewGenerator);
    searcherModel = new StaticSearcherModel5(ui->tableViewSearcher);
    proxyModel = new SortFilterProxyModel(ui->tableViewSearcher, searcherModel);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(proxyModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorOffset->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->filterGenerator->disableControls(Controls::EncounterSlots | Controls::Height | Controls::Level | Controls::Weight);
    ui->filterSearcher->disableControls(Controls::DisableFilter | Controls::EncounterSlots | Controls::Height | Controls::Level
                                        | Controls::Weight);

    ui->comboMenuGeneratorLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuGeneratorLead->addMenu(tr("Cute Charm"),
                                        { { tr("♂ Lead"), toInt(Lead::CuteCharmM) }, { tr("♀ Lead"), toInt(Lead::CuteCharmF) } });
    ui->comboMenuGeneratorLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboMenuSearcherLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuSearcherLead->addMenu(tr("Cute Charm"),
                                       { { tr("♂ Lead"), toInt(Lead::CuteCharmM) }, { tr("♀ Lead"), toInt(Lead::CuteCharmF) } });
    ui->comboMenuSearcherLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboBoxGeneratorLuckyPower->setup({ 0, 3 });
    ui->comboBoxSearcherLuckyPower->setup({ 0, 3 });

    ui->comboBoxGeneratorShiny->setup({ toInt(Shiny::Never), toInt(Shiny::Random), toInt(Shiny::Always) });
    ui->comboBoxSearcherShiny->setup({ toInt(Shiny::Never), toInt(Shiny::Random), toInt(Shiny::Always) });

    auto *advanceFinder = ui->tableViewGenerator->addAction(tr("Advance Finder"));
    connect(advanceFinder, &QAction::triggered, this, &Static5::openAdvanceFinder);

    auto *adjacentSeeds = ui->tableViewSearcher->addAction(tr("Adjacent Seeds"));
    connect(adjacentSeeds, &QAction::triggered, this, &Static5::openAdjacentSeeds);

    connect(ui->profileDisplay, &ProfileDisplay5::profileChanged, this, &Static5::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay5::profilesChanged, this, &Static5::profilesChanged);
    connect(ui->tabRNGSelector, &TabWidget::transferFilters, this, &Static5::transferFilters);
    connect(ui->tabRNGSelector, &TabWidget::transferSettings, this, &Static5::transferSettings);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Static5::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Static5::search);
    connect(ui->comboBoxGeneratorCategory, &QComboBox::currentIndexChanged, this, &Static5::generatorCategoryIndexChanged);
    connect(ui->comboBoxSearcherCategory, &QComboBox::currentIndexChanged, this, &Static5::searcherCategoryIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, &QComboBox::currentIndexChanged, this, &Static5::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherPokemon, &QComboBox::currentIndexChanged, this, &Static5::searcherPokemonIndexChanged);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &StaticGeneratorModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &StaticSearcherModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::ivsChanged, this, &Static5::searcherFastSearchChanged);
    connect(ui->textBoxSearcherInitialIVAdvances, &TextBox::textChanged, this, &Static5::searcherFastSearchChanged);
    connect(ui->textBoxSearcherMaxIVAdvances, &TextBox::textChanged, this, &Static5::searcherFastSearchChanged);

    updateProfiles();
    if (hasProfiles())
    {
        generatorCategoryIndexChanged(0);
        searcherCategoryIndexChanged(0);
    }
    searcherFastSearchChanged();

    QSettings setting;
    setting.beginGroup(settingPrefix);
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    if (setting.contains("startDate"))
    {
        ui->dateEditSearcherStartDate->setDate(setting.value("startDate").toDate());
    }
    if (setting.contains("endDate"))
    {
        ui->dateEditSearcherEndDate->setDate(setting.value("endDate").toDate());
    }
    setting.endGroup();
}

Static5::~Static5()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("geometry", this->saveGeometry());
    setting.setValue("startDate", ui->dateEditSearcherStartDate->date());
    setting.setValue("endDate", ui->dateEditSearcherEndDate->date());
    setting.endGroup();

    delete ivCache;
    delete shaCache;
    delete ui;
}

bool Static5::hasProfiles() const
{
    return ui->profileDisplay->hasProfiles();
}

void Static5::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

bool Static5::fastSearchEnabled() const
{
    if (ivCache == nullptr)
    {
        return false;
    }

    u32 initialAdvances = ui->textBoxSearcherInitialIVAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxIVAdvances->getUInt();

    if (initialAdvances < ivCache->getInitialAdvances()
        || (initialAdvances + maxAdvances) > (ivCache->getInitialAdvances() + ivCache->getMaxAdvances()))
    {
        return false;
    }

    auto min = ui->filterSearcher->getMinIVs();
    auto max = ui->filterSearcher->getMaxIVs();

    const StaticTemplate5 *staticTemplate
        = Encounters5::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex(), ui->comboBoxSearcherPokemon->getCurrentInt());
    if (staticTemplate->getRoamer())
    {
        return min[0] >= 30 && min[2] >= 30 && min[4] >= 30 && min[5] >= 30 && (min[1] >= 30 || min[3] >= 30);
    }
    else
    {
        return min[0] >= 30 && min[2] >= 30 && min[4] >= 30 && (min[1] >= 30 || min[3] >= 30) && (min[5] >= 30 || max[5] <= 1);
    }
}

void Static5::generate()
{
    if (!ui->filterGenerator->isValid())
    {
        return;
    }

    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 ivAdvances = ui->textBoxGeneratorIVAdvances->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGeneratorOffset->getUInt();
    auto lead = ui->comboMenuGeneratorLead->getEnum<Lead>();
    u8 luckyPower = ui->comboBoxGeneratorLuckyPower->getCurrentUChar();

    const StaticTemplate5 *staticTemplate
        = Encounters5::getStaticEncounter(ui->comboBoxGeneratorCategory->currentIndex(), ui->comboBoxGeneratorPokemon->getCurrentInt());

    auto filter = ui->filterGenerator->getFilter<StateFilter>();
    StaticGenerator5 generator(initialAdvances, maxAdvances, offset, Method::None, lead, luckyPower, *staticTemplate, *currentProfile,
                               filter);

    auto states = generator.generate(seed, ivAdvances, 0);
    generatorModel->addItems(states);
}

void Static5::generatorCategoryIndexChanged(int index)
{
    if (index >= 0)
    {
        int size;
        const StaticTemplate5 *templates = Encounters5::getStaticEncounters(index, &size);

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

        bool flag = index >= 3 && index <= 5; // Only allow leads for stationary, legends, and event
        ui->labelGeneratorLead->setVisible(flag);
        ui->comboMenuGeneratorLead->setVisible(flag);
    }
}

void Static5::generatorPokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        const StaticTemplate5 *staticTemplate
            = Encounters5::getStaticEncounter(ui->comboBoxGeneratorCategory->currentIndex(), ui->comboBoxGeneratorPokemon->getCurrentInt());
        ui->spinBoxGeneratorLevel->setValue(staticTemplate->getLevel());
        ui->comboBoxGeneratorShiny->setCurrentIndex(ui->comboBoxGeneratorShiny->findData(toInt(staticTemplate->getShiny())));

        bool flag = staticTemplate->getInfo()->getFixedGender();
        ui->comboMenuGeneratorLead->hideAction(toInt(Lead::CuteCharmF), flag);
        ui->comboMenuGeneratorLead->hideAction(toInt(Lead::CuteCharmM), flag);

        flag = (currentProfile->getVersion() & Game::BW2) != Game::None && staticTemplate->getWild();
        ui->labelGeneratorLuckyPower->setHidden(!flag);
        ui->comboBoxGeneratorLuckyPower->setHidden(!flag);
    }
}

void Static5::openAdjacentSeeds()
{
    QModelIndex index = proxyModel->mapToSource(ui->tableViewSearcher->currentIndex());
    const auto &state = searcherModel->getItem(index.row());
    const StaticTemplate5 *staticTemplate
        = Encounters5::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex(), ui->comboBoxSearcherPokemon->getCurrentInt());

    auto *window = new AdjacentSeeds(staticTemplate->getRoamer(), state.getButtons(), state.getDateTime(), *currentProfile);
    window->show();
}

void Static5::profileChanged(const Profile5 &profile)
{
    currentProfile = &profile;

    if (ivCache)
    {
        delete ivCache;
        ivCache = nullptr;
    }

    if (shaCache)
    {
        delete shaCache;
        shaCache = nullptr;
    }

    auto ivCachePath = currentProfile->getIVCache();
    if (!ivCachePath.empty())
    {
        ivCache = new IVCache(ivCachePath);
    }

    auto shaCachePath = currentProfile->getSHACache();
    if (!shaCachePath.empty())
    {
        shaCache = new SHA1Cache(shaCachePath);
        ui->dateEditSearcherStartDate->setDateRange(shaCache->getStartDate(), shaCache->getEndDate());
        ui->dateEditSearcherEndDate->setDateRange(shaCache->getStartDate(), shaCache->getEndDate());
    }
    else
    {
        ui->dateEditSearcherStartDate->clearDateRange();
        ui->dateEditSearcherEndDate->clearDateRange();
    }

    bool bw = (currentProfile->getVersion() & Game::BW) != Game::None;

    ui->labelGeneratorLuckyPower->setHidden(!bw);
    ui->labelSearcherLuckyPower->setHidden(!bw);
    ui->comboBoxGeneratorLuckyPower->setHidden(!bw);
    ui->comboBoxSearcherLuckyPower->setHidden(!bw);

    // Event
    ui->comboBoxGeneratorCategory->setItemHidden(5, !bw);
    ui->comboBoxSearcherCategory->setItemHidden(5, !bw);

    // Roamer
    ui->comboBoxGeneratorCategory->setItemHidden(6, !bw);
    ui->comboBoxSearcherCategory->setItemHidden(6, !bw);

    // Curtis
    ui->comboBoxGeneratorCategory->setItemHidden(7, bw);
    ui->comboBoxSearcherCategory->setItemHidden(7, bw);

    // Yancy
    ui->comboBoxGeneratorCategory->setItemHidden(8, bw);
    ui->comboBoxSearcherCategory->setItemHidden(8, bw);

    generatorCategoryIndexChanged(ui->comboBoxGeneratorCategory->currentIndex());
    searcherCategoryIndexChanged(ui->comboBoxSearcherCategory->currentIndex());

    searcherFastSearchChanged();
}

void Static5::search()
{
    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();
    if (start > end)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid date range"), tr("Start date is after end date"));
        msg.exec();
        return;
    }

    if (!ui->filterSearcher->isValid())
    {
        return;
    }

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialIVAdvances = ui->textBoxSearcherInitialIVAdvances->getUInt();
    u32 maxIVAdvances = ui->textBoxSearcherMaxIVAdvances->getUInt();
    u32 initialAdvances = ui->textBoxSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    auto lead = ui->comboMenuSearcherLead->getEnum<Lead>();
    u8 luckyPower = ui->comboBoxSearcherLuckyPower->getCurrentUChar();

    const StaticTemplate5 *staticTemplate
        = Encounters5::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex(), ui->comboBoxSearcherPokemon->getCurrentInt());

    auto filter = ui->filterSearcher->getFilter<StateFilter>();
    StaticGenerator5 generator(initialAdvances, maxAdvances, 0, Method::Method5, lead, luckyPower, *staticTemplate, *currentProfile,
                               filter);

    SearcherBase5<StaticGenerator5, State5> *searcher;
    if (fastSearchEnabled())
    {
        CacheType type = staticTemplate->getRoamer() ? CacheType::Roamer : CacheType::Normal;
        auto ivMap = ivCache->getCache(initialIVAdvances, maxIVAdvances, currentProfile->getVersion(), type, filter);
        if (shaCache && shaCache->isValid(*currentProfile))
        {
            auto shaMap = shaCache->getCache(initialAdvances, maxIVAdvances, start, end, ivMap, type, *currentProfile);
            searcher = new IVSearcher5CacheFast<StaticGenerator5, State5>(initialIVAdvances, maxIVAdvances, shaMap, ivMap, generator,
                                                                          *currentProfile);
        }
        else
        {
            searcher = new IVSearcher5Fast<StaticGenerator5, State5>(initialIVAdvances, maxIVAdvances, ivMap, generator, *currentProfile);
        }
    }
    else
    {
        searcher = new IVSearcher5<StaticGenerator5, State5>(initialIVAdvances, maxIVAdvances, generator, *currentProfile);
    }

    searcher->setMaxProgress(searcher->getMaxProgress(start, end));

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads, start, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] {
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

void Static5::searcherCategoryIndexChanged(int index)
{
    if (index >= 0)
    {
        int size;
        const StaticTemplate5 *templates = Encounters5::getStaticEncounters(index, &size);

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

        bool flag = index >= 3 && index <= 5; // Only allow leads for stationary, legends, and event
        ui->labelSearcherLead->setVisible(flag);
        ui->comboMenuSearcherLead->setVisible(flag);
    }
}

void Static5::searcherFastSearchChanged()
{
    if (fastSearchEnabled())
    {
        if (shaCache && shaCache->isValid(*currentProfile))
        {
            ui->labelIVFastSearch->setText(tr("Settings are configured for fast IV/SHA searching"));
        }
        else
        {
            ui->labelIVFastSearch->setText(
                tr("Settings are configured for fast IV searching.\nProfile is missing or has an incompatible SHA cache."));
        }
    }
    else
    {
        if (ivCache == nullptr)
        {
            ui->labelIVFastSearch->setText(tr("Profile does not have a IV cache file configured"));
        }
        else
        {
            QStringList text
                = { tr("Settings are not configured for fast searching"),
                    tr("Keep initial/max advances below %1/%2").arg(ivCache->getInitialAdvances()).arg(ivCache->getMaxAdvances()),
                    tr("Ensure IV filters are set to common spreads") };
            ui->labelIVFastSearch->setText(text.join('\n'));
        }
    }
}

void Static5::searcherPokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        const StaticTemplate5 *staticTemplate
            = Encounters5::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex(), ui->comboBoxSearcherPokemon->getCurrentInt());
        ui->spinBoxSearcherLevel->setValue(staticTemplate->getLevel());
        ui->comboBoxSearcherShiny->setCurrentIndex(ui->comboBoxSearcherShiny->findData(toInt(staticTemplate->getShiny())));

        bool flag = staticTemplate->getInfo()->getFixedGender();
        ui->comboMenuSearcherLead->hideAction(toInt(Lead::CuteCharmF), flag);
        ui->comboMenuSearcherLead->hideAction(toInt(Lead::CuteCharmM), flag);

        flag = (currentProfile->getVersion() & Game::BW2) != Game::None && staticTemplate->getWild();
        ui->labelSearcherLuckyPower->setHidden(!flag);
        ui->comboBoxSearcherLuckyPower->setHidden(!flag);
    }
}

void Static5::transferFilters(int index)
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

void Static5::transferSettings(int index)
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

void Static5::openAdvanceFinder()
{
    auto *advanceFinder = new AdvanceFinder(generatorModel, ui->tableViewGenerator, this);
    advanceFinder->show();
}
