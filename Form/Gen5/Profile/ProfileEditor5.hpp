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

#ifndef PROFILEEDITOR5_HPP
#define PROFILEEDITOR5_HPP

#include <Core/Global.hpp>
#include <QDialog>

namespace Ui
{
    class ProfileEditor5;
}

class ProfileSearcherState5;
class Profile5;
enum class DSType : u8;
enum class Game : u32;
enum class Language : u8;

/**
 * @brief Provides dialog to view/edit fields of a profile
 */
class ProfileEditor5 : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new ProfileEditor5 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    ProfileEditor5(QWidget *parent = nullptr);

    /**
     * @brief Construct a new ProfileEditor5 object
     *
     * @param profile Existing profile to populate the dialog
     * @param parent Parent widget, which takes memory ownership
     */
    ProfileEditor5(const Profile5 &profile, QWidget *parent = nullptr);

    /**
     * @brief Construct a new ProfileEditor5 object
     *
     * @param version Profile game
     * @param language Profile language
     * @param dsType Profile DS type
     * @param mac Profile MAC address
     * @param calibration Profile calibration information
     * @param parent Parent widget, which takes memory ownership
     */
    ProfileEditor5(Game version, Language language, DSType dsType, u64 mac, const ProfileSearcherState5 &calibration,
                   QWidget *parent = nullptr);

    /**
     * @brief Destroy the ProfileEditor4 object
     */
    ~ProfileEditor5() override;

    /**
     * @brief Creates finalized profile based on input fields
     *
     * @return Profile information
     */
    Profile5 getProfile();

private:
    Ui::ProfileEditor5 *ui;

private slots:
    /**
     * @brief Clears the selected IV cache
     */
    void clearIVCache();

    /**
     * @brief Clears the selected SHA cache
     */
    void clearSHACache();

    /**
     * @brief Opens window to calibrate a new profile
     */
    void findParameters();

    /**
     * @brief Validates that a profile name exists before allowing the dialog to be closed
     */
    void okay();

    /**
     * @brief Prompts user to select IV cache
     */
    void selectIVCache();

    /**
     * @brief Prompts user to select SHA cache
     */
    void selectSHACache();

    /**
     * @brief Toggles whether memory link, and shiny charm is enabled based on selected Game
     *
     * @param index Current index of combo box
     */
    void versionIndexChanged(int index);
};

#endif // PROFILEEDITOR5_HPP
