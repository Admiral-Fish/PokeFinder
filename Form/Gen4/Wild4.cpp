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
#include <Form/Controls/CheckList.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen4/Profile/ProfileManager4.hpp>
#include <Form/Gen4/Tools/SeedToTime4.hpp>
#include <Model/Gen4/WildModel4.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QAbstractItemView>
#include <QAction>
#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <algorithm>

enum Gen4Movement : u8
{
    Walking,
    Running,
    Biking,
    WalkingLongGrass,
    RunningLongGrass,
    Surfing
};

enum HGSSSearcherStepOption : u8
{
    StepWhiteFlute = 1 << 0,
    StepPokemonMarch = 1 << 1,
    StepWalking = 1 << 2,
    StepBiking = 1 << 3,
    StepRunning = 1 << 4,
    StepWalkingLongGrass = 1 << 5,
    StepRunningLongGrass = 1 << 6,
    StepPokemonLullaby = 1 << 7
};

static bool hasLongGrass(Game version, Encounter encounter, u8 location)
{
    if (encounter == Encounter::BugCatchingContest)
    {
        return true;
    }

    if ((version & Game::HGSS) != Game::None)
    {
        return location == 23 || location == 24; // National Park
    }

    return location == 156 || location == 163; // Route 210 (South), Route 214
}

static bool isBikeOnlyLocation(Game version, u8 location)
{
    return (version & Game::HGSS) != Game::None && location == 126; // Route 17
}

static bool isBikeRestrictedLocation(Game version, Encounter encounter, u8 location)
{
    if ((version & Game::DPPt) != Game::None)
    {
        return location == 13 || location == 139 || location == 165 || location == 166; // Mt. Coronet Summit, Acuity Lakefront, Route 216/217
    }

    if ((version & Game::HGSS) == Game::None)
    {
        return false;
    }
    return encounter == Encounter::BugCatchingContest || location == 6 || location == 11 || location == 28 || location == 29
        || location == 30 || location == 89;
}

static const QString settingPrefix = QStringLiteral("wild4");

