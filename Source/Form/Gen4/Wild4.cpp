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

#include "Wild4.hpp"
#include "ui_Wild4.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Generators/WildGenerator4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/Searchers/WildSearcher4.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen4/Profile/ProfileManager4.hpp>
#include <Form/Gen4/Tools/SeedToTime4.hpp>
#include <Model/Gen4/WildModel4.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

Wild4::Wild4(QWidget *parent) : QWidget(parent), ui(new Ui::Wild4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new WildGeneratorModel4(ui->tableViewGenerator, Method::MethodJ);
    searcherModel = new WildSearcherModel4(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorDelay->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherMinDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMinAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvance->setValues(InputType::Advance32Bit);

    ui->filterSearcher->disableControls(Controls::DisableFilter);

    ui->comboMenuGeneratorLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuGeneratorLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") },
                                        { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->comboMenuGeneratorLead->addMenu(
        tr("Encounter Modifier"), { tr("Arena Trap"), tr("Illuminate"), tr("No Guard"), tr("Sticky Hold"), tr("Suction Cups") },
        { toInt(Lead::ArenaTrap), toInt(Lead::Illuminate), toInt(Lead::NoGuard), toInt(Lead::StickyHold), toInt(Lead::SuctionCups) });
    ui->comboMenuGeneratorLead->addAction(tr("Compound Eyes"), toInt(Lead::CompoundEyes));
    ui->comboMenuGeneratorLead->addMenu(tr("Slot Modifier"), { tr("Magnet Pull"), tr("Static") },
                                        { toInt(Lead::MagnetPull), toInt(Lead::Static) });
    ui->comboMenuGeneratorLead->addMenu(tr("Level Modifier"), { tr("Hustle"), tr("Pressure"), tr("Vital Spirit") },
                                        { toInt(Lead::Hustle), toInt(Lead::Pressure), toInt(Lead::VitalSpirit) });
    ui->comboMenuGeneratorLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboMenuSearcherLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuSearcherLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") },
                                       { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->comboMenuSearcherLead->addMenu(
        tr("Encounter Modifier"), { tr("Arena Trap"), tr("Illuminate"), tr("No Guard"), tr("Sticky Hold"), tr("Suction Cups") },
        { toInt(Lead::ArenaTrap), toInt(Lead::Illuminate), toInt(Lead::NoGuard), toInt(Lead::StickyHold), toInt(Lead::SuctionCups) });
    ui->comboMenuSearcherLead->addAction(tr("Compound Eyes"), toInt(Lead::CompoundEyes));
    ui->comboMenuSearcherLead->addMenu(tr("Slot Modifier"), { tr("Magnet Pull"), tr("Static") },
                                       { toInt(Lead::MagnetPull), toInt(Lead::Static) });
    ui->comboMenuSearcherLead->addMenu(tr("Level Modifier"), { tr("Hustle"), tr("Pressure"), tr("Vital Spirit") },
                                       { toInt(Lead::Hustle), toInt(Lead::Pressure), toInt(Lead::VitalSpirit) });
    ui->comboMenuSearcherLead->addAction(tr("Synchronize"), toInt(Lead::Synchronize));

    ui->comboBoxGeneratorEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::RockSmash), toInt(Encounter::BugCatchingContest),
                                            toInt(Encounter::Headbutt), toInt(Encounter::HeadbuttAlt), toInt(Encounter::HeadbuttSpecial),
                                            toInt(Encounter::Surfing), toInt(Encounter::OldRod), toInt(Encounter::GoodRod),
                                            toInt(Encounter::SuperRod) });

    ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::RockSmash), toInt(Encounter::BugCatchingContest),
                                           toInt(Encounter::Headbutt), toInt(Encounter::HeadbuttAlt), toInt(Encounter::HeadbuttSpecial),
                                           toInt(Encounter::Surfing), toInt(Encounter::OldRod), toInt(Encounter::GoodRod),
                                           toInt(Encounter::SuperRod) });

    ui->comboBoxGeneratorDualSlot->setup(
        { toInt(Game::Ruby), toInt(Game::Sapphire), toInt(Game::FireRed), toInt(Game::LeafGreen), toInt(Game::Emerald) });

    ui->comboBoxSearcherDualSlot->setup(
        { toInt(Game::Ruby), toInt(Game::Sapphire), toInt(Game::FireRed), toInt(Game::LeafGreen), toInt(Game::Emerald) });

    ui->checkBoxGeneratorPokeRadarShiny->setVisible(false);
    ui->checkBoxSearcherPokeRadarShiny->setVisible(false);

    ui->comboBoxGeneratorLocation->enableAutoComplete();
    ui->comboBoxSearcherLocation->enableAutoComplete();

    ui->comboBoxGeneratorHappiness->setup({ 0, 20, 30, 40, 50 });
    ui->comboBoxSearcherHappiness->setup({ 0, 20, 30, 40, 50 });

    auto *seedToTime = new QAction(tr("Generate times for seed"), ui->tableViewSearcher);
    connect(seedToTime, &QAction::triggered, this, &Wild4::seedToTime);
    ui->tableViewSearcher->addAction(seedToTime);

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Wild4::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Wild4::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Wild4::search);
    connect(ui->comboBoxGeneratorEncounter, &QComboBox::currentIndexChanged, this, &Wild4::generatorEncounterIndexChanged);
    connect(ui->comboBoxSearcherEncounter, &QComboBox::currentIndexChanged, this, &Wild4::searcherEncounterIndexChanged);
    connect(ui->comboBoxGeneratorLocation, &QComboBox::currentIndexChanged, this, &Wild4::generatorLocationIndexChanged);
    connect(ui->comboBoxSearcherLocation, &QComboBox::currentIndexChanged, this, &Wild4::searcherLocationIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, &QComboBox::currentIndexChanged, this, &Wild4::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherPokemon, &QComboBox::currentIndexChanged, this, &Wild4::searcherPokemonIndexChanged);
    connect(ui->comboBoxGeneratorTime, &QComboBox::currentIndexChanged, this, [=] { generatorEncounterUpdate(); });
    connect(ui->comboBoxSearcherTime, &QComboBox::currentIndexChanged, this, [=] { searcherEncounterUpdate(); });
    connect(ui->comboBoxGeneratorDualSlot, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->checkBoxGeneratorDualSlot->isChecked())
        {
            generatorEncounterUpdate();
        }
    });
    connect(ui->comboBoxSearcherDualSlot, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->checkBoxSearcherDualSlot->isChecked())
        {
            searcherEncounterUpdate();
        }
    });
    connect(ui->comboBoxGeneratorRadio, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->checkBoxGeneratorRadio->isChecked())
        {
            generatorEncounterUpdate();
        }
    });
    connect(ui->comboBoxSearcherRadio, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->checkBoxSearcherRadio->isChecked())
        {
            searcherEncounterUpdate();
        }
    });
    connect(ui->comboBoxGeneratorReplacement0, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->checkBoxGeneratorReplacement->isChecked())
        {
            generatorEncounterUpdate();
        }
    });
    connect(ui->comboBoxGeneratorReplacement1, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->checkBoxGeneratorReplacement->isChecked())
        {
            generatorEncounterUpdate();
        }
    });
    connect(ui->comboBoxSearcherReplacement0, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->checkBoxSearcherReplacement->isChecked())
        {
            searcherEncounterUpdate();
        }
    });
    connect(ui->comboBoxSearcherReplacement1, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->checkBoxSearcherReplacement->isChecked())
        {
            searcherEncounterUpdate();
        }
    });
    connect(ui->buttonGroupGenerator, &QButtonGroup::buttonClicked, this, [=] { generatorEncounterUpdate(); });
    connect(ui->buttonGroupSearcher, &QButtonGroup::buttonClicked, this, [=] { searcherEncounterUpdate(); });
    connect(ui->checkBoxGeneratorPokeRadar, &QCheckBox::stateChanged, this, &Wild4::generatorPokeRadarStateChanged);
    connect(ui->checkBoxSearcherPokeRadar, &QCheckBox::stateChanged, this, &Wild4::searcherPokeRadarStateChanged);
    connect(ui->spinBoxGeneratorPlainsBlock, &QSpinBox::valueChanged, this, [=] { generatorEncounterUpdate(); });
    connect(ui->spinBoxGeneratorForestBlock, &QSpinBox::valueChanged, this, [=] { generatorEncounterUpdate(); });
    connect(ui->spinBoxGeneratorPeakBlock, &QSpinBox::valueChanged, this, [=] { generatorEncounterUpdate(); });
    connect(ui->spinBoxGeneratorWaterBlock, &QSpinBox::valueChanged, this, [=] { generatorEncounterUpdate(); });
    connect(ui->spinBoxSearcherPlainsBlock, &QSpinBox::valueChanged, this, [=] { searcherEncounterUpdate(); });
    connect(ui->spinBoxSearcherForestBlock, &QSpinBox::valueChanged, this, [=] { searcherEncounterUpdate(); });
    connect(ui->spinBoxSearcherPeakBlock, &QSpinBox::valueChanged, this, [=] { searcherEncounterUpdate(); });
    connect(ui->spinBoxSearcherWaterBlock, &QSpinBox::valueChanged, this, [=] { searcherEncounterUpdate(); });
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &WildGeneratorModel4::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &WildSearcherModel4::setShowStats);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Wild4::profileManager);

    updateProfiles();
    generatorEncounterIndexChanged(0);
    searcherEncounterIndexChanged(0);

    QSettings setting;
    setting.beginGroup("wild4");
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

