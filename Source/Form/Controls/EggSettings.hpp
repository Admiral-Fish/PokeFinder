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
    void toggleInheritance(bool);

public:
    /**
     * @brief Creates a new DateTimeEdit
     * @param parent Parent widget, which takes memory ownership
     */
    explicit EggSettings(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~EggSettings() override;

    /**
     * @brief Configures what should be shown and enabled based on the provided game
     * @param version Game
     */
    void setup(Game version);

    /**
     * @brief Gets various parent information: IVs, ability, gender, item masuda, etc.
     * @return Parent information
     */
    Daycare getDaycareSettings() const;

    /**
     * @brief Determines if selected settings for valid for parents in the daycare
     * @return True if compatible, false otherwise
     */
    bool compatibleParents() const;

    /**
     * @brief Changes order of the parents to match what the game does
     * @return True if the order was changed, false otherwise
     */
    bool reorderParents();

private:
    Ui::EggSettings *ui;

private slots:
    /**
     * @brief Shows inherited IVs if selected or IV value if not selected
     * @param checked Whether or not the check box is selected
     */
    void showInheritance(bool checked);
};

#endif // EGGSETTINGS_HPP
