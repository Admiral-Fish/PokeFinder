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

#ifndef PICKUP_HPP
#define PICKUP_HPP

#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Gen5/Generators/PickupGenerator.hpp>
#include <QWidget>
#include <array>
#include <vector>

class ComboBox;
class ComboBoxProxy;
class PickupGeneratorModel5;
class PickupSearcherModel5;
class Profile5;
class QPushButton;
class QCheckBox;
class QSpinBox;
class SortFilterProxyModel;
class TextBox;

namespace Ui
{
    class Pickup;
}

class Pickup : public QWidget
{
    Q_OBJECT
signals:
    void profilesChanged(int);

public:
    Pickup(QWidget *parent = nullptr);
    ~Pickup() override;

    bool hasProfiles() const;

public slots:
    void updateProfiles();

private:
    Ui::Pickup *ui;

    PickupGeneratorModel5 *generatorModel;
    PickupSearcherModel5 *searcherModel;
    Profile5 *currentProfile = nullptr;
    SortFilterProxyModel *proxyModel;
    std::vector<Profile5> profiles;

    std::array<QCheckBox *, 6> generatorSlots;
    std::array<QCheckBox *, 6> searcherSlots;
    std::array<QSpinBox *, 6> generatorLevels;
    std::array<QSpinBox *, 6> searcherLevels;
    std::array<ComboBox *, 6> generatorItems;
    std::array<ComboBox *, 6> searcherItems;
    std::array<ComboBox *, 6> searcherItemCounts;
    std::array<QPushButton *, 6> searcherItemClearButtons;
    TextBox *generatorIVAdvances;
    ComboBox *generatorEncounter;
    ComboBox *generatorSeason;
    ComboBoxProxy *generatorLocation;
    ComboBox *generatorPokemon;
    ComboBox *searcherEncounter;
    ComboBox *searcherSeason;
    ComboBoxProxy *searcherLocation;
    std::vector<EncounterArea5> encounterGenerator;
    std::vector<EncounterArea5> encounterSearcher;

    void setupPickupUi();
    void updateEncounterLocations(ComboBox *encounter, ComboBox *season, ComboBoxProxy *location, std::vector<EncounterArea5> &areas);
    std::array<PickupGenerator::Slot, 6> getSettings(const std::array<QCheckBox *, 6> &pickupSlots,
                                                     const std::array<QSpinBox *, 6> &levels,
                                                     const std::array<ComboBox *, 6> &items) const;
    std::array<PickupGenerator::Slot, 6> getSearcherSettings() const;
    std::vector<PickupGenerator::Request> getSearcherRequests() const;
    void updateItemList(QCheckBox *slot, QSpinBox *level, ComboBox *items) const;
    void updateSearcherItemLists();

private slots:
    void generate();
    void search();
    void profileIndexChanged(int index);
    void profileManager();
    void transferSettings(int index);
    void generatorEncounterIndexChanged(int index);
    void generatorSeasonIndexChanged(int index);
    void generatorLocationIndexChanged(int index);
    void generatorPokemonIndexChanged(int index);
    void searcherEncounterIndexChanged(int index);
    void searcherSeasonIndexChanged(int index);
};

#endif // PICKUP_HPP