Wild4::~Wild4()
{
    QSettings setting;
    setting.beginGroup("wild4");
    setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("minAdvance", ui->textBoxSearcherMinAdvance->text());
    setting.setValue("maxAdvance", ui->textBoxSearcherMaxAdvance->text());
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Wild4::updateProfiles()
{
    profiles = ProfileLoader4::getProfiles();
    profiles.insert(profiles.begin(), Profile4("None", Game::Diamond, 12345, 54321, false));

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("wild4/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Wild4::updateEncounterGenerator()
{
    auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();

    EncounterSettings4 settings = {};
    if ((currentProfile->getVersion() & Game::DPPt) != Game::None)
    {
        settings.dppt.dual = ui->checkBoxGeneratorDualSlot->isChecked() ? ui->comboBoxGeneratorDualSlot->getEnum<Game>() : Game::None;
        if (ui->checkBoxGeneratorReplacement->isChecked())
        {
            settings.dppt.replacement[0] = ui->comboBoxGeneratorReplacement0->getCurrentUShort();
            settings.dppt.replacement[1]
                = ui->comboBoxGeneratorReplacement1->count() > 0 ? ui->comboBoxGeneratorReplacement1->getCurrentUShort() : 0;
        }
        settings.dppt.radar = ui->checkBoxGeneratorPokeRadar->isChecked();
    }
    else
    {
        settings.hgss.radio = ui->checkBoxGeneratorRadio->isChecked() ? ui->comboBoxGeneratorRadio->currentIndex() + 1 : 0;
        settings.hgss.blocks
            = { 0, static_cast<u8>(ui->spinBoxGeneratorPlainsBlock->value()), static_cast<u8>(ui->spinBoxGeneratorForestBlock->value()),
                static_cast<u8>(ui->spinBoxGeneratorPeakBlock->value()), static_cast<u8>(ui->spinBoxGeneratorWaterBlock->value()) };
    }
    settings.time = ui->comboBoxGeneratorTime->currentIndex();
    settings.swarm = ui->checkBoxGeneratorSwarm->isChecked();

    encounterGenerator = Encounters4::getEncounters(encounter, settings, currentProfile);
}

void Wild4::updateEncounterSearcher()
{
    auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();

    EncounterSettings4 settings = {};
    if ((currentProfile->getVersion() & Game::DPPt) != Game::None)
    {
        settings.dppt.dual = ui->checkBoxSearcherDualSlot->isChecked() ? ui->comboBoxSearcherDualSlot->getEnum<Game>() : Game::None;
        if (ui->checkBoxSearcherReplacement->isChecked())
        {
            settings.dppt.replacement[0] = ui->comboBoxSearcherReplacement0->getCurrentUShort();
            settings.dppt.replacement[1]
                = ui->comboBoxSearcherReplacement1->count() > 0 ? ui->comboBoxSearcherReplacement1->getCurrentUShort() : 0;
        }
        settings.dppt.radar = ui->checkBoxSearcherPokeRadar->isChecked();
    }
    else
    {
        settings.hgss.radio = ui->checkBoxSearcherRadio->isChecked() ? ui->comboBoxSearcherRadio->currentIndex() + 1 : 0;
        settings.hgss.blocks
            = { 0, static_cast<u8>(ui->spinBoxSearcherPlainsBlock->value()), static_cast<u8>(ui->spinBoxSearcherForestBlock->value()),
                static_cast<u8>(ui->spinBoxSearcherPeakBlock->value()), static_cast<u8>(ui->spinBoxSearcherWaterBlock->value()) };
    }
    settings.time = ui->comboBoxSearcherTime->currentIndex();
    settings.swarm = ui->checkBoxSearcherSwarm->isChecked();

    encounterSearcher = Encounters4::getEncounters(encounter, settings, currentProfile);
}

void Wild4::generate()
{
    Method method;
    u8 radarSlot = 0;
    if ((currentProfile->getVersion() & Game::DPPt) != Game::None)
    {
        if (ui->checkBoxGeneratorPokeRadar->isChecked())
        {
            method = Method::PokeRadar;
            std::array<bool, 12> encounters = ui->filterGenerator->getEncounterSlots();
            if (std::count(encounters.begin(), encounters.end(), true) != 1)
            {
                QMessageBox msg(QMessageBox::Warning, tr("Too many slots selected"), tr("Please a single encounter slot for Poke Radar"));
                msg.exec();
                return;
            }
            else
            {
                radarSlot = std::find(encounters.begin(), encounters.end(), true) - encounters.begin();
            }
        }
        else
        {
            method = Method::MethodJ;
        }
    }
    else
    {
        method = Method::MethodK;
    }

    generatorModel->clearModel();
    generatorModel->setMethod(method);

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 delay = ui->textBoxGeneratorDelay->getUInt();
    auto lead = ui->comboMenuGeneratorLead->getEnum<Lead>();
    bool chained = ui->checkBoxGeneratorPokeRadarShiny->isChecked();
    bool unownRadio = ui->checkBoxGeneratorRadio->isChecked() && ui->comboBoxGeneratorRadio->currentIndex() == 2;
    u8 happiness = ui->comboBoxGeneratorHappiness->getCurrentUChar();

    WildStateFilter filter = ui->filterGenerator->getFilter<WildStateFilter, true>();
    WildGenerator4 generator(initialAdvances, maxAdvances, delay, method, lead, chained, unownRadio, happiness,
                             encounterGenerator[ui->comboBoxGeneratorLocation->getCurrentInt()], *currentProfile, filter);

    auto states = generator.generate(seed, radarSlot);
    generatorModel->addItems(states);
}

void Wild4::generatorEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();
        switch (encounter)
        {
        case Encounter::Grass:
            ui->filterGenerator->setEncounterSlots(12);
            break;
        case Encounter::Surfing:
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            ui->filterGenerator->setEncounterSlots(5);
            break;
        case Encounter::RockSmash:
            ui->filterGenerator->setEncounterSlots(2);
            break;
        case Encounter::BugCatchingContest:
            ui->filterGenerator->setEncounterSlots(10);
            break;
        case Encounter::Headbutt:
        case Encounter::HeadbuttAlt:
        case Encounter::HeadbuttSpecial:
            ui->filterGenerator->setEncounterSlots(6);
            break;
        default:
            break;
        }

        bool bug = encounter == Encounter::BugCatchingContest;
        bool fish = encounter == Encounter::OldRod || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;
        bool grass = encounter == Encounter::Grass;
        bool hgss = (currentProfile->getVersion() & Game::HGSS) != Game::None;
        bool swarm = encounter == Encounter::Grass || encounter == Encounter::Surfing || encounter == Encounter::OldRod
            || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;

        ui->labelGeneratorHappiness->setVisible(hgss && fish);
        ui->comboBoxGeneratorHappiness->setVisible(hgss && fish);

        ui->checkBoxGeneratorDualSlot->setVisible(!hgss && grass);
        ui->comboBoxGeneratorDualSlot->setVisible(!hgss && grass);
        if (!ui->checkBoxGeneratorDualSlot->isVisible())
        {
            ui->checkBoxGeneratorDualSlot->setChecked(false);
        }

        ui->checkBoxGeneratorPokeRadar->setVisible(!hgss && grass);
        if (!ui->checkBoxGeneratorPokeRadar->isVisible())
        {
            ui->checkBoxGeneratorPokeRadar->setChecked(false);
            ui->checkBoxGeneratorPokeRadarShiny->setChecked(false);
        }

        ui->checkBoxGeneratorRadio->setVisible(hgss && grass);
        ui->comboBoxGeneratorRadio->setVisible(hgss && grass);
        if (!ui->checkBoxGeneratorRadio->isVisible())
        {
            ui->checkBoxGeneratorRadio->setChecked(false);
        }

        ui->checkBoxGeneratorSwarm->setVisible((!hgss && grass) || (hgss && swarm));
        if (!ui->checkBoxGeneratorSwarm->isVisible())
        {
            ui->checkBoxGeneratorSwarm->setChecked(false);
        }

        ui->labelGeneratorTime->setVisible((!hgss && grass) || hgss);
        ui->comboBoxGeneratorTime->setVisible((!hgss && grass) || hgss);

        ui->comboMenuGeneratorLead->hideAction(toInt(Lead::MagnetPull), bug);
        ui->comboMenuGeneratorLead->hideAction(toInt(Lead::Static), bug);
        ui->comboMenuGeneratorLead->hideAction(toInt(Lead::Pressure), bug); // Also handles Hustle and Vital Spirit

        updateEncounterGenerator();

        std::vector<u16> locs;
        std::transform(encounterGenerator.begin(), encounterGenerator.end(), std::back_inserter(locs),
                       [](const EncounterArea4 &area) { return area.getLocation(); });

        auto locations = Translator::getLocations(locs, currentProfile->getVersion());
        std::vector<int> indices(locations.size());
        std::iota(indices.begin(), indices.end(), 0);

        if (!bug)
        {
            std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });
        }

        ui->comboBoxGeneratorLocation->clear();
        for (int i : indices)
        {
            ui->comboBoxGeneratorLocation->addItem(QString::fromStdString(locations[i]), i);
        }
    }
}

