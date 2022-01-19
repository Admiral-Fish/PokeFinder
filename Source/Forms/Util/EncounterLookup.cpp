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
#include <Core/Parents/Slot.hpp>
#include <Core/Util/Translator.hpp>
#include <QCompleter>
#include <QSettings>
#include <QStandardItemModel>

EncounterLookup::EncounterLookup(QWidget *parent) : QWidget(parent), ui(new Ui::EncounterLookup)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

EncounterLookup::~EncounterLookup()
{
    QSettings setting;
    setting.setValue("encounterLookup/geometry", this->saveGeometry());

    delete ui;
}

void EncounterLookup::setupModels()
{
    model = new QStandardItemModel(ui->tableView);
    model->setHorizontalHeaderLabels(QStringList() << tr("Location") << tr("Encounter Type") << tr("Level Range"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->comboBoxGame->addItem(tr("Ruby"), toInt(Game::Ruby));
    ui->comboBoxGame->addItem(tr("Sapphire"), toInt(Game::Sapphire));
    ui->comboBoxGame->addItem(tr("Fire Red"), toInt(Game::FireRed));
    ui->comboBoxGame->addItem(tr("Leaf Green"), toInt(Game::LeafGreen));
    ui->comboBoxGame->addItem(tr("Emerald"), toInt(Game::Emerald));
    ui->comboBoxGame->addItem(tr("Diamond"), toInt(Game::Diamond));
    ui->comboBoxGame->addItem(tr("Pearl"), toInt(Game::Pearl));
    ui->comboBoxGame->addItem(tr("Platinum"), toInt(Game::Platinum));
    ui->comboBoxGame->addItem(tr("Heart Gold"), toInt(Game::HeartGold));
    ui->comboBoxGame->addItem(tr("Soul Silver"), toInt(Game::SoulSilver));
    ui->comboBoxGame->addItem(tr("Brilliant Diamond"), toInt(Game::BD));
    ui->comboBoxGame->addItem(tr("Shining Pearl"), toInt(Game::SP));

    ui->comboBoxPokemon->setEditable(true);
    ui->comboBoxPokemon->setInsertPolicy(QComboBox::NoInsert);
    ui->comboBoxPokemon->completer()->setCompletionMode(QCompleter::PopupCompletion);

    connect(ui->pushButtonFind, &QPushButton::clicked, this, &EncounterLookup::find);
    connect(ui->comboBoxGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EncounterLookup::gameIndexChanged);

    gameIndexChanged(0);

    QSettings setting;
    if (setting.contains("encounterLookup/geometry"))
    {
        this->restoreGeometry(setting.value("encounterLookup/geometry").toByteArray());
    }
}

std::set<std::pair<u16, QString>> EncounterLookup::getEncounters3(Game game, u16 specie)
{
    std::set<std::pair<u16, QString>> encounters;
    Profile3 profile("", game, 0, 0);

    // Encounter variables to iterate through
    auto types = { Encounter::Grass,   Encounter::SafariZone, Encounter::RockSmash, Encounter::OldRod,
                   Encounter::GoodRod, Encounter::SuperRod,   Encounter::Surfing };

    for (const auto &type : types)
    {
        auto areas = Encounters3::getEncounters(type, profile.getVersion());
        for (const auto &area : areas)
        {
            std::vector<Slot> pokemon = area.getPokemon();
            if (std::any_of(pokemon.begin(), pokemon.end(), [specie](const auto &entry) { return entry.getSpecie() == specie; }))
            {
                QString info = getEncounterString(type);
                std::pair<u8, u8> range = area.getLevelRange(specie);
                info += QString("/%1-%2").arg(range.first).arg(range.second);
                encounters.insert(std::make_pair(area.getLocation(), info));
            }
        }
    }

    return encounters;
}

std::set<std::pair<u16, QString>> EncounterLookup::getEncounters4(Game game, u16 specie)
{
    std::set<std::pair<u16, QString>> encounters;
    std::vector<Profile4> profiles;

    // Encounter variables to iterate through
    auto types = { Encounter::Grass, Encounter::RockSmash, Encounter::OldRod, Encounter::GoodRod, Encounter::SuperRod, Encounter::Surfing };

    // Setup profiles to iterate through the different combinations of possibilities depending on HGSS vs DPPt
    if ((game & Game::HGSS) != Game::None)
    {
        for (const int radio : { 0, 1, 2 })
        {
            for (const bool swarm : { false, true })
            {
                profiles.emplace_back("", game, 0, 0, Game::None, radio, false, swarm);
            }
        }
    }
    else
    {
        auto duals = { Game::Emerald, Game::Ruby, Game::Sapphire, Game::FireRed, Game::LeafGreen };
        for (const Game dual : duals)
        {
            for (const bool swarm : { false, true })
            {
                for (const bool radar : { false, true })
                {
                    profiles.emplace_back("", game, 0, 0, dual, 0, radar, swarm);
                }
            }
        }
    }

    for (const auto &profile : profiles)
    {
        for (const auto type : types)
        {
            for (const auto &time : { 0, 1, 2 })
            {
                auto areas = Encounters4::getEncounters(type, time, profile);
                for (const auto &area : areas)
                {
                    auto pokemon = area.getPokemon();
                    if (std::any_of(pokemon.begin(), pokemon.end(), [specie](const auto &entry) { return entry.getSpecie() == specie; }))
                    {
                        QString info = getEncounterString(type);
                        std::pair<u8, u8> range = area.getLevelRange(specie);
                        info += QString("/%1-%2").arg(range.first).arg(range.second);
                        encounters.insert(std::make_pair(area.getLocation(), info));
                    }
                }
            }
        }
    }

    return encounters;
}

std::set<std::pair<u16, QString>> EncounterLookup::getEncounters8(Game game, u16 specie)
{
    std::set<std::pair<u16, QString>> encounters;
    std::vector<Profile8> profiles;

    // Encounter variables to iterate through
    auto types = { Encounter::Grass, Encounter::OldRod, Encounter::GoodRod, Encounter::SuperRod, Encounter::Surfing };

    // Setup profiles to iterate through the different combinations of possibilities for BDSP
    for (const bool swarm : { false, true })
    {
        for (const bool radar : { false, true })
        {
            profiles.emplace_back("", game, 0, 0, false, false, radar, swarm);
        }
    }

    for (const auto &profile : profiles)
    {
        for (const auto type : types)
        {
            for (const auto &time : { 0, 1, 2 })
            {
                auto areas = Encounters8::getEncounters(type, time, profile);
                for (const auto &area : areas)
                {
                    auto pokemon = area.getPokemon();
                    if (std::any_of(pokemon.begin(), pokemon.end(), [specie](const auto &entry) { return entry.getSpecie() == specie; }))
                    {
                        QString info = getEncounterString(type);
                        std::pair<u8, u8> range = area.getLevelRange(specie);
                        info += QString("/%1-%2").arg(range.first).arg(range.second);
                        encounters.insert(std::make_pair(area.getLocation(), info));
                    }
                }
            }
        }
    }

    return encounters;
}

QString EncounterLookup::getEncounterString(Encounter type)
{
    switch (type)
    {
    case Encounter::Grass:
        return tr("Grass");
    case Encounter::SafariZone:
        return tr("Safari Zone");
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
    default:
        return "-";
    }
}

void EncounterLookup::find()
{
    model->removeRows(0, model->rowCount());

    Game game = static_cast<Game>(ui->comboBoxGame->currentData().toUInt());
    u16 specie = static_cast<u16>(ui->comboBoxPokemon->currentIndex() + 1);
    std::set<std::pair<u16, QString>> encounters;
    std::vector<std::string> locationNames;

    if ((game & Game::Gen3) != Game::None)
    {
        encounters = getEncounters3(game, specie);
    }
    else if ((game & Game::Gen4) != Game::None)
    {
        encounters = getEncounters4(game, specie);
    }
    else if ((game & Game::Gen8) != Game::None)
    {
        encounters = getEncounters8(game, specie);
    }

    std::vector<u16> locations;
    std::transform(encounters.begin(), encounters.end(), std::back_inserter(locations),
                   [](const std::pair<u16, QString> &encounter) { return encounter.first; });
    locationNames = Translator::getLocations(locations, game);

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
        Game game = static_cast<Game>(ui->comboBoxGame->currentData().toUInt());
        u16 max = 0;

        if ((game & Game::Gen3) != Game::None)
        {
            max = 386;
        }
        else if ((game & Game::Gen4) != Game::None)
        {
            max = 493;
        }
        else if ((game & Game::BDSP) != Game::None)
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