Wild4::Wild4(QWidget *parent) : QWidget(parent), ui(new Ui::Wild4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->profileDisplay->setup(settingPrefix, Game::Gen4);

    generatorModel = new WildGeneratorModel4(ui->tableViewGenerator, Method::MethodJ);
    searcherModel = new WildSearcherModel4(ui->tableViewSearcher);
    proxyModel = new SortFilterProxyModel(ui->tableViewSearcher, searcherModel);
    ui->comboBoxSearcherRadio->removeItem(3);
    ui->comboBoxSearcherRadio->removeItem(3);

    ui->checkBoxGeneratorStepEncounter->setVisible(false);
    ui->checkBoxGeneratorWhiteFlute->setVisible(false);
    ui->checkBoxGeneratorWhiteFlute->setEnabled(false);
    ui->labelGeneratorMovement->setVisible(false);
    ui->labelGeneratorMovement->setEnabled(false);
    ui->comboBoxGeneratorMovement->setVisible(false);
    ui->comboBoxGeneratorMovement->setEnabled(false);
    ui->checkBoxSearcherStepEncounter->setVisible(false);
    ui->checkBoxSearcherWhiteFlute->setVisible(false);
    ui->checkBoxSearcherWhiteFlute->setEnabled(false);

    checkListSearcherStepOptions = new CheckList(this);
    checkListSearcherStepOptions->setVisible(false);
    connect(checkListSearcherStepOptions->view(), &QAbstractItemView::pressed, this,
            [=] { QTimer::singleShot(0, this, &Wild4::updateSearcherStepOptionsText); });

    if (auto *layout = qobject_cast<QGridLayout *>(ui->checkBoxSearcherWhiteFlute->parentWidget()->layout()))
    {
        layout->addWidget(checkListSearcherStepOptions, 7, 2, 1, 2);
    }
    updateSearcherStepOptionsText();

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

    ui->filterGenerator->disableControls(Controls::Height | Controls::Weight);
    ui->filterSearcher->disableControls(Controls::DisableFilter | Controls::Height | Controls::Weight);

    ui->comboMenuGeneratorLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuGeneratorLead->addAction(tr("Compound Eyes"), toInt(Lead::CompoundEyes));
    ui->comboMenuGeneratorLead->addMenu(tr("Cute Charm"),
                                        { { tr("♂ Lead"), toInt(Lead::CuteCharmM) }, { tr("♀ Lead"), toInt(Lead::CuteCharmF) } });
    ui->comboMenuGeneratorLead->addMenu(tr("Encounter Modifier"),
                                        { { tr("Sticky Hold"), toInt(Lead::StickyHold) },
                                          { tr("Suction Cups"), toInt(Lead::SuctionCups) } });
    ui->comboMenuGeneratorLead->addMenu(tr("Level Modifier"),
                                        { { tr("Hustle"), toInt(Lead::Hustle) },
                                          { tr("Pressure"), toInt(Lead::Pressure) },
                                          { tr("Vital Spirit"), toInt(Lead::VitalSpirit) } });
    ui->comboMenuGeneratorLead->addMenu(tr("Slot Modifier"),
                                        { { tr("Magnet Pull"), toInt(Lead::MagnetPull) }, { tr("Static"), toInt(Lead::Static) } });
    ui->comboMenuGeneratorLead->addMenu(tr("Step Modifier"),
                                        { { tr("Arena Trap"), toInt(Lead::ArenaTrap) },
                                          { tr("Illuminate"), toInt(Lead::Illuminate) },
                                          { tr("No Guard"), toInt(Lead::NoGuard) } });
    ui->comboMenuGeneratorLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboMenuSearcherLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuSearcherLead->addAction(tr("Compound Eyes"), toInt(Lead::CompoundEyes));
    ui->comboMenuSearcherLead->addMenu(tr("Cute Charm"),
                                       { { tr("♂ Lead"), toInt(Lead::CuteCharmM) }, { tr("♀ Lead"), toInt(Lead::CuteCharmF) } });
    ui->comboMenuSearcherLead->addMenu(tr("Encounter Modifier"),
                                       { { tr("Sticky Hold"), toInt(Lead::StickyHold) },
                                         { tr("Suction Cups"), toInt(Lead::SuctionCups) } });
    ui->comboMenuSearcherLead->addMenu(tr("Level Modifier"),
                                       { { tr("Hustle"), toInt(Lead::Hustle) },
                                         { tr("Pressure"), toInt(Lead::Pressure) },
                                         { tr("Vital Spirit"), toInt(Lead::VitalSpirit) } });
    ui->comboMenuSearcherLead->addMenu(tr("Slot Modifier"),
                                       { { tr("Magnet Pull"), toInt(Lead::MagnetPull) }, { tr("Static"), toInt(Lead::Static) } });
    ui->comboMenuSearcherLead->addMenu(tr("Step Modifier"),
                                       { { tr("Arena Trap"), toInt(Lead::ArenaTrap) },
                                         { tr("Illuminate"), toInt(Lead::Illuminate) },
                                         { tr("No Guard"), toInt(Lead::NoGuard) } });
    ui->comboMenuSearcherLead->addAction(tr("Synchronize"), toInt(Lead::Synchronize));

    ui->comboBoxGeneratorEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::HoneyTree), toInt(Encounter::RockSmash),
                                            toInt(Encounter::BugCatchingContest), toInt(Encounter::Headbutt), toInt(Encounter::HeadbuttAlt),
                                            toInt(Encounter::HeadbuttSpecial), toInt(Encounter::Surfing), toInt(Encounter::OldRod),
                                            toInt(Encounter::GoodRod), toInt(Encounter::SuperRod) });

    ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::HoneyTree), toInt(Encounter::RockSmash),
                                           toInt(Encounter::BugCatchingContest), toInt(Encounter::Headbutt), toInt(Encounter::HeadbuttAlt),
                                           toInt(Encounter::HeadbuttSpecial), toInt(Encounter::Surfing), toInt(Encounter::OldRod),
                                           toInt(Encounter::GoodRod), toInt(Encounter::SuperRod) });

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

    connect(ui->profileDisplay, &ProfileDisplay4::profileChanged, this, &Wild4::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay4::profilesChanged, this, &Wild4::profilesChanged);
    connect(ui->tabRNGSelector, &TabWidget::transferFilters, this, &Wild4::transferFilters);
    connect(ui->tabRNGSelector, &TabWidget::transferSettings, this, &Wild4::transferSettings);
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
    connect(ui->checkBoxGeneratorFeebasTile, &QCheckBox::checkStateChanged, this, &Wild4::generatorFeebasTileStateChanged);
    connect(ui->checkBoxSearcherFeebasTile, &QCheckBox::checkStateChanged, this, &Wild4::searcherFeebasTileStateChanged);
    connect(ui->checkBoxGeneratorPokeRadar, &QCheckBox::checkStateChanged, this, &Wild4::generatorPokeRadarStateChanged);
    connect(ui->checkBoxSearcherPokeRadar, &QCheckBox::checkStateChanged, this, &Wild4::searcherPokeRadarStateChanged);
    connect(ui->checkBoxGeneratorStepEncounter, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        generatorModel->setShowStepEncounter(state == Qt::Checked);
        ui->checkBoxGeneratorWhiteFlute->setEnabled(state == Qt::Checked);
        ui->labelGeneratorMovement->setEnabled(state == Qt::Checked);
        ui->comboBoxGeneratorMovement->setEnabled(state == Qt::Checked);
        if (state != Qt::Checked)
        {
            ui->checkBoxGeneratorWhiteFlute->setChecked(false);
        }
        if (generatorModel->rowCount() > 0)
        {
            generate();
        }
    });
    connect(ui->checkBoxSearcherStepEncounter, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) {
        searcherModel->setShowStepEncounter(state == Qt::Checked);
        ui->checkBoxSearcherWhiteFlute->setEnabled(state == Qt::Checked && ui->checkBoxSearcherWhiteFlute->isVisible());
        checkListSearcherStepOptions->setEnabled(state == Qt::Checked && checkListSearcherStepOptions->isVisible());
        if (state != Qt::Checked)
        {
            ui->checkBoxSearcherWhiteFlute->setChecked(false);
        }
        updateSearcherStepOptions();
    });
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

    updateProfiles();
    generatorEncounterIndexChanged(0);
    searcherEncounterIndexChanged(0);

    QSettings setting;
    setting.beginGroup(settingPrefix);
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
    setting.beginGroup(settingPrefix);
    setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("minAdvance", ui->textBoxSearcherMinAdvance->text());
    setting.setValue("maxAdvance", ui->textBoxSearcherMaxAdvance->text());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Wild4::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void Wild4::updateEncounterGenerator()
{
    auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();

    EncounterSettings4 settings = { };
    if ((currentProfile->getVersion() & Game::DPPt) != Game::None)
    {
        settings.dppt.dual = ui->checkBoxGeneratorDualSlot->isChecked() ? ui->comboBoxGeneratorDualSlot->getEnum<Game>() : Game::None;
        if (ui->checkBoxGeneratorReplacement->isChecked())
        {
            settings.dppt.replacement[0] = ui->comboBoxGeneratorReplacement0->getCurrentUShort();
            settings.dppt.replacement[1]
                = ui->comboBoxGeneratorReplacement1->count() > 0 ? ui->comboBoxGeneratorReplacement1->getCurrentUShort() : 0;
        }
        settings.dppt.feebasTile = ui->checkBoxGeneratorFeebasTile->isChecked();
        settings.dppt.radar = ui->checkBoxGeneratorPokeRadar->isChecked();
    }
    else
    {
        settings.hgss.radio
            = ui->checkBoxGeneratorRadio->isChecked() && ui->comboBoxGeneratorRadio->currentIndex() < 3
            ? ui->comboBoxGeneratorRadio->currentIndex() + 1
            : 0;
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

    EncounterSettings4 settings = { };
    if ((currentProfile->getVersion() & Game::DPPt) != Game::None)
    {
        settings.dppt.dual = ui->checkBoxSearcherDualSlot->isChecked() ? ui->comboBoxSearcherDualSlot->getEnum<Game>() : Game::None;
        if (ui->checkBoxSearcherReplacement->isChecked())
        {
            settings.dppt.replacement[0] = ui->comboBoxSearcherReplacement0->getCurrentUShort();
            settings.dppt.replacement[1]
                = ui->comboBoxSearcherReplacement1->count() > 0 ? ui->comboBoxSearcherReplacement1->getCurrentUShort() : 0;
        }
        settings.dppt.feebasTile = ui->checkBoxSearcherFeebasTile->isChecked();
        settings.dppt.radar = ui->checkBoxSearcherPokeRadar->isChecked();
    }
    else
    {
        settings.hgss.radio
            = ui->checkBoxSearcherRadio->isChecked() && ui->comboBoxSearcherRadio->currentIndex() < 3
            ? ui->comboBoxSearcherRadio->currentIndex() + 1
            : 0;
        settings.hgss.blocks
            = { 0, static_cast<u8>(ui->spinBoxSearcherPlainsBlock->value()), static_cast<u8>(ui->spinBoxSearcherForestBlock->value()),
                static_cast<u8>(ui->spinBoxSearcherPeakBlock->value()), static_cast<u8>(ui->spinBoxSearcherWaterBlock->value()) };
    }
    settings.time = ui->comboBoxSearcherTime->currentIndex();
    settings.swarm = ui->checkBoxSearcherSwarm->isChecked();

    encounterSearcher = Encounters4::getEncounters(encounter, settings, currentProfile);
}

void Wild4::updateGeneratorMovementOptions()
{
    if (encounterGenerator.empty() || ui->comboBoxGeneratorLocation->currentIndex() < 0)
    {
        return;
    }

    u8 current = ui->comboBoxGeneratorMovement->count() == 0 ? Walking : ui->comboBoxGeneratorMovement->getCurrentUChar();
    auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();
    u8 location = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()].getLocation();
    bool hgss = (currentProfile->getVersion() & Game::HGSS) != Game::None;

    ui->comboBoxGeneratorMovement->clear();
    if (encounter == Encounter::Surfing)
    {
        ui->comboBoxGeneratorMovement->addItem(tr("Surfing"), Surfing);
    }
    else if (isBikeOnlyLocation(currentProfile->getVersion(), location))
    {
        ui->comboBoxGeneratorMovement->addItem(tr("Biking"), Biking);
    }
    else
    {
        ui->comboBoxGeneratorMovement->addItem(hgss ? tr("Walking") : tr("Walking / Running"), Walking);
        if (hgss)
        {
            ui->comboBoxGeneratorMovement->addItem(tr("Running"), Running);
        }
        if (!isBikeRestrictedLocation(currentProfile->getVersion(), encounter, location))
        {
            ui->comboBoxGeneratorMovement->addItem(tr("Biking"), Biking);
        }
        if (hasLongGrass(currentProfile->getVersion(), encounter, location))
        {
            ui->comboBoxGeneratorMovement->addItem(hgss ? tr("Walking in Long Grass") : tr("Walking / Running in Long Grass"),
                                                   WalkingLongGrass);
            if (hgss)
            {
                ui->comboBoxGeneratorMovement->addItem(tr("Running in Long Grass"), RunningLongGrass);
            }
        }
    }

    if (!hgss && current == Running)
    {
        current = Walking;
    }
    else if (!hgss && current == RunningLongGrass)
    {
        current = WalkingLongGrass;
    }
    int index = ui->comboBoxGeneratorMovement->findData(current);
    ui->comboBoxGeneratorMovement->setCurrentIndex(index == -1 ? 0 : index);
}

