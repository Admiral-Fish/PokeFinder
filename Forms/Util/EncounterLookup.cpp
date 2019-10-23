/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <QSettings>
#include "EncounterLookup.hpp"
#include "ui_EncounterLookup.h"
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Util/Encounter.hpp>
#include <Core/Util/Game.hpp>
#include <Core/Util/Translator.hpp>

namespace PokeFinderForms
{
    EncounterLookup::EncounterLookup(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::EncounterLookup)
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

        ui->comboBoxGame->addItem(tr("Ruby"), PokeFinderCore::Game::Ruby);
        ui->comboBoxGame->addItem(tr("Sapphire"), PokeFinderCore::Game::Sapphire);
        ui->comboBoxGame->addItem(tr("Fire Red"), PokeFinderCore::Game::FireRed);
        ui->comboBoxGame->addItem(tr("Leaf Green"), PokeFinderCore::Game::LeafGreen);
        ui->comboBoxGame->addItem(tr("Emerald"), PokeFinderCore::Game::Emerald);
        ui->comboBoxGame->addItem(tr("Diamond"), PokeFinderCore::Game::Diamond);
        ui->comboBoxGame->addItem(tr("Pearl"), PokeFinderCore::Game::Pearl);
        ui->comboBoxGame->addItem(tr("Platinum"), PokeFinderCore::Game::Platinum);
        ui->comboBoxGame->addItem(tr("Heart Gold"), PokeFinderCore::Game::HeartGold);
        ui->comboBoxGame->addItem(tr("Soul Silver"), PokeFinderCore::Game::SoulSilver);

