/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef UNOWN4_HPP
#define UNOWN4_HPP

#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <QMenu>

class UnownGeneratorModel4;
class UnownSearcherModel4;

namespace Ui
{
    class Unown4;
}

class Unown4 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Unown4(QWidget *parent = nullptr);
    ~Unown4() override;
    void updateProfiles();

private:
    Ui::Unown4 *ui;
    std::vector<Profile4> profiles;
    Profile4 currentProfile;
    UnownGeneratorModel4 *generatorModel = nullptr;
    UnownSearcherModel4 *searcherModel = nullptr;
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
    void generate();
    void search();
    void profilesIndexChanged(int index);
    void generatorLead();
    void seedToTime();
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void profileManager();
};

#endif // UNOWN4_HPP