void Wild4::updateSearcherStepOptions()
{
    if (encounterSearcher.empty() || ui->comboBoxSearcherLocation->currentIndex() < 0)
    {
        return;
    }

    std::vector<u16> previous;
    std::vector<u16> previousOptions;
    for (int i = 0; i < checkListSearcherStepOptions->count(); i++)
    {
        previousOptions.emplace_back(checkListSearcherStepOptions->itemData(i).toUInt());
    }
    previous = checkListSearcherStepOptions->getExplicitCheckedData();
    bool hasPrevious = checkListSearcherStepOptions->count() != 0;
    auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();
    u8 location = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()].getLocation();
    bool hgss = (currentProfile->getVersion() & Game::HGSS) != Game::None;
    bool surf = encounter == Encounter::Surfing;
    bool bug = encounter == Encounter::BugCatchingContest;
    bool bikeOnly = isBikeOnlyLocation(currentProfile->getVersion(), location);
    bool bikeRestricted = isBikeRestrictedLocation(currentProfile->getVersion(), encounter, location);
    bool longGrass = hasLongGrass(currentProfile->getVersion(), encounter, location);

    checkListSearcherStepOptions->clear();
    std::vector<bool> checks;
    auto addOption = [&](const QString &text, u8 data, bool defaultChecked) {
        checkListSearcherStepOptions->addItem(text, data);
        bool existed = std::ranges::contains(previousOptions, data);
        bool checked = hasPrevious && existed ? std::ranges::contains(previous, data) : defaultChecked;
        checks.emplace_back(checked);
    };

    if (!bug)
    {
        addOption(tr("White Flute"), StepWhiteFlute, false);
    }
    if (hgss)
    {
        addOption(tr("Pokemon March"), StepPokemonMarch, false);
        addOption(tr("Pokemon Lullaby"), StepPokemonLullaby, false);
    }
    if (!surf && (bikeOnly || !bikeRestricted))
    {
        addOption(tr("Biking"), StepBiking, true);
    }
    if (!surf && !bikeOnly && longGrass)
    {
        addOption(hgss ? tr("Walking in Long Grass") : tr("Walking / Running in Long Grass"), StepWalkingLongGrass, true);
    }
    if (hgss && !surf && !bikeOnly && longGrass)
    {
        addOption(tr("Running in Long Grass"), StepRunningLongGrass, true);
    }

    checkListSearcherStepOptions->setChecks(checks);

    updateSearcherStepOptionsText();
}

