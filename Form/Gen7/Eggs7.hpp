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

#ifndef EGGS7_HPP
#define EGGS7_HPP

#include <QWidget>

class EggModel7;
class Profile7;

namespace Ui
{
    class Eggs7;
}

/**
 * @brief Provides settings and filters to RNG egg encounters in Gen 7 games
 */
class Eggs7 final : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been changed
     */
    void profilesChanged(int);

public:
    /**
     * @brief Construct a new Eggs7 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Eggs7(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Eggs7 object
     */
    ~Eggs7() override;

public slots:
    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Eggs7 *ui;

    EggModel7 *model;
    const Profile7 *currentProfile;

private slots:
    /**
     * @brief Generates static encounters from a starting seed
     */
    void generate();

    /**
     * @brief Updates showing profile related information
     *
     * @param profile Selected profile
     */
    void profileChanged(const Profile7 &profile);
};

#endif // EGGS7_HPP