void Wild4::generatorEncounterUpdate()
{
    updateEncounterGenerator();
    generatorLocationIndexChanged(0);
}

void Wild4::generatorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounterGenerator[ui->comboBoxGeneratorLocation->getCurrentInt()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();
        bool greatMarsh = area.greatMarsh(currentProfile->getVersion());
        bool safari = area.safariZone(currentProfile->getVersion());
        bool trophyGarden = area.trophyGarden(currentProfile->getVersion());

        ui->checkBoxGeneratorReplacement->setVisible(greatMarsh || trophyGarden);
        ui->comboBoxGeneratorReplacement0->setVisible(greatMarsh || trophyGarden);
        ui->comboBoxGeneratorReplacement1->setVisible(trophyGarden);

        ui->labelGeneratorPlainsBlock->setVisible(safari);
        ui->spinBoxGeneratorPlainsBlock->setVisible(safari);
        ui->labelGeneratorForestBlock->setVisible(safari);
        ui->spinBoxGeneratorForestBlock->setVisible(safari);
        ui->labelGeneratorPeakBlock->setVisible(safari);
        ui->spinBoxGeneratorPeakBlock->setVisible(safari);
        ui->labelGeneratorWaterBlock->setVisible(safari);
        ui->spinBoxGeneratorWaterBlock->setVisible(safari);

        ui->comboBoxGeneratorPokemon->clear();
        ui->comboBoxGeneratorPokemon->addItem(QString("-"));
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxGeneratorPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }

        if (greatMarsh && index != 0)
        {
            // Block signals so we don't cause infinite signal recursion
            ui->comboBoxGeneratorReplacement0->blockSignals(true);

            ui->comboBoxGeneratorReplacement0->clear();
            for (u16 specie : Encounters4::getGreatMarshPokemon(currentProfile))
            {
                if (specie == 0)
                {
                    break;
                }
                ui->comboBoxGeneratorReplacement0->addItem(QString::fromStdString(Translator::getSpecie(specie)), specie);
            }

            ui->comboBoxGeneratorReplacement0->blockSignals(false);
        }
        else if (trophyGarden && index != 0)
        {
            // Block signals so we don't cause infinite signal recursion
            ui->comboBoxGeneratorReplacement0->blockSignals(true);
            ui->comboBoxGeneratorReplacement1->blockSignals(true);

            ui->comboBoxGeneratorReplacement0->clear();
            ui->comboBoxGeneratorReplacement1->clear();
            for (u16 specie : Encounters4::getTrophyGardenPokemon(currentProfile))
            {
                const auto &name = Translator::getSpecie(specie);
                ui->comboBoxGeneratorReplacement0->addItem(QString::fromStdString(name), specie);
                ui->comboBoxGeneratorReplacement1->addItem(QString::fromStdString(name), specie);
            }

            ui->comboBoxGeneratorReplacement0->blockSignals(false);
            ui->comboBoxGeneratorReplacement1->blockSignals(false);
        }

        // Account for safari zone not being its own encounter method
        if (safari)
        {
            ui->filterGenerator->setEncounterSlots(10);
            ui->labelGeneratorTime->setVisible(true);
            ui->comboBoxGeneratorTime->setVisible(true);
        }
        else
        {
            if ((currentProfile->getVersion() & Game::HGSS) != Game::None)
            {
                auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();
                bool time = encounter == Encounter::Grass || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;
                ui->labelGeneratorTime->setVisible(time);
                ui->comboBoxGeneratorTime->setVisible(time);
            }
        }
    }
}