void Wild4::updateSearcherStepOptionsText()
{
    QStringList enabled;
    bool all = true;
    std::vector<bool> checks = checkListSearcherStepOptions->getExplicitChecked();
    for (int i = 0; i < checkListSearcherStepOptions->count() && i < checks.size(); i++)
    {
        if (checks[i])
        {
            enabled << checkListSearcherStepOptions->itemText(i);
        }
        else
        {
            all = false;
        }
    }

    if (enabled.isEmpty())
    {
        checkListSearcherStepOptions->lineEdit()->setText(tr("None"));
    }
    else
    {
        QString text = all ? tr("Any") : enabled.join(", ");
        QFontMetrics metric(checkListSearcherStepOptions->lineEdit()->font());
        checkListSearcherStepOptions->lineEdit()->setText(metric.elidedText(text, Qt::ElideRight, checkListSearcherStepOptions->lineEdit()->width()));
    }
}

u8 Wild4::getSearcherStepOptions() const
{
    u8 options = 0;
    for (u16 data : checkListSearcherStepOptions->getExplicitCheckedData())
    {
        options |= static_cast<u8>(data);
    }

    auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();
    bool hgss = (currentProfile->getVersion() & Game::HGSS) != Game::None;
    bool surf = encounter == Encounter::Surfing;
    u8 location = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()].getLocation();
    bool bikeOnly = isBikeOnlyLocation(currentProfile->getVersion(), location);
    if (surf)
    {
        options |= StepWalking;
        if (!hgss && ui->checkBoxSearcherWhiteFlute->isChecked())
        {
            options |= StepWhiteFlute;
        }
    }
    else if (!bikeOnly)
    {
        options |= StepWalking;
        if (hgss)
        {
            options |= StepRunning;
        }
    }

    return options;
}

