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
 * @brief The EggSettings class
 */
class EggSettings : public QWidget
{
    Q_OBJECT

signals:
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
    ~EggSettings();

    /**
     * @brief setup
     * @param version
     */
    void setup(Game version);

    /**
     * @brief getDaycareSettings
     * @return
     */
    Daycare getDaycareSettings() const;

    /**
     * @brief compatibleParents
     * @return
     */
    bool compatibleParents() const;

    /**
     * @brief reorderParents
     * @return
     */
    bool reorderParents();

private:
    Ui::EggSettings *ui;

private slots:
    /**
     * @brief showInheritance
     * @param checked
     */
    void showInheritance(bool checked);
};

#endif // EGGSETTINGS_HPP
