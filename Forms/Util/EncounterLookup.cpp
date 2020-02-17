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

    ui->comboBoxGame->addItem(tr("Ruby"), Game::Ruby);
    ui->comboBoxGame->addItem(tr("Sapphire"), Game::Sapphire);
    ui->comboBoxGame->addItem(tr("Fire Red"), Game::FireRed);
    ui->comboBoxGame->addItem(tr("Leaf Green"), Game::LeafGreen);
    ui->comboBoxGame->addItem(tr("Emerald"), Game::Emerald);
    ui->comboBoxGame->addItem(tr("Diamond"), Game::Diamond);
    ui->comboBoxGame->addItem(tr("Pearl"), Game::Pearl);
    ui->comboBoxGame->addItem(tr("Platinum"), Game::Platinum);
    ui->comboBoxGame->addItem(tr("Heart Gold"), Game::HeartGold);
    ui->comboBoxGame->addItem(tr("Soul Silver"), Game::SoulSilver);

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

QSet<QPair<u8, QString>> EncounterLookup::getEncounters3(Game game, u16 specie)
{
    QSet<QPair<u8, QString>> encounters;
    Profile3 profile("", game, 0, 0);

    // Encounter variables to iterate through
    auto types
        = { Encounter::Grass, Encounter::SafariZone, Encounter::RockSmash, Encounter::OldRod, Encounter::GoodRod, Encounter::SuperRod };

    for (const auto &type : types)
    {
        auto areas = Encounters3::getEncounters(type, profile);
        for (const auto &area : areas)
        {
            QVector<Slot> pokemon = area.getPokemon();
            if (std::any_of(pokemon.begin(), pokemon.end(), [specie](const auto &entry) { return entry.getSpecie() == specie; }))
            {
                QString info = getEncounterString(type);
                QPair<u8, u8> range = area.getLevelRange(specie);
                info += QString("/%1-%2").arg(range.first).arg(range.second);
                encounters.insert(qMakePair(area.getLocation(), info));
            }
        }
    }

    return encounters;
}

QSet<QPair<u8, QString>> EncounterLookup::getEncounters4(Game game, u16 specie)
{
    QSet<QPair<u8, QString>> encounters;
    QVector<Profile4> profiles;

    // Encounter variables to iterate through
    auto types = { Encounter::Grass, Encounter::RockSmash, Encounter::OldRod, Encounter::GoodRod, Encounter::SuperRod };
    auto duals = { Game::Emerald, Game::Ruby, Game::Sapphire, Game::FireRed, Game::LeafGreen };

    // Setup profiles to iterate through of the different combinations of possibilities depending on HGSS vs DPPt
    if (game & Game::HGSS)
    {
        for (const auto radio : { 0, 1, 2 })
        {
            for (const auto swarm : { false, true })
            {
                profiles.append(Profile4("", game, 0, 0, Game::Blank, radio, false, swarm));
            }
        }
    }
    else
    {
        for (const auto dual : duals)
        {
            for (const auto swarm : { false, true })
            {
                for (const auto radar : { false, true })
                {
                    profiles.append(Profile4("", game, 0, 0, dual, 0, radar, swarm));
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
                        QPair<u8, u8> range = area.getLevelRange(specie);
                        info += QString("/%1-%2").arg(range.first).arg(range.second);
                        encounters.insert(qMakePair(area.getLocation(), info));
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

    Game game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());
    u16 specie = static_cast<u16>(ui->comboBoxPokemon->currentIndex() + 1);
    QSet<QPair<u8, QString>> encounters;
    QVector<u8> locations;
    QStringList locationNames;

    if (game & Game::FRLG || game & Game::RSE)
    {
        encounters = getEncounters3(game, specie);
        for (const auto &encounter : encounters)
        {
            locations.append(encounter.first);
        }
        locationNames = Translator::getLocations(locations, game);
    }
    else if (game & Game::DPPt || game & Game::HGSS)
    {
        encounters = getEncounters4(game, specie);
        for (const auto &encounter : encounters)
        {
            locations.append(encounter.first);
        }
        locationNames = Translator::getLocations(locations, game);
    }

    u16 i = 0;
    for (const auto &encounter : encounters)
    {
        QList<QStandardItem *> row;
        QStringList split = encounter.second.split('/');
        row << new QStandardItem(locationNames[i++]) << new QStandardItem(split.at(0)) << new QStandardItem(split.at(1));
        model->appendRow(row);
    }
}

void EncounterLookup::gameIndexChanged(int index)
{
    if (index >= 0)
    {
        Game game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());
        u16 max = 0;

        if (game & Game::FRLG || game & Game::RSE)
        {
            max = 386;
        }
        else if (game & Game::HGSS || game & Game::DPPt)
        {
            max = 493;
        }

        QVector<u16> nums;
        for (u16 i = 1; i <= max; i++)
        {
            nums.append(i);
        }

        int oldIndex = ui->comboBoxPokemon->currentIndex();
        ui->comboBoxPokemon->clear();
        ui->comboBoxPokemon->addItems(Translator::getSpecies(nums));
        if (oldIndex >= 0 && oldIndex < nums.size())
        {
            ui->comboBoxPokemon->setCurrentIndex(oldIndex);
        }
    }
}