void Wild4::generate()
{
    if (!ui->filterGenerator->isValid(ui->spinBoxGeneratorLevelMin->value(), ui->spinBoxGeneratorLevelMax->value()))
    {
        return;
    }

    Method method;
    u8 fixedSlot = 0;
    auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();
    if ((currentProfile->getVersion() & Game::DPPt) != Game::None)
    {
        if (ui->checkBoxGeneratorPokeRadar->isChecked())
        {
            method = Method::PokeRadar;
            std::array<bool, 12> encounters = ui->filterGenerator->getEncounterSlots();
            if (std::ranges::count(encounters, true) != 1)
            {
                QMessageBox msg(QMessageBox::Warning, tr("Too many slots selected"),
                                tr("Please select a single encounter slot for Poke Radar"));
                msg.exec();
                return;
            }
            else
            {
                fixedSlot = std::ranges::find(encounters, true) - encounters.begin();
            }
        }
        else if (encounter == Encounter::HoneyTree)
        {
            method = Method::HoneyTree;
            std::array<bool, 12> encounters = ui->filterGenerator->getEncounterSlots();
            if (std::ranges::count(encounters, true) != 1)
            {
                QMessageBox msg(QMessageBox::Warning, tr("Too many slots selected"),
                                tr("Please select a single encounter slot for Honey Tree"));
                msg.exec();
                return;
            }
            else
            {
                fixedSlot = std::ranges::find(encounters, true) - encounters.begin();
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
    u32 offset = ui->textBoxGeneratorOffset->getUInt();
    auto lead = ui->comboMenuGeneratorLead->getEnum<Lead>();
    bool feebasTile = ui->checkBoxGeneratorFeebasTile->isChecked();
    bool chained = ui->checkBoxGeneratorPokeRadarShiny->isChecked();
    bool unownRadio = ui->checkBoxGeneratorRadio->isChecked() && ui->comboBoxGeneratorRadio->currentIndex() == 2;
    u8 happiness = ui->comboBoxGeneratorHappiness->getCurrentUChar();
    bool searchStepEncounter = ui->checkBoxGeneratorStepEncounter->isChecked();
    bool whiteFlute = ui->checkBoxGeneratorWhiteFlute->isChecked();
    u8 movement = ui->comboBoxGeneratorMovement->getCurrentUChar();
    bool fastMovement = movement == Biking || movement == WalkingLongGrass || movement == RunningLongGrass;
    u8 radio = ui->checkBoxGeneratorRadio->isChecked() ? ui->comboBoxGeneratorRadio->currentIndex() + 1 : 0;

    auto filter = ui->filterGenerator->getFilter<WildStateFilter, true>();
    WildGenerator4 generator(initialAdvances, maxAdvances, offset, method, lead, feebasTile, chained, unownRadio, happiness,
                             searchStepEncounter, whiteFlute, fastMovement, movement, radio,
                             encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()], *currentProfile, filter);

    auto states = generator.generate(seed, fixedSlot);
    generatorModel->addItems(states);
}

void Wild4::generatorEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();
        u16 currentLocation = ui->comboBoxGeneratorLocation->getCurrentUShort();

        bool bug = encounter == Encounter::BugCatchingContest;
        bool fish = encounter == Encounter::OldRod || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;
        bool grass = encounter == Encounter::Grass;
        bool hgss = (currentProfile->getVersion() & Game::HGSS) != Game::None;
        generatorModel->setMethod(hgss ? Method::MethodK : Method::MethodJ);
        bool stepEncounter = encounter == Encounter::Grass || encounter == Encounter::Surfing || encounter == Encounter::BugCatchingContest;
        bool swarm = encounter == Encounter::Grass || encounter == Encounter::Surfing || encounter == Encounter::OldRod
            || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;
        bool honey = encounter == Encounter::HoneyTree;

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

        ui->checkBoxGeneratorRadio->setVisible(hgss && stepEncounter);
        ui->comboBoxGeneratorRadio->setVisible(hgss && stepEncounter);
        if (!ui->checkBoxGeneratorRadio->isVisible())
        {
            ui->checkBoxGeneratorRadio->setChecked(false);
        }

        ui->checkBoxGeneratorSwarm->setVisible((!hgss && grass) || (hgss && swarm));
        if (!ui->checkBoxGeneratorSwarm->isVisible())
        {
            ui->checkBoxGeneratorSwarm->setChecked(false);
        }

        ui->checkBoxGeneratorStepEncounter->setVisible(stepEncounter);
        ui->checkBoxGeneratorWhiteFlute->setVisible(stepEncounter);
        ui->checkBoxGeneratorWhiteFlute->setEnabled(ui->checkBoxGeneratorStepEncounter->isChecked());
        ui->labelGeneratorMovement->setVisible(stepEncounter && encounter != Encounter::Surfing);
        ui->comboBoxGeneratorMovement->setVisible(stepEncounter && encounter != Encounter::Surfing);
        ui->labelGeneratorMovement->setEnabled(ui->checkBoxGeneratorStepEncounter->isChecked() && ui->labelGeneratorMovement->isVisible());
        ui->comboBoxGeneratorMovement->setEnabled(ui->checkBoxGeneratorStepEncounter->isChecked() && ui->comboBoxGeneratorMovement->isVisible());
        if (!ui->checkBoxGeneratorStepEncounter->isVisible())
        {
            ui->checkBoxGeneratorStepEncounter->setChecked(false);
            ui->checkBoxGeneratorWhiteFlute->setChecked(false);
            ui->checkBoxGeneratorWhiteFlute->setEnabled(false);
            ui->labelGeneratorMovement->setEnabled(false);
            ui->comboBoxGeneratorMovement->setEnabled(false);
        }

        ui->labelGeneratorTime->setVisible((!hgss && grass) || hgss);
        ui->comboBoxGeneratorTime->setVisible((!hgss && grass) || hgss);

        ui->comboMenuGeneratorLead->hideAction(toInt(Lead::MagnetPull), bug || honey);
        ui->comboMenuGeneratorLead->hideAction(toInt(Lead::Static), bug || honey);
        ui->comboMenuGeneratorLead->hideAction(toInt(Lead::Pressure), bug); // Also handles Hustle and Vital Spirit

        updateEncounterGenerator();

        std::vector<u16> locs;
        std::ranges::transform(encounterGenerator, std::back_inserter(locs), [](const EncounterArea4 &area) { return area.getLocation(); });

        ui->comboBoxGeneratorLocation->clear();
        ui->comboBoxGeneratorLocation->addItems(Translator::getLocations(locs, currentProfile->getVersion()), locs);
        ui->comboBoxGeneratorLocation->setCurrentIndexByData(currentLocation);
        updateGeneratorMovementOptions();
    }
}

void Wild4::generatorEncounterUpdate()
{
    updateEncounterGenerator();
    generatorLocationIndexChanged(0);
}

void Wild4::generatorFeebasTileStateChanged(Qt::CheckState state)
{
    updateEncounterGenerator();
    generatorLocationIndexChanged(0);
}

void Wild4::generatorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();
        bool feebas = area.feebasLocation(currentProfile->getVersion());
        bool greatMarsh = area.greatMarsh(currentProfile->getVersion());
        bool safari = area.safariZone(currentProfile->getVersion());
        bool trophyGarden = area.trophyGarden(currentProfile->getVersion());
        auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();

        updateGeneratorMovementOptions();

        ui->filterGenerator->setEncounterSlots(area.getCount());

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
            ui->labelGeneratorTime->setVisible(true);
            ui->comboBoxGeneratorTime->setVisible(true);
        }
        else
        {
            if ((currentProfile->getVersion() & Game::HGSS) != Game::None)
            {
                bool time = encounter == Encounter::Grass || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;
                ui->labelGeneratorTime->setVisible(time);
                ui->comboBoxGeneratorTime->setVisible(time);
            }
        }

        if (feebas && (encounter == Encounter::OldRod || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod))
        {
            ui->checkBoxGeneratorFeebasTile->setVisible(true);
        }
        else
        {
            ui->checkBoxGeneratorFeebasTile->setVisible(false);
            ui->checkBoxGeneratorFeebasTile->setChecked(false);
        }
    }
}

