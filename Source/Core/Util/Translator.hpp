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

#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <Core/Global.hpp>
#include <string>
#include <vector>

enum class Buttons: u16;
enum class Game : u32;

namespace Translator
{
    void init(const std::string &locale);
    std::vector<std::string> *getCharacteristic();
    std::string *getCharacteristic(u8 characteristic);
    std::vector<std::string> *getNatures();
    std::string *getNature(u8 nature);
    std::string *getAbility(u16 ability);
    std::vector<std::string> *getHiddenPowers();
    std::string *getHiddenPower(u8 power);
    std::string *getSpecies(u16 specie);
    std::vector<std::string> getSpecies(const std::vector<u16> &nums);
    std::string *getGender(u8 gender);
    std::vector<std::string> *getGenders();
    std::vector<std::string> getLocations(const std::vector<u16> &nums, Game game);
    std::string *getKeypress(u8 keypress);
    std::string getKeypresses(Buttons keypresses);
}

#endif // TRANSLATOR_HPP
