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

#ifndef WILD4_HPP
#define WILD4_HPP

#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <QMenu>

class WildState;
class WildGeneratorModel4;
class WildSearcherModel4;

namespace Ui
{
    class Wild4;
}

class Wild4 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Wild4(QWidget *parent = nullptr);
    ~Wild4() override;
    void updateProfiles();

private:
    Ui::Wild4 *ui;
    std::vector<Profile4> profiles;
    Profile4 currentProfile;
    WildGeneratorModel4 *generatorModel = nullptr;
    WildSearcherModel4 *searcherModel = nullptr;
    QMenu *generatorMenu = nullptr;
    QMenu *searcherMenu = nullptr;
    std::vector<EncounterArea4> encounterGenerator;
    std::vector<EncounterArea4> encounterSearcher;

    void setupModels();
    void updateLocationsGenerator();
    void updateLocationsSearcher();
    void updatePokemonGenerator();
    void updatePokemonSearcher();

private slots:
    void updateProgress(const std::vector<WildState> &states, int progress);
    void generate();
    void search();
    void profilesIndexChanged(int index);
    void generatorLead();
    void generatorEncounterIndexChanged(int index);
    void searcherEncounterIndexChanged(int index);
    void generatorLocationIndexChanged(int index);
    void searcherLocationIndexChanged(int index);
    void generatorPokemonIndexChanged(int index);
    void searcherPokemonIndexChanged(int index);
    void generatorTimeIndexChanged(int index);
    void searcherTimeIndexChanged(int index);
    void seedToTime();
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void profileManager();
};

#endif // WILD4_HPP