void Wild4::generatorPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterGenerator->resetEncounterSlots();
        ui->spinBoxGeneratorLevelMin->setValue(0);
        ui->spinBoxGeneratorLevelMax->setValue(0);
        ui->filterGenerator->setLevelRange(1, 100);
    }
    else
    {
        u16 num = ui->comboBoxGeneratorPokemon->getCurrentUShort();
        auto flags = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()].getSlots(num);
        ui->filterGenerator->toggleEncounterSlots(flags);

        auto range = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()].getLevelRange(num);
        ui->spinBoxGeneratorLevelMin->setValue(range.first);
        ui->spinBoxGeneratorLevelMax->setValue(range.second);
        ui->filterGenerator->setLevelRange(range.first, range.second);
    }
}

void Wild4::generatorPokeRadarStateChanged(Qt::CheckState state)
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

void Wild4::profileChanged(const Profile4 &profile)
{
    currentProfile = &profile;

    bool hgss = (currentProfile->getVersion() & Game::HGSS) != Game::None;

    ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::HoneyTree)), hgss);
    ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::RockSmash)), !hgss);
    ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::BugCatchingContest)), !hgss);
    ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::Headbutt)), !hgss);
    ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::HeadbuttAlt)), !hgss);
    ui->comboBoxGeneratorEncounter->setItemHidden(ui->comboBoxGeneratorEncounter->findData(toInt(Encounter::HeadbuttSpecial)), !hgss);
    ui->comboMenuGeneratorLead->hideAction(toInt(Lead::ArenaTrap), false); // Also handles Illuminate and No Guard
    ui->comboMenuGeneratorLead->hideAction(toInt(Lead::StickyHold), !hgss); // Also handles Suction Cups

    ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::HoneyTree)), hgss);
    ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::RockSmash)), !hgss);
    ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::BugCatchingContest)), !hgss);
    ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::Headbutt)), !hgss);
    ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::HeadbuttAlt)), !hgss);
    ui->comboBoxSearcherEncounter->setItemHidden(ui->comboBoxSearcherEncounter->findData(toInt(Encounter::HeadbuttSpecial)), !hgss);
    ui->comboMenuSearcherLead->hideAction(toInt(Lead::ArenaTrap), false); // Also handles Illuminate and No Guard
    ui->comboMenuSearcherLead->hideAction(toInt(Lead::StickyHold), !hgss); // Also handles Suction Cups

    generatorEncounterIndexChanged(0);
    searcherEncounterIndexChanged(0);
}

