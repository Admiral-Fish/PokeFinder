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

#ifndef SHA1CACHEFINDER_HPP
#define SHA1CACHEFINDER_HPP

#include <QWidget>

class Profile5;

namespace Ui
{
    class SHA1CacheFinder;
}

/**
 * @brief Provides settings to generate a SHA1 seed cache
 */
class SHA1CacheFinder: public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new SHA1CacheFinder object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    SHA1CacheFinder(QWidget *parent = nullptr);

    /**
     * @brief Destroy the SHA1CacheFinder object
     */
    ~SHA1CacheFinder() override;

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
    Ui::SHA1CacheFinder *ui;

    Profile5 *currentProfile;
    std::vector<Profile5> profiles;

private slots:
    /**
     * @brief Updates displayed information for a profile.
     *
     * @param index Profile index
     */
    void profileIndexChanged(int index);

    /**
     * @brief Opens the profile manager
     */
    void profileManager();

    /**
     * @brief Generates the cache file for the selected profile
     */
    void search();

    /**
     * @brief Prompts user to select a file to output the cache to
     */
    void updateOutputFile();
};

#endif // SHA1CACHEFINDER_HPP