void Wild4::generatorPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterGenerator->resetEncounterSlots();
    }
    else
    {
        u16 num = ui->comboBoxGeneratorPokemon->getCurrentUShort();
        auto flags = encounterGenerator[ui->comboBoxGeneratorLocation->getCurrentInt()].getSlots(num);
        ui->filterGenerator->toggleEncounterSlots(flags);
    }
}

void Wild4::generatorPokeRadarStateChanged(int state)
{
    ui->checkBoxGeneratorPokeRadarShiny->setVisible(state == Qt::Checked);
    if (!ui->checkBoxGeneratorPokeRadarShiny->isVisible())
    {
        ui->checkBoxGeneratorPokeRadarShiny->setChecked(false);
    }
    ui->comboMenuGeneratorLead->hideAction(toInt(Lead::MagnetPull), state == Qt::Checked);
    ui->comboMenuGeneratorLead->hideAction(toInt(Lead::Hustle), state == Qt::Checked); // Also handles Pressure and Vital Spirit
    ui->comboMenuGeneratorLead->hideAction(toInt(Lead::Static), state == Qt::Checked);
}

void Wild4::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));
        ui->labelProfileNationalDexValue->setText(currentProfile->getNationalDex() ? tr("Yes") : tr("No"));

        bool hgss = (currentProfile->getVersion() & Game::HGSS) != Game::None;

        ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::RockSmash)), !hgss);
        ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::BugCatchingContest)),
                                                      !hgss);
        ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::Headbutt)), !hgss);
        ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::HeadbuttAlt)), !hgss);
        ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::HeadbuttSpecial)), !hgss);
        ui->comboMenuGeneratorLead->hideAction(toInt(Lead::ArenaTrap), !hgss); // Also handles Illuminate and No Guard
        ui->comboMenuGeneratorLead->hideAction(toInt(Lead::StickyHold), !hgss); // Also handles Suction Cups

        ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::RockSmash)), !hgss);
        ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::BugCatchingContest)), !hgss);
        ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::Headbutt)), !hgss);
        ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::HeadbuttAlt)), !hgss);
        ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::HeadbuttSpecial)), !hgss);
        ui->comboMenuSearcherLead->hideAction(toInt(Lead::ArenaTrap), !hgss); // Also handles Illuminate and No Guard
        ui->comboMenuSearcherLead->hideAction(toInt(Lead::StickyHold), !hgss); // Also handles Suction Cups

        generatorEncounterIndexChanged(0);
        searcherEncounterIndexChanged(0);
    }
}

