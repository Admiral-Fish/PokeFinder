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

#ifndef HIDDENGROTTO_HPP
#define HIDDENGROTTO_HPP

#include <QWidget>

class HiddenGrottoArea;
class HiddenGrottoGeneratorModel5;
class HiddenGrottoSearcherModel5;
class HiddenGrottoSlotGeneratorModel5;
class HiddenGrottoSlotSearcherModel5;
class IVCache;
class Profile5;
class SHA1Cache;
class SortFilterProxyModel;

namespace Ui
{
    class HiddenGrotto;
}

/**
 * @brief Provides settings and filters to RNG hidden grotto in Gen 5
 */
class HiddenGrotto : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new HiddenGrotto object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    HiddenGrotto(QWidget *parent = nullptr);

    /**
     * @brief Destroy the HiddenGrotto object
     */
    ~HiddenGrotto() override;

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
    Ui::HiddenGrotto *ui;

    HiddenGrottoSlotGeneratorModel5 *grottoGeneratorModel;
    HiddenGrottoSlotSearcherModel5 *grottoSearcherModel;
    HiddenGrottoGeneratorModel5 *pokemonGeneratorModel;
    HiddenGrottoSearcherModel5 *pokemonSearcherModel;
    IVCache *ivCache;
    Profile5 *currentProfile;
    SHA1Cache *shaCache;
    SortFilterProxyModel *grottoProxyModel;
    SortFilterProxyModel *pokemonProxyModel;
    std::vector<HiddenGrottoArea> encounter;
    std::vector<Profile5> profiles;

    /**
     * @brief Determines if the IV seed cache can be used for fast IV search
     *
     * @return true Fast search is enabled
     * @return false Fast search is not disabled
     */
    bool fastSearchEnabled() const;

private slots:
    /**
     * @brief Generates grotto encounters from a starting seed
     */
    void grottoGenerate();

    /**
     * @brief Updates the grotto listed
     *
     * @param index Location index
     */
    void grottoGeneratorLocationIndexChanged(int index);

    /**
     * @brief Updates generator filter with selected pokemon and item
     */
    void grottoGeneratorUpdateFilter();

    /**
     * @brief Searches grotto encounters from date range
     */
    void grottoSearch();

    /**
     * @brief Updates the grotto listed
     *
     * @param index Location index
     */
    void grottoSearcherLocationIndexChanged(int index);

    /**
     * @brief Updates searcher filter with selected pokemon and item
     */
    void grottoSearcherUpdateFilter();

    /**
     * @brief Generates grotto pokemon from a starting seed
     */
    void pokemonGenerate();

    /**
     * @brief Updates the group listed
     *
     * @param index Group index
     */
    void pokemonGeneratorGroupIndexChanged(int index);

    /**
     * @brief Updates the grotto listed
     *
     * @param index Location index
     */
    void pokemonGeneratorLocationIndexChanged(int index);

    /**
     * @brief Updates the pokemon listed
     *
     * @param index Pokemonindex
     */
    void pokemonGeneratorPokemonIndexChanged(int index);

    /**
     * @brief Searches grotto pokemon from date range
     */
    void pokemonSearch();

    /**
     * @brief Updates fast search eligibility based on IV advances and IV filters.
     */
    void pokemonSearcherFastSearchChanged();

    /**
     * @brief Updates the group listed
     *
     * @param index Group index
     */
    void pokemonSearcherGroupIndexChanged(int index);

    /**
     * @brief Updates the grotto listed
     *
     * @param index Location index
     */
    void pokemonSearcherLocationIndexChanged(int index);

    /**
     * @brief Updates the pokemon listed
     *
     * @param index Pokemon index
     */
    void pokemonSearcherPokemonIndexChanged(int index);

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

#endif // HIDDENGROTTO_HPP
