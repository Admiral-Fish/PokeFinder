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

#ifndef STATE5_HPP
#define STATE5_HPP

#include <Core/Global.hpp>
#include <Core/Util/DateTime.hpp>

enum class Buttons : u16;

/**
 * @brief Provides additional information to hit initial seeds in Gen 5
 *
 * @tparam StateType Interal state type
 */
template <class StateType>
class SearcherState5
{
public:
    /**
     * @brief SearcherState5
     *
     * @param dt Date/time
     * @param initialSeed Initial seed
     * @param initialAdvances Initial advances
     * @param buttons Key presses
     * @param timer0 Timer0
     * @param state Internal state
     */
    SearcherState5(const DateTime &dt, u64 initialSeed, u32 initialAdvances, Buttons buttons, u16 timer0, const StateType &state) :
        initialSeed(initialSeed), dt(dt), initialAdvances(initialAdvances), buttons(buttons), timer0(timer0), state(state)
    {
    }

    /**
     * @brief Returns the key presses of the state
     *
     * @return Key presses
     */
    Buttons getButtons() const
    {
        return buttons;
    }

    /**
     * @brief Returns the date/time of the state
     *
     * @return Date/time
     */
    DateTime getDateTime() const
    {
        return dt;
    }

    /**
     * @brief Returns the initial advances of the state
     *
     * @return Initial advances
     */
    u32 getInitialAdvances() const
    {
        return initialAdvances;
    }

    /**
     * @brief Returns the initial seed of the state
     *
     * @return Initial seed
     */
    u64 getInitialSeed() const
    {
        return initialSeed;
    }

    /**
     * @brief Returns the interal state
     *
     * @return Internal state
     */
    StateType getState() const
    {
        return state;
    }

    /**
     * @brief Returns the Timer0 of the state
     *
     * @return Timer0
     */
    u16 getTimer0() const
    {
        return timer0;
    }

private:
    u64 initialSeed;
    DateTime dt;
    u32 initialAdvances;
    Buttons buttons;
    u16 timer0;
    StateType state;
};

#endif // STATE5_HPP
