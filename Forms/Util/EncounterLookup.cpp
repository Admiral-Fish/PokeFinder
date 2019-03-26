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

#include "EncounterLookup.hpp"
#include "ui_EncounterLookup.h"

EncounterLookup::EncounterLookup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EncounterLookup)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

EncounterLookup::~EncounterLookup()
{
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
}

QVector<QPair<u8, QString>> EncounterLookup::getEncounters3(Game game, u16 specie)
{
    QVector<QPair<u8, QString>> encounters;
    QVector<Encounter> types = { Encounter::Grass, Encounter::OldRod, Encounter::GoodRod, Encounter::SuperRod };
    Profile3 profile("", game, 0, 0);

    for (auto type : types)
    {
        QVector<EncounterArea3> areas = Encounters3(type, profile).getEncounters();
        for (const auto &area : areas)
        {
            QVector<Slot> pokemon = area.getPokemon();
            for (auto entry : pokemon)
            {
                if (entry.getSpecie() == specie)
                {
                    QString info = getEncounterString(type);
                    info += "/" + QString::number(entry.getMinLevel()) + "-" + QString::number(entry.getMaxLevel());
                    encounters.append(qMakePair(area.getLocation(), info));
                    break;
                }
            }
        }
    }

    return encounters;
}

QVector<QPair<u8, QString>> EncounterLookup::getEncounters4(Game game, u16 specie)
{
    QVector<QPair<u8, QString>> encounters;
    QVector<Encounter> types = { Encounter::Grass, Encounter::OldRod, Encounter::GoodRod, Encounter::SuperRod };
    Profile4 profile("", game, 0, 0);

    // TODO: dual slot / radio / swarm / etc
    for (auto type : types)
    {
        QVector<EncounterArea4> areas = Encounters4(type, 0, profile).getEncounters();
        for (const auto &area : areas)
        {
            QVector<Slot> pokemon = area.getPokemon();
            for (auto entry : pokemon)
            {
                if (entry.getSpecie() == specie)
                {
                    QString info = getEncounterString(type);
                    info += "/" + QString::number(entry.getMinLevel()) + "-" + QString::number(entry.getMaxLevel());
                    encounters.append(qMakePair(area.getLocation(), info));
                    break;
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
        case Encounter::Surfing:
            return tr("Surfing");
        case Encounter::OldRod:
            return tr("Old Rod");
        case Encounter::GoodRod:
            return tr("Good Rod");
        case Encounter::SuperRod:
            return tr("Super Rod");
        default:
            return "-";
    }
}

void EncounterLookup::on_pushButtonFind_clicked()
{
    model->removeRows(0, model->rowCount());

    Game game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());
    u16 specie = ui->comboBoxPokemon->currentIndex() + 1;
    QVector<QPair<u8, QString>> encounters;
    QVector<u8> locations;
    QStringList locationNames;

    if (game & FRLG || game & RSE)
    {
        encounters = getEncounters3(game, specie);
        for (const auto &encounter : encounters)
        {
            locations.append(encounter.first);
        }
        locationNames = Translator::getLocationsGen3(locations, game);
    }
    else if (game & DPPt || game & HGSS)
    {
        encounters = getEncounters4(game, specie);
        for (const auto &encounter : encounters)
        {
            locations.append(encounter.first);
        }
        locationNames = Translator::getLocationsGen4(locations, game);
    }

    for (u16 i = 0; i < encounters.size(); i++)
    {
        QList<QStandardItem *> row;
        QStringList split = encounters[i].second.split('/');
        row << new QStandardItem(locationNames[i]) << new QStandardItem(split[0]) << new QStandardItem(split[1]);
        model->appendRow(row);
    }
}

void EncounterLookup::on_comboBoxGame_currentIndexChanged(int index)
{
    if (index >= 0)
    {
        Game game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());
        int max;

        if (game & FRLG || game & RSE)
        {
            max = 386;
        }
        else if (game & HGSS || game & DPPt)
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
