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

#include "EncounterLookup.hpp"
#include "ui_EncounterLookup.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Util/Translator.hpp>
#include <QSettings>
#include <QStandardItemModel>

EncounterLookup::EncounterLookup(QWidget *parent) : QWidget(parent), ui(new Ui::EncounterLookup)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new QStandardItemModel(ui->tableView);
    model->setHorizontalHeaderLabels(QStringList() << tr("Location") << tr("Encounter Type") << tr("Level Range"));
    ui->tableView->setModel(model);

    ui->comboBoxGame->setup({ toInt(Game::Ruby), toInt(Game::Sapphire), toInt(Game::FireRed), toInt(Game::LeafGreen), toInt(Game::Emerald),
                              toInt(Game::Diamond), toInt(Game::Pearl), toInt(Game::Platinum), toInt(Game::HeartGold),
                              toInt(Game::SoulSilver), toInt(Game::BD), toInt(Game::SP) });

    ui->comboBoxPokemon->enableAutoComplete();

    connect(ui->pushButtonFind, &QPushButton::clicked, this, &EncounterLookup::find);
    connect(ui->comboBoxGame, &QComboBox::currentIndexChanged, this, &EncounterLookup::gameIndexChanged);

    gameIndexChanged(0);

    QSettings setting;
    if (setting.contains("encounterLookup/geometry"))
    {
        this->restoreGeometry(setting.value("encounterLookup/geometry").toByteArray());
    }
}

EncounterLookup::~EncounterLookup()
{
    QSettings setting;
    setting.setValue("encounterLookup/geometry", this->saveGeometry());

    delete ui;
}

QString EncounterLookup::getEncounterString(Encounter type)
{
    switch (type)
    {
    case Encounter::Grass:
        return tr("Grass");
    case Encounter::Surfing:
        return tr("Surfing");
    case Encounter::OldRod:
        return tr("Old Rod");
    case Encounter::GoodRod:
        return tr("Good Rod");
    case Encounter::SuperRod:
        return tr("Super Rod");
    case Encounter::RockSmash:
        return tr("Rock Smash");
    case Encounter::Headbutt:
    case Encounter::HeadbuttAlt:
    case Encounter::HeadbuttSpecial:
        return tr("Headbutt");
    case Encounter::BugCatchingContest:
        return tr("Bug Catching Contest");
    default:
        return "-";
    }
}

std::set<std::pair<u16, QString>> EncounterLookup::getEncounters3(Game version, u16 specie)
{
    std::set<std::pair<u16, QString>> encounters;
    Profile3 profile("", version, 0, 0, false);

    // Encounter variables to iterate through
    auto types = { Encounter::Grass, Encounter::RockSmash, Encounter::OldRod, Encounter::GoodRod, Encounter::SuperRod, Encounter::Surfing };

    for (const auto &type : types)
    {
        auto areas = Encounters3::getEncounters(type, profile.getVersion());
        for (const auto &area : areas)
        {
            auto pokemon = area.getPokemon();
            if (std::any_of(pokemon.begin(), pokemon.end(), [specie](const auto &entry) { return entry.getSpecie() == specie; }))
            {
                std::pair<u8, u8> range = area.getLevelRange(specie);
                QString info = QString("%1/%2-%3").arg(getEncounterString(type)).arg(range.first).arg(range.second);
                encounters.insert(std::make_pair(area.getLocation(), info));
            }
        }
    }

    return encounters;
}