void Wild4::search()
{
    if (!ui->filterSearcher->isValid(ui->spinBoxSearcherLevelMin->value(), ui->spinBoxSearcherLevelMax->value()))
    {
        return;
    }

    Method method;
    u8 fixedSlot = 0;
    auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();
    if ((currentProfile->getVersion() & Game::DPPt) != Game::None)
    {
        if (ui->checkBoxSearcherPokeRadar->isChecked())
        {
            method = Method::PokeRadar;
            std::array<bool, 12> encounters = ui->filterSearcher->getEncounterSlots();
            if (std::ranges::count(encounters, true) != 1)
            {
                QMessageBox msg(QMessageBox::Warning, tr("Too many slots selected"),
                                tr("Please select a single encounter slot for Poke Radar"));
                msg.exec();
                return;
            }
            else
            {
                fixedSlot = std::ranges::find(encounters, true) - encounters.begin();
            }
        }
        else if (encounter == Encounter::HoneyTree)
        {
            method = Method::HoneyTree;
            std::array<bool, 12> encounters = ui->filterSearcher->getEncounterSlots();
            if (std::ranges::count(encounters, true) != 1)
            {
                QMessageBox msg(QMessageBox::Warning, tr("Too many slots selected"),
                                tr("Please select a single encounter slot for Honey Tree"));
                msg.exec();
                return;
            }
            else
            {
                fixedSlot = std::ranges::find(encounters, true) - encounters.begin();
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

    auto &area = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()];
    if (encounter == Encounter::BugCatchingContest || area.safariZone(currentProfile->getVersion()))
    {
        bool flag = std::ranges::any_of(min, [](u8 iv) { return iv == 31; });
        if (!flag)
        {
            QMessageBox msg(QMessageBox::Warning, tr("Missing Flawless IV"), tr("This search needs at least one IV at 31"));
            msg.exec();
            return;
        }
    }

    searcherModel->clearModel();
    searcherModel->setMethod(method);
    searcherModel->setShowStepMovement(encounter != Encounter::Surfing);

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 minAdvance = ui->textBoxSearcherMinAdvance->getUInt();
    u32 maxAdvance = ui->textBoxSearcherMaxAdvance->getUInt();
    u32 minDelay = ui->textBoxSearcherMinDelay->getUInt();
    u32 maxDelay = ui->textBoxSearcherMaxDelay->getUInt();
    auto lead = ui->comboMenuSearcherLead->getEnum<Lead>();
    bool feebas = ui->checkBoxSearcherFeebasTile->isChecked();
    bool shiny = ui->checkBoxSearcherPokeRadarShiny->isChecked();
    bool unownRadio = ui->checkBoxSearcherRadio->isChecked() && ui->comboBoxSearcherRadio->currentIndex() == 2;
    u8 happiness = ui->comboBoxSearcherHappiness->getCurrentUChar();
    bool searchStepEncounter = ui->checkBoxSearcherStepEncounter->isChecked();
    u8 stepOptions = searchStepEncounter ? getSearcherStepOptions() : 0;

    auto filter = ui->filterSearcher->getFilter<WildStateFilter, true>();
    auto *searcher = new WildSearcher4(minAdvance, maxAdvance, minDelay, maxDelay, method, lead, feebas, shiny, unownRadio, happiness,
                                       searchStepEncounter, stepOptions, area, *currentProfile, filter);

    int maxProgress = 1;
    for (u8 i = 0; i < 6; i++)
    {
        maxProgress *= max[i] - min[i] + 1;
    }
    searcher->setMaxProgress(maxProgress);

    auto *thread = QThread::create([=] { searcher->startSearch(min, max, fixedSlot); });
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
        u16 currentLocation = ui->comboBoxSearcherLocation->getCurrentUShort();

        bool bug = encounter == Encounter::BugCatchingContest;
        bool fish = encounter == Encounter::OldRod || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;
        bool grass = encounter == Encounter::Grass;
        bool hgss = (currentProfile->getVersion() & Game::HGSS) != Game::None;
        searcherModel->setMethod(hgss ? Method::MethodK : Method::MethodJ);
        searcherModel->setShowStepMovement(encounter != Encounter::Surfing);
        bool stepEncounter = encounter == Encounter::Grass || encounter == Encounter::Surfing || encounter == Encounter::BugCatchingContest;
        bool swarm = encounter == Encounter::Grass || encounter == Encounter::Surfing || encounter == Encounter::OldRod
            || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;
        bool honey = encounter == Encounter::HoneyTree;

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

        ui->checkBoxSearcherRadio->setVisible(hgss && stepEncounter);
        ui->comboBoxSearcherRadio->setVisible(hgss && stepEncounter);
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

        bool dpptSurf = !hgss && encounter == Encounter::Surfing;
        ui->checkBoxSearcherStepEncounter->setVisible(stepEncounter);
        ui->checkBoxSearcherWhiteFlute->setVisible(stepEncounter && dpptSurf);
        checkListSearcherStepOptions->setVisible(stepEncounter && !dpptSurf);
        ui->checkBoxSearcherWhiteFlute->setEnabled(ui->checkBoxSearcherStepEncounter->isChecked() && ui->checkBoxSearcherWhiteFlute->isVisible());
        checkListSearcherStepOptions->setEnabled(ui->checkBoxSearcherStepEncounter->isChecked() && checkListSearcherStepOptions->isVisible());
        if (!ui->checkBoxSearcherWhiteFlute->isVisible())
        {
            ui->checkBoxSearcherWhiteFlute->setChecked(false);
        }
        if (!ui->checkBoxSearcherStepEncounter->isVisible())
        {
            ui->checkBoxSearcherStepEncounter->setChecked(false);
            ui->checkBoxSearcherWhiteFlute->setChecked(false);
        }

        ui->comboMenuSearcherLead->hideAction(toInt(Lead::MagnetPull), bug || honey);
        ui->comboMenuSearcherLead->hideAction(toInt(Lead::Static), bug || honey);
        ui->comboMenuSearcherLead->hideAction(toInt(Lead::Pressure), bug); // Also handles Hustle and Vital Spirit

        updateEncounterSearcher();

        std::vector<u16> locs;
        std::ranges::transform(encounterSearcher, std::back_inserter(locs), [](const EncounterArea4 &area) { return area.getLocation(); });

        ui->comboBoxSearcherLocation->clear();
        ui->comboBoxSearcherLocation->addItems(Translator::getLocations(locs, currentProfile->getVersion()), locs);
        ui->comboBoxSearcherLocation->setCurrentIndexByData(currentLocation);
        updateSearcherStepOptions();
    }
}

void Wild4::searcherEncounterUpdate()
{
    updateEncounterSearcher();
    searcherLocationIndexChanged(0);
}

void Wild4::searcherFeebasTileStateChanged(Qt::CheckState state)
{
    updateEncounterSearcher();
    searcherLocationIndexChanged(0);
}

void Wild4::searcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();
        bool feebas = area.feebasLocation(currentProfile->getVersion());
        bool greatMarsh = area.greatMarsh(currentProfile->getVersion());
        bool safari = area.safariZone(currentProfile->getVersion());
        bool trophyGarden = area.trophyGarden(currentProfile->getVersion());
        auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();

        updateSearcherStepOptions();

        ui->filterSearcher->setEncounterSlots(area.getCount());

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
            ui->labelSearcherTime->setVisible(true);
            ui->comboBoxSearcherTime->setVisible(true);
        }
        else if ((currentProfile->getVersion() & Game::HGSS) == Game::None)
        {
            bool time = encounter == Encounter::Grass || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod;
            ui->labelSearcherTime->setVisible(time);
            ui->comboBoxSearcherTime->setVisible(time);
        }

        if (feebas && (encounter == Encounter::OldRod || encounter == Encounter::GoodRod || encounter == Encounter::SuperRod))
        {
            ui->checkBoxSearcherFeebasTile->setVisible(true);
        }
        else
        {
            ui->checkBoxSearcherFeebasTile->setVisible(false);
            ui->checkBoxSearcherFeebasTile->setChecked(false);
        }
    }
}

void Wild4::searcherPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterSearcher->resetEncounterSlots();
        ui->spinBoxSearcherLevelMin->setValue(0);
        ui->spinBoxSearcherLevelMax->setValue(0);
        ui->filterSearcher->setLevelRange(1, 100);
    }
    else
    {
        u16 num = ui->comboBoxSearcherPokemon->getCurrentUShort();
        auto flags = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()].getSlots(num);
        ui->filterSearcher->toggleEncounterSlots(flags);

        auto range = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()].getLevelRange(num);
        ui->spinBoxSearcherLevelMin->setValue(range.first);
        ui->spinBoxSearcherLevelMax->setValue(range.second);
        ui->filterSearcher->setLevelRange(range.first, range.second);
    }
}

