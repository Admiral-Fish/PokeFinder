/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Filters/StateFilter8.hpp>
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

Underground::Underground(QWidget *parent) : QWidget(parent), ui(new Ui::Underground)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    model = new UndergroundModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxDelay->setValues(InputType::Advance32Bit);

    ui->comboMenuLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") }, { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->comboMenuLead->addMenu(tr("Item Modifier"), { tr("Compound Eyes"), tr("Super Luck") },
                               { toInt(Lead::CompoundEyes), toInt(Lead::SuperLuck) });
    ui->comboMenuLead->addMenu(tr("Level Modifier"), { tr("Hustle"), tr("Pressure"), tr("Vital Spirit") },
                               { toInt(Lead::Hustle), toInt(Lead::Pressure), toInt(Lead::VitalSpirit) });
    ui->comboMenuLead->addMenu(tr("Synchronize"), *Translator::getNatures());

    ui->filter->disableControls(Controls::EncounterSlots);

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Underground::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Underground::generate);
    connect(ui->comboBoxStoryFlag, &QComboBox::currentIndexChanged, this, &Underground::storyFlagIndexChanged);
    connect(ui->comboBoxLocation, &QComboBox::currentIndexChanged, this, &Underground::locationIndexChanged);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Underground::profileManager);
    connect(ui->filter, &Filter::showStatsChanged, model, &UndergroundModel::setShowStats);

    updateProfiles();
    updateEncounters();

    std::vector<u16> locs;
    std::transform(encounters.begin(), encounters.end(), std::back_inserter(locs),
                   [](const UndergroundArea &area) { return area.getLocation() + 181; });

    auto locations = Translator::getLocations(locs, currentProfile->getVersion());
    std::vector<int> indices(locations.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });

    for (int i : indices)
    {
        ui->comboBoxLocation->addItem(QString::fromStdString(locations[i]), i);
    }

    QSettings setting;
    setting.beginGroup("underground");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Underground::~Underground()
{
    QSettings setting;
    setting.beginGroup("underground");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Underground::updateProfiles()
{
    profiles.clear();
    auto completeProfiles = ProfileLoader8::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile8 &profile) { return (profile.getVersion() & Game::BDSP) != Game::None; });
    profiles.insert(profiles.begin(), Profile8("-", Game::BD, 12345, 54321, false, false));

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("underground/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Underground::updateEncounters()
{
    u8 flag = ui->comboBoxStoryFlag->currentIndex() + 1;
    bool diglett = ui->checkBoxDiglett->isChecked();
    encounters = Encounters8::getEncounters(flag, diglett, currentProfile);
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
    u32 offset = ui->textBoxDelay->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    auto lead = ui->comboMenuLead->getEnum<Lead>();
    bool bonus = ui->checkBoxDiglett->isChecked();
    u8 levelFlag = ui->comboBoxLevelFlag->currentIndex();

    std::vector<u16> species = ui->checkListPokemon->getCheckedData();

    UndergroundStateFilter filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(),
                                  ui->filter->getDisableFilters(), ui->filter->getMinIVs(), ui->filter->getMaxIVs(),
                                  ui->filter->getNatures(), ui->filter->getHiddenPowers(), species);
    UndergroundGenerator generator(initialAdvances, maxAdvances, offset, tid, sid, currentProfile->getVersion(), lead, bonus, levelFlag,
                                   filter);

    auto states = generator.generate(seed0, seed1, encounters[ui->comboBoxLocation->getCurrentInt()]);
    model->addItems(states);
}

void Underground::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto area = encounters[ui->comboBoxLocation->getCurrentInt()];
        auto species = area.getSpecies();
        auto names = area.getSpecieNames();
        ui->checkListPokemon->setup(names, species);
    }
}

void Underground::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(*Translator::getGame(currentProfile->getVersion())));

        updateEncounters();
        locationIndexChanged(ui->comboBoxLocation->currentIndex());
    }
}

void Underground::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