void Wild4::profileManager()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}

void Wild4::search()
{
    Method method;
    u8 radarSlot = 0;
    if ((currentProfile->getVersion() & Game::DPPt) != Game::None)
    {
        if (ui->checkBoxSearcherPokeRadar->isChecked())
        {
            method = Method::PokeRadar;
            std::array<bool, 12> encounters = ui->filterSearcher->getEncounterSlots();
            if (std::count(encounters.begin(), encounters.end(), true) != 1)
            {
                QMessageBox msg(QMessageBox::Warning, tr("Too many slots selected"), tr("Please a single encounter slot for Poke Radar"));
                msg.exec();
                return;
            }
            else
            {
                radarSlot = std::find(encounters.begin(), encounters.end(), true) - encounters.begin();
            }
        }
        else
        {
            method = Method::MethodJ;
        }
    }
    else
    {
        method = Method::MethodK;
    }

    std::array<u8, 6> min = ui->filterSearcher->getMinIVs();
    std::array<u8, 6> max = ui->filterSearcher->getMaxIVs();

    auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();
    auto &area = encounterSearcher[ui->comboBoxSearcherLocation->getCurrentInt()];
    if (encounter == Encounter::BugCatchingContest || area.safariZone(currentProfile->getVersion()))
    {
        bool flag = std::any_of(min.begin(), min.end(), [](u8 iv) { return iv == 31; });
        if (!flag)
        {
            QMessageBox msg(QMessageBox::Warning, tr("Missing Flawless IV"),
                            tr("Bug Catching Contest Searcher needs at least one IV at 31"));
            msg.exec();
            return;
        }
    }

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 minAdvance = ui->textBoxSearcherMinAdvance->getUInt();
    u32 maxAdvance = ui->textBoxSearcherMaxAdvance->getUInt();
    u32 minDelay = ui->textBoxSearcherMinDelay->getUInt();
    u32 maxDelay = ui->textBoxSearcherMaxDelay->getUInt();
    auto lead = ui->comboMenuSearcherLead->getEnum<Lead>();
    bool shiny = ui->checkBoxSearcherPokeRadarShiny->isChecked();
    bool unownRadio = ui->checkBoxSearcherRadio->isChecked() && ui->comboBoxSearcherRadio->currentIndex() == 2;
    u8 happiness = ui->comboBoxSearcherHappiness->getCurrentUChar();

    WildStateFilter filter = ui->filterSearcher->getFilter<WildStateFilter, true>();
    auto *searcher = new WildSearcher4(minAdvance, maxAdvance, minDelay, maxDelay, method, lead, shiny, unownRadio, happiness, area,
                                       *currentProfile, filter);

    int maxProgress = 1;
    for (u8 i = 0; i < 6; i++)
    {
        maxProgress *= max[i] - min[i] + 1;
    }
    ui->progressBar->setRange(0, maxProgress);

    auto *thread = QThread::create([=] { searcher->startSearch(min, max, radarSlot); });
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

void Wild4::searcherEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();
        switch (encounter)
        {
        case Encounter::Grass:
            ui->filterSearcher->setEncounterSlots(12);
            break;
        case Encounter::Surfing:
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            ui->filterSearcher->setEncounterSlots(5);
            break;
        case Encounter::RockSmash:
            ui->filterSearcher->setEncounterSlots(2);
            break;
        case Encounter::BugCatchingContest:
            ui->filterSearcher->setEncounterSlots(10);
            break;
        case Encounter::Headbutt:
        case Encounter::HeadbuttAlt:
        case Encounter::HeadbuttSpecial:
            ui->filterSearcher->setEncounterSlots(6);
            break;
        default:
            break;
        }

        bool bug = encounter == Encounter::BugCatchingContest;
        bool fish = encounter == Encounter::OldRod || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;
        bool grass = encounter == Encounter::Grass;
        bool hgss = (currentProfile->getVersion() & Game::HGSS) != Game::None;
        bool swarm = encounter == Encounter::Grass || encounter == Encounter::Surfing || encounter == Encounter::OldRod
            || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;

        ui->labelSearcherHappiness->setVisible(hgss && fish);
        ui->comboBoxSearcherHappiness->setVisible(hgss && fish);

        ui->checkBoxSearcherDualSlot->setVisible(!hgss && grass);
        ui->comboBoxSearcherDualSlot->setVisible(!hgss && grass);
        if (!ui->checkBoxSearcherDualSlot->isVisible())
        {
            ui->checkBoxSearcherDualSlot->setChecked(false);
        }

        ui->checkBoxSearcherPokeRadar->setVisible(!hgss && grass);
        if (!ui->checkBoxSearcherPokeRadar->isVisible())
        {
            ui->checkBoxSearcherPokeRadar->setChecked(false);
            ui->checkBoxSearcherPokeRadarShiny->setChecked(false);
        }

        ui->checkBoxSearcherRadio->setVisible(hgss && grass);
        ui->comboBoxSearcherRadio->setVisible(hgss && grass);
        if (!ui->checkBoxSearcherRadio->isVisible())
        {
            ui->checkBoxSearcherRadio->setChecked(false);
        }

        ui->checkBoxSearcherSwarm->setVisible((!hgss && grass) || (hgss && swarm));
        if (!ui->checkBoxSearcherSwarm->isVisible())
        {
            ui->checkBoxSearcherSwarm->setChecked(false);
        }

        ui->labelSearcherTime->setVisible((!hgss && grass) || hgss);
        ui->comboBoxSearcherTime->setVisible((!hgss && grass) || hgss);

        ui->comboMenuSearcherLead->hideAction(toInt(Lead::MagnetPull), bug);
        ui->comboMenuSearcherLead->hideAction(toInt(Lead::Static), bug);
        ui->comboMenuSearcherLead->hideAction(toInt(Lead::Pressure), bug); // Also handles Hustle and Vital Spirit

        updateEncounterSearcher();

        std::vector<u16> locs;
        std::transform(encounterSearcher.begin(), encounterSearcher.end(), std::back_inserter(locs),
                       [](const EncounterArea4 &area) { return area.getLocation(); });

        auto locations = Translator::getLocations(locs, currentProfile->getVersion());
        std::vector<int> indices(locations.size());
        std::iota(indices.begin(), indices.end(), 0);

        if (!bug)
        {
            std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });
        }

        ui->comboBoxSearcherLocation->clear();
        for (int i : indices)
        {
            ui->comboBoxSearcherLocation->addItem(QString::fromStdString(locations[i]), i);
        }
    }
}