void Wild4::searcherPokeRadarStateChanged(Qt::CheckState state)
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
    QModelIndex index = proxyModel->mapToSource(ui->tableViewSearcher->currentIndex());
    const auto &state = searcherModel->getItem(index.row());

    auto *time = new SeedToTime4(state.getSeed(), currentProfile->getVersion());
    time->show();
}

void Wild4::transferFilters(int index)
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

void Wild4::transferSettings(int index)
{
    if (index == 0)
    {
        ui->comboBoxSearcherEncounter->setCurrentIndex(ui->comboBoxGeneratorEncounter->currentIndex());
        ui->comboBoxSearcherLocation->setCurrentIndex(ui->comboBoxGeneratorLocation->currentIndex());
        ui->comboBoxSearcherPokemon->setCurrentIndex(ui->comboBoxGeneratorPokemon->currentIndex());
        ui->comboBoxSearcherTime->setCurrentIndex(ui->comboBoxGeneratorTime->currentIndex());
        ui->comboBoxSearcherHappiness->setCurrentIndex(ui->comboBoxGeneratorHappiness->currentIndex());

        ui->checkBoxSearcherDualSlot->setCheckState(ui->checkBoxGeneratorDualSlot->checkState());
        ui->comboBoxSearcherDualSlot->setCurrentIndex(ui->comboBoxGeneratorDualSlot->currentIndex());

        ui->checkBoxSearcherFeebasTile->setChecked(ui->checkBoxGeneratorFeebasTile->isChecked());

        ui->checkBoxSearcherPokeRadar->setCheckState(ui->checkBoxGeneratorPokeRadar->checkState());

        ui->checkBoxSearcherRadio->setCheckState(ui->checkBoxGeneratorRadio->checkState());
        ui->comboBoxSearcherRadio->setCurrentIndex(ui->comboBoxGeneratorRadio->currentIndex());

        ui->checkBoxSearcherSwarm->setCheckState(ui->checkBoxGeneratorSwarm->checkState());
        ui->checkBoxSearcherStepEncounter->setCheckState(ui->checkBoxGeneratorStepEncounter->checkState());
        ui->checkBoxSearcherWhiteFlute->setCheckState(ui->checkBoxGeneratorWhiteFlute->checkState());
        ui->checkBoxSearcherReplacement->setCheckState(ui->checkBoxGeneratorReplacement->checkState());
        ui->comboBoxSearcherReplacement0->setCurrentIndex(ui->comboBoxGeneratorReplacement0->currentIndex());
        ui->comboBoxSearcherReplacement1->setCurrentIndex(ui->comboBoxGeneratorReplacement1->currentIndex());

        ui->spinBoxSearcherPlainsBlock->setValue(ui->spinBoxGeneratorPlainsBlock->value());
        ui->spinBoxSearcherForestBlock->setValue(ui->spinBoxGeneratorForestBlock->value());
        ui->spinBoxSearcherPeakBlock->setValue(ui->spinBoxGeneratorPeakBlock->value());
        ui->spinBoxSearcherWaterBlock->setValue(ui->spinBoxGeneratorWaterBlock->value());
    }
    else
    {
        ui->comboBoxGeneratorEncounter->setCurrentIndex(ui->comboBoxSearcherEncounter->currentIndex());
        ui->comboBoxGeneratorLocation->setCurrentIndex(ui->comboBoxSearcherLocation->currentIndex());
        ui->comboBoxGeneratorPokemon->setCurrentIndex(ui->comboBoxSearcherPokemon->currentIndex());
        ui->comboBoxGeneratorTime->setCurrentIndex(ui->comboBoxSearcherTime->currentIndex());
        ui->comboBoxGeneratorHappiness->setCurrentIndex(ui->comboBoxSearcherHappiness->currentIndex());

        ui->checkBoxGeneratorDualSlot->setCheckState(ui->checkBoxSearcherDualSlot->checkState());
        ui->comboBoxGeneratorDualSlot->setCurrentIndex(ui->comboBoxSearcherDualSlot->currentIndex());

        ui->checkBoxGeneratorFeebasTile->setChecked(ui->checkBoxSearcherFeebasTile->isChecked());

        ui->checkBoxGeneratorPokeRadar->setCheckState(ui->checkBoxSearcherPokeRadar->checkState());

        ui->checkBoxGeneratorRadio->setCheckState(ui->checkBoxSearcherRadio->checkState());
        ui->comboBoxGeneratorRadio->setCurrentIndex(ui->comboBoxSearcherRadio->currentIndex());

        ui->checkBoxGeneratorSwarm->setCheckState(ui->checkBoxSearcherSwarm->checkState());
        ui->checkBoxGeneratorStepEncounter->setCheckState(ui->checkBoxSearcherStepEncounter->checkState());
        ui->checkBoxGeneratorWhiteFlute->setCheckState(ui->checkBoxSearcherWhiteFlute->checkState());
        ui->checkBoxGeneratorReplacement->setCheckState(ui->checkBoxSearcherReplacement->checkState());
        ui->comboBoxGeneratorReplacement0->setCurrentIndex(ui->comboBoxSearcherReplacement0->currentIndex());
        ui->comboBoxGeneratorReplacement1->setCurrentIndex(ui->comboBoxSearcherReplacement1->currentIndex());

        ui->spinBoxGeneratorPlainsBlock->setValue(ui->spinBoxSearcherPlainsBlock->value());
        ui->spinBoxGeneratorForestBlock->setValue(ui->spinBoxSearcherForestBlock->value());
        ui->spinBoxGeneratorPeakBlock->setValue(ui->spinBoxSearcherPeakBlock->value());
        ui->spinBoxGeneratorWaterBlock->setValue(ui->spinBoxSearcherWaterBlock->value());
    }
}
