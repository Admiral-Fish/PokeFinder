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

#ifndef IDS5_HPP
#define IDS5_HPP

#include <QWidget>

class IDModel5;
class Profile5;

namespace Ui
{
    class IDs5;
}

/**
 * @brief Provides settings and filters to RNG TID/SID in Gen 5 games
 */
class IDs5 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new IDs5 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    IDs5(QWidget *parent = nullptr);

    /**
     * @brief Destroy the IDs5 object
     */
    ~IDs5() override;

    /**
     * @brief Determines if any profiles exist
     *
     * @return true At least 1 profile exists
     * @return false 0 profiles exist
     */
    bool hasProfiles() const;

public slots:
    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::IDs5 *ui;

    IDModel5 *model;
    Profile5 *currentProfile;
    std::vector<Profile5> profiles;

private slots:
    /**
     * @brief Lists possible candidates for the TID obtained
     */
    void find();

    /**
     * @brief Searches and displays results
     */
    void search();

    /**
     * @brief Toggles whether or not the XOR check controls are enabled
     *
     * @param checked Whether or not to enable
     */
    void setXOR(bool checked);

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

    /**
     * @brief Transfers the current Searcher filters to the Generator.
     */
    void transferFiltersToGenerator();

    /**
     * @brief Transfers the current Searcher settings to the Generator.
     */
    void transferSettingsToGenerator();
};

#endif // IDS5_HPP
