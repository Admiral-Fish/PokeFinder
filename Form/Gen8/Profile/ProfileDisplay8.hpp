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

#ifndef PROFILEDISPLAY8_HPP
#define PROFILEDISPLAY8_HPP

#include <Core/Global.hpp>
#include <QWidget>

class Profile8;
enum class Game : u32;

namespace Ui
{
    class ProfileDisplay8;
}

/**
 * @brief Provides widget to view fields of a profile
 */
class ProfileDisplay8 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the selected profile has been changed
     */
    void profileChanged(const Profile8 &);

    /**
     * @brief Emits that the profiles have been changed
     */
    void profilesChanged(int);

public:
    /**
     * @brief Construct a new ProfileDisplay8 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    ProfileDisplay8(QWidget *parent = nullptr);

    /**
     * @brief Destroy the ProfileDisplay8 object
     */
    ~ProfileDisplay8() override;

    /**
     * @brief Sets the QSetting group prefix and version filter
     *
     * @param prefix Setting prefix
     * @param filter Game version filter
     */
    void setup(const QString &prefix, Game filter);

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::ProfileDisplay8 *ui;

    QString prefix;
    std::vector<Profile8> profiles;
    Game filter;

private slots:
    /**
     * @brief Updates displayed information for a profile
     *
     * @param index Profile index
     */
    void profileIndexChanged(int index);

    /**
     * @brief Opens the profile manager
     */
    void profileManager();
};

#endif // PROFILEDISPLAY5_HPP