        QSettings setting;
        if (setting.contains("encounterLookup/geometry")) this->restoreGeometry(setting.value("encounterLookup/geometry").toByteArray());
    }

    QSet<QPair<u8, QString>> EncounterLookup::getEncounters3(PokeFinderCore::Game game, u16 specie)
    {
        QSet<QPair<u8, QString>> encounters;
        PokeFinderCore::Profile3 profile("", game, 0, 0);

        // Encounter variables to iterate through
        QVector<PokeFinderCore::Encounter> types =
        {
            PokeFinderCore::Encounter::Grass, PokeFinderCore::Encounter::SafariZone, PokeFinderCore::Encounter::RockSmash,
            PokeFinderCore::Encounter::OldRod, PokeFinderCore::Encounter::GoodRod, PokeFinderCore::Encounter::SuperRod
        };

        for (const auto &type : types)
        {
            QVector<PokeFinderCore::EncounterArea3> areas = PokeFinderCore::Encounters3(type, profile).getEncounters();
            for (const auto &area : areas)
            {
                QVector<PokeFinderCore::Slot> pokemon = area.getPokemon();
                for (const auto &entry : pokemon)
                {
                    if (entry.getSpecie() == specie)
                    {
                        QString info = getEncounterString(type);
                        QPair<u8, u8> range = area.getLevelRange(specie);
                        info += QString("/%1-%2").arg(range.first).arg(range.second);
                        encounters.insert(qMakePair(area.getLocation(), info));
                        break;
                    }
                }
            }
        }

        return encounters;
    }

    QSet<QPair<u8, QString>> EncounterLookup::getEncounters4(PokeFinderCore::Game game, u16 specie)
    {
        QSet<QPair<u8, QString>> encounters;
        QVector<PokeFinderCore::Profile4> profiles;

        // Encounter variables to iterate through
        QVector<PokeFinderCore::Encounter> types =
        {
            PokeFinderCore::Encounter::Grass, PokeFinderCore::Encounter::RockSmash, PokeFinderCore::Encounter::OldRod,
            PokeFinderCore::Encounter::GoodRod, PokeFinderCore::Encounter::SuperRod
        };
        QVector<PokeFinderCore::Game> duals =
        {
            PokeFinderCore::Game::Emerald, PokeFinderCore::Game::Ruby, PokeFinderCore::Game::Sapphire,
            PokeFinderCore::Game::FireRed, PokeFinderCore::Game::LeafGreen
        };

        // Setup profiles to iterate through of the different combinations of possibilities depending on HGSS vs DPPt
        if (game & PokeFinderCore::Game::HGSS)
        {
            for (const auto &radio : { 0, 1, 2 })
            {
                for (const auto &swarm : { false, true })
                {
                    profiles.append(PokeFinderCore::Profile4("", game, 0, 0, PokeFinderCore::Game::Blank, radio, PokeFinderCore::Language::English, false, swarm));
                }
            }
        }
        else
        {
            for (const auto &dual : duals)
            {
                for (const auto &swarm : { false, true })
                {
                    for (const auto &radar : { false, true })
                    {
                        profiles.append(PokeFinderCore::Profile4("", game, 0, 0, dual, 0, PokeFinderCore::Language::English, radar, swarm));
                    }
                }
            }
        }

        for (const auto &profile : profiles)
        {
            for (const auto &type : types)
            {
                for (const auto &time : { 0, 1, 2 })
                {
                    QVector<PokeFinderCore::EncounterArea4> areas = PokeFinderCore::Encounters4(type, time, profile).getEncounters();
                    for (const auto &area : areas)
                    {
                        QVector<PokeFinderCore::Slot> pokemon = area.getPokemon();
                        for (const auto &entry : pokemon)
                        {
                            if (entry.getSpecie() == specie)
                            {
                                QString info = getEncounterString(type);
                                QPair<u8, u8> range = area.getLevelRange(specie);
                                info += QString("/%1-%2").arg(range.first).arg(range.second);
                                encounters.insert(qMakePair(area.getLocation(), info));
                                break;
                            }
                        }
                    }
                }
            }
        }

        return encounters;
    }

    QString EncounterLookup::getEncounterString(PokeFinderCore::Encounter type)
    {
        switch (type)
        {
            case PokeFinderCore::Encounter::Grass:
                return tr("Grass");
            case PokeFinderCore::Encounter::SafariZone:
                return tr("Safari Zone");
            case PokeFinderCore::Encounter::Surfing:
                return tr("Surfing");
            case PokeFinderCore::Encounter::OldRod:
                return tr("Old Rod");
            case PokeFinderCore::Encounter::GoodRod:
                return tr("Good Rod");
            case PokeFinderCore::Encounter::SuperRod:
                return tr("Super Rod");
            case PokeFinderCore::Encounter::RockSmash:
                return tr("Rock Smash");
            default:
                return "-";
        }
    }

    void EncounterLookup::on_pushButtonFind_clicked()
    {
        model->removeRows(0, model->rowCount());

        PokeFinderCore::Game game = static_cast<PokeFinderCore::Game>(ui->comboBoxGame->currentData().toInt());
        u16 specie = ui->comboBoxPokemon->currentIndex() + 1;
        QSet<QPair<u8, QString>> encounters;
        QVector<u8> locations;
        QStringList locationNames;

        if (game & PokeFinderCore::Game::FRLG || game & PokeFinderCore::Game::RSE)
        {
            encounters = getEncounters3(game, specie);
            for (const auto &encounter : encounters)
            {
                locations.append(encounter.first);
            }
            locationNames = PokeFinderCore::Translator::getLocationsGen3(locations, game);
        }
        else if (game & PokeFinderCore::Game::DPPt || game & PokeFinderCore::Game::HGSS)
        {
            encounters = getEncounters4(game, specie);
            for (const auto &encounter : encounters)
            {
                locations.append(encounter.first);
            }
            locationNames = PokeFinderCore::Translator::getLocationsGen4(locations, game);
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

    void EncounterLookup::on_comboBoxGame_currentIndexChanged(int index)
    {
        if (index >= 0)
        {
            PokeFinderCore::Game game = static_cast<PokeFinderCore::Game>(ui->comboBoxGame->currentData().toInt());
            u16 max = 0;

            if (game & PokeFinderCore::Game::FRLG || game & PokeFinderCore::Game::RSE)
            {
                max = 386;
            }
            else if (game & PokeFinderCore::Game::HGSS || game & PokeFinderCore::Game::DPPt)
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
            ui->comboBoxPokemon->addItems(PokeFinderCore::Translator::getSpecies(nums));
            if (oldIndex >= 0 && oldIndex < nums.size())
            {
                ui->comboBoxPokemon->setCurrentIndex(oldIndex);
            }
        }
    }
}
