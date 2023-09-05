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
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/StaticGenerator5.hpp>
#include <Core/Gen5/IVSeedCache.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/IVSearcher5.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/StaticModel5.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

Static5::Static5(QWidget *parent) : QWidget(parent), ui(new Ui::Static5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new StaticGeneratorModel5(ui->tableViewGenerator);
    searcherModel = new StaticSearcherModel5(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorOffset->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->filterSearcher->disableControls(Controls::DisableFilter);

    ui->comboMenuGeneratorLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuGeneratorLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") },
                                        { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->comboMenuGeneratorLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboMenuSearcherLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuSearcherLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") },
                                       { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->comboMenuSearcherLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboBoxGeneratorLuckyPower->setup({ 0, 3, 3 });
    ui->comboBoxSearcherLuckyPower->setup({ 0, 3, 3 });

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Static5::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Static5::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Static5::search);
    connect(ui->comboBoxGeneratorCategory, &QComboBox::currentIndexChanged, this, &Static5::generatorCategoryIndexChanged);
    connect(ui->comboBoxSearcherCategory, &QComboBox::currentIndexChanged, this, &Static5::searcherCategoryIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, &QComboBox::currentIndexChanged, this, &Static5::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherPokemon, &QComboBox::currentIndexChanged, this, &Static5::searcherPokemonIndexChanged);
    connect(ui->textBoxSearcherInitialIVAdvances, &TextBox::textChanged, this, &Static5::searcherFastSearchChanged);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Static5::profileManager);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &StaticGeneratorModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &StaticSearcherModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::ivsChanged, this, &Static5::searcherFastSearchChanged);

    updateProfiles();
    generatorCategoryIndexChanged(0);
    searcherCategoryIndexChanged(0);
    searcherFastSearchChanged();

    QSettings setting;
    setting.beginGroup("static5");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Static5::~Static5()
{
    QSettings setting;
    setting.beginGroup("static5");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

bool Static5::hasProfiles() const
{
    return !profiles.empty();
}

void Static5::updateProfiles()
{
    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("static5/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

bool Static5::fastSearchEnabled() const
{
    u32 initialAdvances = ui->textBoxSearcherInitialIVAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxIVAdvances->getUInt();

    if (initialAdvances + maxAdvances > 5)
    {
        return false;
    }

    auto min = ui->filterSearcher->getMinIVs();
    auto max = ui->filterSearcher->getMaxIVs();

    return min[0] >= 30 && min[2] >= 30 && min[4] >= 30 && (min[1] >= 30 || min[3] >= 30) && (min[5] >= 30 || max[5] <= 1);
}

void Static5::generate()
{
    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 ivAdvances = ui->textBoxGeneratorIVAdvances->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGeneratorOffset->getUInt();
    auto lead = ui->comboMenuGeneratorLead->getEnum<Lead>();
    u8 luckyPower = 0;

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
    }
}

void Static5::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileMACAddressValue->setText(QString::number(currentProfile->getMac(), 16));
        ui->labelProfileDSTypeValue->setText(QString::fromStdString(currentProfile->getDSTypeString()));
        ui->labelProfileVCountValue->setText(QString::number(currentProfile->getVCount(), 16));
        ui->labelProfileTimer0Value->setText(QString::number(currentProfile->getTimer0Min(), 16) + "-"
                                             + QString::number(currentProfile->getTimer0Max(), 16));
        ui->labelProfileGxStatValue->setText(QString::number(currentProfile->getGxStat()));
        ui->labelProfileVFrameValue->setText(QString::number(currentProfile->getVFrame()));
        ui->labelProfileKeypressesValue->setText(QString::fromStdString(currentProfile->getKeypressesString()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));
    }
}

void Static5::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
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

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialIVAdvances = ui->textBoxSearcherInitialIVAdvances->getUInt();
    u32 maxIVAdvances = ui->textBoxSearcherMaxIVAdvances->getUInt();
    u32 initialAdvances = ui->textBoxSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    auto lead = ui->comboMenuSearcherLead->getEnum<Lead>();
    u8 luckyPower = 0;

    const StaticTemplate5 *staticTemplate
        = Encounters5::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex(), ui->comboBoxSearcherPokemon->getCurrentInt());

    auto filter = ui->filterSearcher->getFilter<StateFilter>();
    StaticGenerator5 generator(initialAdvances, maxAdvances, 0, Method::Method5, lead, luckyPower, *staticTemplate, *currentProfile,
                               filter);

    IVSearcher5<StaticGenerator5, State5> *searcher;
    if (fastSearchEnabled())
    {
        auto ivCache = IVSeedCache::getNormalCache(initialIVAdvances, maxIVAdvances, currentProfile->getVersion(), filter);
        searcher = new IVSearcher5<StaticGenerator5, State5>(initialIVAdvances, maxIVAdvances, ivCache, generator, *currentProfile);
    }
    else
    {
        searcher = new IVSearcher5<StaticGenerator5, State5>(initialIVAdvances, maxIVAdvances, generator, *currentProfile);
    }

    int maxProgress = Keypresses::getKeypresses(*currentProfile).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= (currentProfile->getTimer0Max() - currentProfile->getTimer0Min() + 1);
    ui->progressBar->setRange(0, maxProgress);

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
    }
}

void Static5::searcherFastSearchChanged()
{
    if (fastSearchEnabled())
    {
        ui->labelIVFastSearch->setText(tr("Settings are configured for fast searching."));
    }
    else
    {
        QStringList text = { tr("Settings are not configured for fast searching."), tr("Keep initial/max advances below 6."),
                             tr("Ensure IV filters are set to common spreads.") };
        ui->labelIVFastSearch->setText(text.join('\n'));
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
    }
}
