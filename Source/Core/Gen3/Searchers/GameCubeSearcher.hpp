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

#ifndef GAMECUBESEARCHER_HPP
#define GAMECUBESEARCHER_HPP

#include <Core/Gen3/Filters/StateFilter3.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>
#include <mutex>

class StaticTemplate;
class ShadowTemplate;

/**
 * @brief Wild encounter searcher for GameCube
 */
class GameCubeSearcher : public Searcher<StateFilter3>
{
public:
    /**
     * @brief GameCubeSearcher
     *
     * @param tid
     * @param sid
     * @param version
     * @param method
     * @param unset
     * @param filter
     */

    GameCubeSearcher(u16 tid, u16 sid, Game version, Method method, bool unset, const StateFilter3 &filter);

    /**
     * @brief Cancels the running search
     */
    void cancelSearch();

    /**
     * @brief Returns the progress of the running search
     *
     * @return Progress
     */
    int getProgress() const;

    /**
     * @brief Returns the states of the running search
     *
     * @return Vector of computed states
     */
    std::vector<SearcherState3> getResults();

    /**
     * @brief Starts the search for the \p shadowTemplate
     *
     * @param min Minimum IVs
     * @param max Maximum IVs
     * @param shadowTemplate Pokemon template
     */
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, const ShadowTemplate *shadowTemplate);

    /**
     * @brief Starts the search for the \p staticTemplate
     *
     * @param min Minimum IVs
     * @param max Maximum IVs
     * @param staticTemplate Pokemon template
     */
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, const StaticTemplate *staticTemplate);

private:
    std::mutex mutex;
    std::vector<SearcherState3> results;
    int progress;
    bool searching;
    bool unset;

    /**
     * @brief Searches for matching states from provided IVs
     * @param minSpd
     * @param maxSpd
     * @param staticTemplate Pokemon template
     */
    void searchChannel(u8 minSpd, u8 maxSpd, const StaticTemplate *staticTemplate);

    /**
     * @brief Searches for matching states from provided IVs
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param shadowTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<SearcherState3> searchColoShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, const ShadowTemplate *shadowTemplate);

    /**
     * @brief Searches for matching states from provided IVs
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param shadowTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<SearcherState3> searchGalesShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, const ShadowTemplate *shadowTemplate);

    /**
     * @brief Searches for matching states from provided IVs
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param staticTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<SearcherState3> searchNonShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, const StaticTemplate *staticTemplate);

    /**
     * @brief Calculates whether the \p seed passes the Channel menu pattern
     *
     * @param seed PRNG state
     *
     * @return true Seed passes the menu pattern
     * @return false Seed does not pass the menu pattern
     */
    bool validateMenu(u32 seed);

    /**
     * @brief Calculates whether the \p seed passes the Channel Jirachi pattern
     *
     * @param seed PRNG state
     *
     * @return true Seed passes the Jirachi pattern
     * @return false Seed does not pass the Jirachi pattern
     */
    bool validateJirachi(u32 seed);
};

#endif // GAMECUBESEARCHER_HPP
