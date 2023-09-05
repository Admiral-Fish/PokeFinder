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

#ifndef STATIC5_HPP
#define STATIC5_HPP

#include <QWidget>

class Profile5;
class StaticGeneratorModel5;
class StaticSearcherModel5;

namespace Ui
{
    class Static5;
}

/**
 * @brief Provides settings and filters to RNG static encounters in Gen 5 games
 */
class Static5 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Wild5 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Static5(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Wild4 object
     */
    ~Static5() override;

    /**
     * @brief Determines if any profiles exist
     *
     * @return true At least 1 profile exists
     * @return false 0 profiles exist
     */
    bool hasProfiles() const;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Static5 *ui;

    Profile5 *currentProfile;
    std::vector<Profile5> profiles;
    StaticGeneratorModel5 *generatorModel;
    StaticSearcherModel5 *searcherModel;

    /**
     * @brief Determines if the IV seed cache can be used for fast IV search
     *
     * @return true Fast search is enabled
     * @return false Fast search is not disabled
     */
    bool fastSearchEnabled() const;

private slots:
    /**
     * @brief Generates wild encounters from a starting seed
     */
    void generate();

    /**
     * @brief Updates the locations listed. Also toggles what controls are displayed based on relevance to the current settings.
     *
     * @param index Encounter index
     */
    void generatorCategoryIndexChanged(int index);

    /**
     * @brief Updates the encounter slot filter based on the pokemon
     *
     * @param index Pokemon index
     */
    void generatorPokemonIndexChanged(int index);

    /**
     * @brief Updates displayed information for a profile.  Also toggles what controls are displayed based on relevance to the current
     * settings.
     *
     * @param index Profile index
     */
    void profileIndexChanged(int index);

    /**
     * @brief Opens the profile manager
     */
    void profileManager();

    /**
     * @brief Searches static encounters from the provided IVs
     */
    void search();

    /**
     * @brief Updates the locations listed. Also toggles what controls are displayed based on relevance to the current settings.
     *
     * @param index Encounter index
     */
    void searcherCategoryIndexChanged(int index);

    /**
     * @brief Updates fast search eligibility based on IV advances and IV filters.
     */
    void searcherFastSearchChanged();

    /**
     * @brief Updates the encounter slot filter based on the pokemon
     *
     * @param index Pokemon index
     */
    void searcherPokemonIndexChanged(int index);
};

#endif // STATIC5_HPP