std::set<std::pair<u16, QString>> EncounterLookup::getEncounters4(Game version, u16 specie)
{
    std::set<std::pair<u16, QString>> encounters;
    Profile4 profile("", version, 0, 0, false);

    // Encounter variables to iterate through
    auto types = { Encounter::Grass, Encounter::OldRod, Encounter::GoodRod, Encounter::SuperRod, Encounter::Surfing };
    auto altTypes
        = { Encounter::RockSmash, Encounter::BugCatchingContest, Encounter::Headbutt, Encounter::HeadbuttAlt, Encounter::HeadbuttSpecial };
    auto duals = { Game::None, Game::Ruby, Game::Sapphire, Game::Emerald, Game::FireRed, Game::LeafGreen };
    auto radars = { false, true };
    auto radios = { 0, 1, 2 };
    auto swarms = { false, true };
    auto times = { 0, 1, 2 };
    std::array<u16, 2> replacement = { 0, 0 };
    std::array<u8, 5> blocks = { 0, 0, 0, 0, 0 };

    for (auto type : types)
    {
        for (auto time : times)
        {
            if ((version & Game::DPPt) != Game::None)
            {
                for (auto dual : duals)
                {
                    for (auto radar : radars)
                    {
                        for (auto swarm : swarms)
                        {
                            auto areas = Encounters4::getEncounters(type, time, dual, radar, 0, swarm, replacement, blocks, &profile);
                            for (const auto &area : areas)
                            {
                                auto pokemon = area.getPokemon();
                                if (std::any_of(pokemon.begin(), pokemon.end(),
                                                [specie](const auto &entry) { return entry.getSpecie() == specie; }))
                                {
                                    std::pair<u8, u8> range = area.getLevelRange(specie);
                                    QString info = QString("%1/%2-%3").arg(getEncounterString(type)).arg(range.first).arg(range.second);
                                    encounters.insert(std::make_pair(area.getLocation(), info));
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                for (auto radio : radios)
                {
                    for (auto swarm : swarms)
                    {
                        auto areas = Encounters4::getEncounters(type, time, Game::None, false, radio, swarm, replacement, blocks, &profile);
                        for (const auto &area : areas)
                        {
                            auto pokemon = area.getPokemon();
                            if (std::any_of(pokemon.begin(), pokemon.end(),
                                            [specie](const auto &entry) { return entry.getSpecie() == specie; }))
                            {
                                std::pair<u8, u8> range = area.getLevelRange(specie);
                                QString info = QString("%1/%2-%3").arg(getEncounterString(type)).arg(range.first).arg(range.second);
                                encounters.insert(std::make_pair(area.getLocation(), info));
                            }
                        }
                    }
                }
            }
        }
    }

    for (auto type : altTypes)
    {
        auto areas = Encounters4::getEncounters(type, 0, Game::None, false, false, false, replacement, blocks, &profile);
        for (const auto &area : areas)
        {
            auto pokemon = area.getPokemon();
            if (std::any_of(pokemon.begin(), pokemon.end(), [specie](const auto &entry) { return entry.getSpecie() == specie; }))
            {
                std::pair<u8, u8> range = area.getLevelRange(specie);
                QString info = QString("%1/%2-%3").arg(getEncounterString(type)).arg(range.first).arg(range.second);
                encounters.insert(std::make_pair(area.getLocation(), info));
            }
        }
    }

    return encounters;
}

std::set<std::pair<u16, QString>> EncounterLookup::getEncounters8(Game version, u16 specie)
{
    std::set<std::pair<u16, QString>> encounters;
    Profile8 profile("", version, 0, 0, false, false, false);

    // Encounter variables to iterate through
    auto types = { Encounter::Grass, Encounter::OldRod, Encounter::GoodRod, Encounter::SuperRod, Encounter::Surfing };
    auto radars = { false, true };
    auto swarms = { false, true };
    std::array<u16, 2> replacement = { 0, 0 };
    auto times = { 0, 1, 2 };

    for (auto type : types)
    {
        for (auto time : times)
        {
            for (auto swarm : swarms)
            {
                for (auto radar : radars)
                {
                    auto areas = Encounters8::getEncounters(type, time, radar, swarm, replacement, &profile);
                    for (const auto &area : areas)
                    {
                        auto pokemon = area.getPokemon();
                        if (std::any_of(pokemon.begin(), pokemon.end(),
                                        [specie](const auto &entry) { return entry.getSpecie() == specie; }))
                        {
                            std::pair<u8, u8> range = area.getLevelRange(specie);
                            QString info = QString("%1/%2-%3").arg(getEncounterString(type)).arg(range.first).arg(range.second);
                            encounters.insert(std::make_pair(area.getLocation(), info));
                        }
                    }
                }
            }
        }
    }

    return encounters;
}

void EncounterLookup::find()
{
    model->removeRows(0, model->rowCount());

    auto version = ui->comboBoxGame->getEnum<Game>();
    u16 specie = static_cast<u16>(ui->comboBoxPokemon->currentIndex() + 1);
    std::set<std::pair<u16, QString>> encounters;

    if ((version & Game::Gen3) != Game::None)
    {
        encounters = getEncounters3(version, specie);
    }
    else if ((version & Game::Gen4) != Game::None)
    {
        encounters = getEncounters4(version, specie);
    }
    else if ((version & Game::Gen8) != Game::None)
    {
        encounters = getEncounters8(version, specie);
    }

    std::vector<u16> locations;
    std::vector<std::string> locationNames;

    std::transform(encounters.begin(), encounters.end(), std::back_inserter(locations),
                   [](const std::pair<u16, QString> &encounter) { return encounter.first; });
    locationNames = Translator::getLocations(locations, version);

    u16 i = 0;
    for (const auto &encounter : encounters)
    {
        QList<QStandardItem *> row;
        QStringList split = encounter.second.split('/');
        row << new QStandardItem(QString::fromStdString(locationNames[i++])) << new QStandardItem(split[0]) << new QStandardItem(split[1]);
        model->appendRow(row);
    }
}

void EncounterLookup::gameIndexChanged(int index)
{
    if (index >= 0)
    {
        auto version = ui->comboBoxGame->getEnum<Game>();
        u16 max = 0;

        if ((version & Game::Gen3) != Game::None)
        {
            max = 386;
        }
        else if ((version & Game::Gen4) != Game::None)
        {
            max = 493;
        }
        else if ((version & Game::BDSP) != Game::None)
        {
            max = 493;
        }

        std::vector<u16> nums;
        for (u16 i = 1; i <= max; i++)
        {
            nums.emplace_back(i);
        }

        int oldIndex = ui->comboBoxPokemon->currentIndex();

        ui->comboBoxPokemon->clear();
        for (const std::string &specie : Translator::getSpecies(nums))
        {
            ui->comboBoxPokemon->addItem(QString::fromStdString(specie));
        }

        if (oldIndex >= 0 && oldIndex < nums.size())
        {
            ui->comboBoxPokemon->setCurrentIndex(oldIndex);
        }
    }
}
