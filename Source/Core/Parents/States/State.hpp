/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef STATE_HPP
#define STATE_HPP

#include <Core/Global.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <array>

/**
 * @brief Parent state that contains all the common information for a Pokemon across each game
 */
class State
{
public:
    /**
     * @brief Construct a new State object
     *
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param info Pokemon information
     */
    State(u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny, const PersonalInfo *info) :
        ec(pid),
        pid(pid),
        abilityIndex(info->getAbility(ability)),
        ivs(ivs),
        ability(ability),
        gender(gender),
        level(level),
        nature(nature),
        shiny(shiny)
    {
        updateStats(info);
    }

    /**
     * @brief Construct a new State object
     *
     * @param ec Pokemon EC
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param info Pokemon information
     */
    State(u32 ec, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny, const PersonalInfo *info) :
        ec(ec),
        pid(pid),
        abilityIndex(info->getAbility(ability)),
        ivs(ivs),
        ability(ability),
        gender(gender),
        level(level),
        nature(nature),
        shiny(shiny)
    {
        updateStats(info);
    }

    /**
     * @brief Returns the ability of the pokemon
     *
     * @return Pokemon ability
     */
    u8 getAbility() const
    {
        return ability;
    }

    /**
     * @brief Returns the ability index of the pokemon
     *
     * @return Pokemon ability index
     */
    u16 getAbilityIndex() const
    {
        return abilityIndex;
    }

    /**
     * @brief Returns the characteristic of the pokemon
     *
     * @return Pokemon characteristic
     */
    u8 getCharacteristic() const
    {
        return characteristic;
    }

    /**
     * @brief Returns the EC of the pokemon
     *
     * @return Pokemon EC
     */
    u32 getEC() const
    {
        return ec;
    }

    /**
     * @brief Returns the gender of the pokemon
     *
     * @return Pokemon gender
     */
    u8 getGender() const
    {
        return gender;
    }

    /**
     * @brief Returns the hidden power of the pokemon
     *
     * @return Pokemon hidden power
     */
    u8 getHiddenPower() const
    {
        return hiddenPower;
    }

    /**
     * @brief Returns the hidden power strength of the pokemon
     *
     * @return Pokemon hidden power strength
     */
    u8 getHiddenPowerStrength() const
    {
        return hiddenPowerStrength;
    }

    /**
     * @brief Returns the specified IV of the pokemon
     *
     * @param index IV index to get
     *
     * @return Pokemon IV
     */
    u8 getIV(int index) const
    {
        return ivs[index];
    }

    /**
     * @brief Returns the IVs of the pokemon
     *
     * @return Pokemon IVs
     */
    std::array<u8, 6> getIVs() const
    {
        return ivs;
    }

    /**
     * @brief Returns the level of the pokemon
     *
     * @return Pokemon level
     */
    u8 getLevel() const
    {
        return level;
    }

    /**
     * @brief Returns the nature of the pokemon
     *
     * @return Pokemon nature
     */
    u8 getNature() const
    {
        return nature;
    }

    /**
     * @brief Returns the PID of the pokemon
     *
     * @return Pokemon PID
     */
    u32 getPID() const
    {
        return pid;
    }

    /**
     * @brief Returns the shininess of the pokemon
     *
     * @return Pokemon shininess
     */
    u8 getShiny() const
    {
        return shiny;
    }

    /**
     * @brief Returns the specified stat of the pokemon
     *
     * @param index Stat index to get
     *
     * @return Pokemon stat
     */
    u16 getStat(int index) const
    {
        return stats[index];
    }

    /**
     * @brief Returns the stats of the pokemon
     *
     * @return Pokemon stats
     */
    std::array<u16, 6> getStats() const
    {
        return stats;
    }

protected:
    u32 ec;
    u32 pid;
    std::array<u16, 6> stats;
    u16 abilityIndex;
    std::array<u8, 6> ivs;
    u8 ability;
    u8 characteristic;
    u8 gender;
    u8 hiddenPower;
    u8 hiddenPowerStrength;
    u8 level;
    u8 nature;
    u8 shiny;

    /**
     * @brief Updates characterstic, hidden power, and stats
     *
     * @param info Pokemon information
     */
    void updateStats(const PersonalInfo *info);
};

/**
 * @brief Parent state class that provides additional information from the generator
 */
class GeneratorState : public State
{
public:
    /**
     * @brief Construct a new GeneratorState object
     *
     * @param advances Advances of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param info Pokemon information
     */
    GeneratorState(u32 advances, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny,
                   const PersonalInfo *info) :
        State(pid, ivs, ability, gender, level, nature, shiny, info), advances(advances)
    {
    }

    /**
     * @brief Construct a new GeneratorState object
     *
     * @param advances Advances of the state
     * @param ec Pokemon EC
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param info Pokemon information
     */
    GeneratorState(u32 advances, u32 ec, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny,
                   const PersonalInfo *info) :
        State(ec, pid, ivs, ability, gender, level, nature, shiny, info), advances(advances)
    {
    }

    /**
     * @brief Returns the advances of the state
     *
     * @return State advances
     */
    u32 getAdvances() const
    {
        return advances;
    }

protected:
    u32 advances;
};

/**
 * @brief Parent state class that provides additional information from the searcher
 */
class SearcherState : public State
{
public:
    /**
     * @brief Construct a new SearcherState object
     *
     * @param seed Seed of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param nature Pokemon nature
     * @param level Pokemon level
     * @param info Pokemon information
     */
    SearcherState(u32 seed, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny,
                  const PersonalInfo *info) :
        State(pid, ivs, ability, gender, level, nature, shiny, info), seed(seed)
    {
    }

    /**
     * @brief Returns the seed of the state
     *
     * @return State seed
     */
    u32 getSeed() const
    {
        return seed;
    }

protected:
    u32 seed;
};

#endif // STATE_HPP