void Wild4::searcherEncounterUpdate()
{
    updateEncounterSearcher();
    searcherLocationIndexChanged(0);
}

void Wild4::searcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounterSearcher[ui->comboBoxSearcherLocation->getCurrentInt()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();
        bool greatMarsh = area.greatMarsh(currentProfile->getVersion());
        bool safari = area.safariZone(currentProfile->getVersion());
        bool trophyGarden = area.trophyGarden(currentProfile->getVersion());

        ui->checkBoxSearcherReplacement->setVisible(greatMarsh || trophyGarden);
        ui->comboBoxSearcherReplacement0->setVisible(greatMarsh || trophyGarden);
        ui->comboBoxSearcherReplacement1->setVisible(trophyGarden);

        ui->labelSearcherPlainsBlock->setVisible(safari);
        ui->spinBoxSearcherPlainsBlock->setVisible(safari);
        ui->labelSearcherForestBlock->setVisible(safari);
        ui->spinBoxSearcherForestBlock->setVisible(safari);
        ui->labelSearcherPeakBlock->setVisible(safari);
        ui->spinBoxSearcherPeakBlock->setVisible(safari);
        ui->labelSearcherWaterBlock->setVisible(safari);
        ui->spinBoxSearcherWaterBlock->setVisible(safari);

        ui->comboBoxSearcherPokemon->clear();
        ui->comboBoxSearcherPokemon->addItem(QString("-"));
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxSearcherPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }

        if (greatMarsh && index != 0)
        {
            // Block signals so we don't cause infinite signal recursion
            ui->comboBoxSearcherReplacement0->blockSignals(true);

            ui->comboBoxSearcherReplacement0->clear();
            for (u16 specie : Encounters4::getGreatMarshPokemon(currentProfile))
            {
                if (specie == 0)
                {
                    break;
                }
                ui->comboBoxSearcherReplacement0->addItem(QString::fromStdString(Translator::getSpecie(specie)), specie);
            }

            ui->comboBoxSearcherReplacement0->blockSignals(false);
        }
        else if (trophyGarden && index != 0)
        {
            // Block signals so we don't cause infinite signal recursion
            ui->comboBoxSearcherReplacement0->blockSignals(true);
            ui->comboBoxSearcherReplacement1->blockSignals(true);

            ui->comboBoxSearcherReplacement0->clear();
            ui->comboBoxSearcherReplacement1->clear();
            for (u16 specie : Encounters4::getTrophyGardenPokemon(currentProfile))
            {
                const auto &name = Translator::getSpecie(specie);
                ui->comboBoxSearcherReplacement0->addItem(QString::fromStdString(name), specie);
                ui->comboBoxSearcherReplacement1->addItem(QString::fromStdString(name), specie);
            }

            ui->comboBoxSearcherReplacement0->blockSignals(false);
            ui->comboBoxSearcherReplacement1->blockSignals(false);
        }

        // Account for safari zone not being its own encounter method
        if (safari)
        {
            ui->filterSearcher->setEncounterSlots(12);
            ui->labelSearcherTime->setVisible(true);
            ui->comboBoxSearcherTime->setVisible(true);
        }
        else if ((currentProfile->getVersion() & Game::HGSS) == Game::None)
        {
            auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();
            bool time = encounter == Encounter::Grass || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;
            ui->labelSearcherTime->setVisible(time);
            ui->comboBoxSearcherTime->setVisible(time);
        }
    }
}

void Wild4::searcherPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterSearcher->resetEncounterSlots();
    }
    else
    {
        u16 num = ui->comboBoxSearcherPokemon->getCurrentUShort();
        auto flags = encounterSearcher[ui->comboBoxSearcherLocation->getCurrentInt()].getSlots(num);
        ui->filterSearcher->toggleEncounterSlots(flags);
    }
}

void Wild4::searcherPokeRadarStateChanged(int state)
{
    ui->checkBoxSearcherPokeRadarShiny->setVisible(state == Qt::Checked);
    if (!ui->checkBoxSearcherPokeRadarShiny->isVisible())
    {
        ui->checkBoxSearcherPokeRadarShiny->setChecked(false);
    }
    ui->comboMenuSearcherLead->hideAction(toInt(Lead::MagnetPull), state == Qt::Checked);
    ui->comboMenuSearcherLead->hideAction(toInt(Lead::Hustle), state == Qt::Checked); // Also handles Pressure and Vital Spirit
    ui->comboMenuSearcherLead->hideAction(toInt(Lead::Static), state == Qt::Checked);
}

void Wild4::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    const auto &state = searcherModel->getItem(index.row());

    auto *time = new SeedToTime4(state.getSeed(), currentProfile->getVersion());
    time->show();
}
