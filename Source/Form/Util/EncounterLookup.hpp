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

#ifndef ENCOUNTERLOOKUP_HPP
#define ENCOUNTERLOOKUP_HPP

#include <Core/Global.hpp>
#include <QWidget>
#include <set>

class QStandardItemModel;
enum class Encounter : u8;
enum class Game : u32;

namespace Ui
{
    class EncounterLookup;
}

/**
 * @brief Allows mapping of Pokemon to wild encounters for various games
 */
class EncounterLookup : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new EncounterLookup object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    EncounterLookup(QWidget *parent = nullptr);

    /**
     * @brief Destroy the EncounterLookup object
     */
    ~EncounterLookup() override;

private:
    Ui::EncounterLookup *ui;

    QStandardItemModel *model;

    /**
     * @brief Converts encounter enum to string
     *
     * @param type Encounter type
     *
     * @return Encounter string
     */
    QString getEncounterString(Encounter type);

    /**
     * @brief Gets wild encounters for Gen 3
     *
     * @param version Game version
     * @param specie Pokemon specie
     *
     * @return Set of pokemon locations and level ranges
     */
    std::set<std::pair<u16, QString>> getEncounters3(Game version, u16 specie);

    /**
     * @brief Gets wild encounters for Gen 4
     *
     * @param version Game version
     * @param specie Pokemon specie
     *
     * @return Set of pokemon locations and level ranges
     */
    std::set<std::pair<u16, QString>> getEncounters4(Game version, u16 specie);

    /**
     * @brief Gets wild encounters for Gen 5
     *
     * @param version Game version
     * @param specie Pokemon specie
     *
     * @return Set of pokemon locations and level ranges
     */
    std::set<std::pair<u16, QString>> getEncounters5(Game version, u16 specie);

    /**
     * @brief Gets wild encounters for Gen 8
     *
     * @param version Game version
     * @param specie Pokemon specie
     *
     * @return Set of pokemon locations and level ranges
     */
    std::set<std::pair<u16, QString>> getEncounters8(Game version, u16 specie);

private slots:
    /**
     * @brief Lookups encounters for the selected pokemon and game
     */
    void find();

    /**
     * @brief Updates the pokemon listed based upon the selected game
     *
     * @param index Game index
     */
    void gameIndexChanged(int index);
};

#endif // ENCOUNTERLOOKUP_HPP
