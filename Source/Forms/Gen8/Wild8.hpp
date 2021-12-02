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

#ifndef WILD8_HPP
#define WILD8_HPP

#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <QMenu>

class WildModel8;

namespace Ui
{
    class Wild8;
}

class Wild8 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Wild8(QWidget *parent = nullptr);
    ~Wild8() override;
    void updateProfiles();
    bool hasProfiles() const;

private:
    Ui::Wild8 *ui;
    std::vector<Profile8> profiles;
    Profile8 currentProfile;
    WildModel8 *model = nullptr;
    QMenu *menu = nullptr;
    std::vector<EncounterArea8> encounters;

    void setupModels();
    void updateLocations();
    void updatePokemon();

private slots:
    void generate();
    void profilesIndexChanged(int index);
    void encounterIndexChanged(int index);
    void locationIndexChanged(int index);
    void pokemonIndexChanged(int index);
    void timeIndexChanged(int index);
    void tableViewContextMenu(QPoint pos);
    void profileManager();
};

#endif // WILD8_HPP
