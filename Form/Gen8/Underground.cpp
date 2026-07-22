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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include "Underground.hpp"
#include "ui_Underground.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Filters/UndergroundFilter.hpp>
#include <Core/Gen8/Generators/UndergroundGenerator.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Gen8/UndergroundArea.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen8/Profile/ProfileManager8.hpp>
#include <Model/Gen8/UndergroundModel.hpp>
#include <QMessageBox>
#include <QSettings>

static const QString settingPrefix = QStringLiteral("underground");

Underground::Underground(QWidget *parent) : QWidget(parent), ui(new Ui::Underground)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->profileDisplay->setup(settingPrefix, Game::BDSP);

    model = new UndergroundModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxOffset->setValues(InputType::Advance32Bit);

    ui->comboMenuLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuLead->addMenu(tr("Cute Charm"), { { tr("♂ Lead"), toInt(Lead::CuteCharmM) }, { tr("♀ Lead"), toInt(Lead::CuteCharmF) } });
    ui->comboMenuLead->addMenu(tr("Item Modifier"),
                               { { tr("Compound Eyes"), toInt(Lead::CompoundEyes) }, { tr("Super Luck"), toInt(Lead::SuperLuck) } });
    ui->comboMenuLead->addMenu(tr("Level Modifier"),
                               { { tr("Hustle"), toInt(Lead::Hustle) },
                                 { tr("Pressure"), toInt(Lead::Pressure) },
                                 { tr("Vital Spirit"), toInt(Lead::VitalSpirit) } });
    ui->comboMenuLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->filter->disableControls(Controls::EncounterSlots | Controls::Level);

    ui->comboBoxLocation->enableAutoComplete();

    connect(ui->profileDisplay, &ProfileDisplay8::profileChanged, this, &Underground::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay8::profilesChanged, this, &Underground::profilesChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Underground::generate);
    connect(ui->comboBoxStoryFlag, &QComboBox::currentIndexChanged, this, &Underground::storyFlagIndexChanged);
    connect(ui->comboBoxLocation, &QComboBox::currentIndexChanged, this, &Underground::locationIndexChanged);
    connect(ui->filter, &Filter::showStatsChanged, model, &UndergroundModel::setShowStats);

    updateProfiles();
    updateEncounters();

    std::vector<u16> locs;
    std::ranges::transform(encounters, std::back_inserter(locs), [](const UndergroundArea &area) { return area.getLocation() + 181; });

    ui->comboBoxLocation->addItems(Translator::getLocations(locs, currentProfile->getVersion()));

    QSettings setting;
    setting.beginGroup(settingPrefix);
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Underground::~Underground()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Underground::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void Underground::updateEncounters()
{
    u8 flag = ui->comboBoxStoryFlag->currentIndex() + 1;
    bool diglett = ui->checkBoxDiglett->isChecked();
    encounters = Encounters8::getUndergroundEncounters(flag, diglett, currentProfile);
}

void Underground::storyFlagIndexChanged(int index)
{
    if (index >= 0)
    {
        updateEncounters();
        locationIndexChanged(ui->comboBoxLocation->currentIndex());
    }
}

void Underground::generate()
{
    u64 seed0 = ui->textBoxSeed0->getULong();
    u64 seed1 = ui->textBoxSeed1->getULong();
    if (seed0 == 0 && seed1 == 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing seeds"), tr("Please insert missing seed information"));
        msg.exec();
        return;
    }

    model->clearModel();

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u32 offset = ui->textBoxOffset->getUInt();
    auto lead = ui->comboMenuLead->getEnum<Lead>();
    bool bonus = ui->checkBoxDiglett->isChecked();
    u8 levelFlag = ui->comboBoxLevelFlag->currentIndex();

    std::vector<u16> species = ui->checkListPokemon->getCheckedData();

    UndergroundStateFilter filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(), ui->filter->getLevelMin(),
                                  ui->filter->getLevelMax(), ui->filter->getHeightMin(), ui->filter->getHeightMax(),
                                  ui->filter->getWeightMin(), ui->filter->getWeightMax(), ui->filter->getDisableFilters(),
                                  ui->filter->getMinIVs(), ui->filter->getMaxIVs(), ui->filter->getNatures(), ui->filter->getHiddenPowers(),
                                  species);
    UndergroundGenerator generator(initialAdvances, maxAdvances, offset, lead, bonus, levelFlag,
                                   encounters[ui->comboBoxLocation->currentIndex()], *currentProfile, filter);

    auto states = generator.generate(seed0, seed1);
    model->addItems(states);
}

void Underground::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        const auto &area = encounters[ui->comboBoxLocation->currentIndex()];
        auto species = area.getSpecies();
        auto names = area.getSpecieNames();
        ui->checkListPokemon->addItems(names, species);
    }
}

void Underground::profileChanged(const Profile8 &profile)
{
    currentProfile = &profile;

    updateEncounters();
    locationIndexChanged(ui->comboBoxLocation->currentIndex());
}
