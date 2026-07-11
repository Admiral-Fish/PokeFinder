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

#ifndef PROFILEDISPLAY5_HPP
#define PROFILEDISPLAY5_HPP

#include <Core/Global.hpp>
#include <QWidget>

class Profile5;
enum class Game : u32;

namespace Ui
{
    class ProfileDisplay5;
}

/**
 * @brief Provides widget to view fields of a profile
 */
class ProfileDisplay5 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the selected profile has been changed
     */
    void profileChanged(const Profile5 &);

    /**
     * @brief Emits that the profiles have been changed
     */
    void profilesChanged(int);

public:
    /**
     * @brief Construct a new ProfileDisplay5 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    ProfileDisplay5(QWidget *parent = nullptr);

    /**
     * @brief Destroy the ProfileDisplay5 object
     */
    ~ProfileDisplay5() override;

    /**
     * @brief Determines if any profiles exist
     *
     * @return true At least 1 profile exists
     * @return false 0 profiles exist
     */
    bool hasProfiles() const;

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
    Ui::ProfileDisplay5 *ui;

    QString prefix;
    std::vector<Profile5> profiles;
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
