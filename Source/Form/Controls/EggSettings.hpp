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

#ifndef EGGSETTINGS_HPP
#define EGGSETTINGS_HPP

#include <Core/Global.hpp>
#include <QWidget>

class Daycare;
enum class Game : u32;

namespace Ui
{
    class EggSettings;
}

/**
 * @brief Provides selection of settings for parents and other relevant daycare options
 */
class EggSettings : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits whether inheritance should be shown
     */
    void showInheritanceChanged(bool);

public:
    /**
     * @brief Construct a new EggSettings object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    EggSettings(QWidget *parent = nullptr);

    /**
     * @brief Destroy the EggSettings object
     */
    ~EggSettings() override;

    /**
     * @brief Determines if selected settings for valid for parents in the daycare
     *
     * @return true Parents are compatible
     * @return false Parents are not compatible
     */
    bool compatibleParents() const;

    /**
     * @brief Gets various parent information: IVs, ability, gender, item masuda, etc.
     *
     * @return Parent information
     */
    Daycare getDaycareSettings() const;

    /**
     * @brief Changes order of the parents to match what the game does
     *
     * @return true Parents were reordered
     * @return false Parents were not reordered
     */
    bool reorderParents();

    /**
     * @brief Configures what should be shown and enabled based on the \p game
     *
     * @param game Game
     */
    void setup(Game game);

private:
    Ui::EggSettings *ui;
};

#endif // EGGSETTINGS_HPP
