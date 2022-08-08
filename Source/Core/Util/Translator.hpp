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
    /**
     * @brief Gets the string for the provided ability
     * @param ability Ability number
     * @return Ability string
     */
    std::string *getAbility(u16 ability);

    /**
     * @brief Gets the string for the provided characteristic
     * @param characteristic Characteristic number
     * @return Characteristic string
     */
    std::string *getCharacteristic(u8 characteristic);

    /**
     * @brief Gets all the characteristic strings
     * @return Vector of characteristic strings
     */
    std::vector<std::string> *getCharacteristics();

    /**
     * @brief Gets the string for the provided game
     * @param version Game version
     * @return Game string
     */
    std::string *getGame(Game version);

    /**
     * @brief Gets the string for the provided gender
     * @param gender Gender number
     * @return Gender string
     */
    std::string *getGender(u8 gender);

    /**
     * @brief Gets all the gender strings
     * @return Vector of gender strings
     */
    std::vector<std::string> *getGenders();

    /**
     * @brief Gets the string for the provided hidden power
     * @param power Hidden power number
     * @return Hidden power string
     */
    std::string *getHiddenPower(u8 power);

    /**
     * @brief Gets all the hidden power strings
     * @return Vector of hidden power strings
     */
    std::vector<std::string> *getHiddenPowers();

    /**
     * @brief Gets the string for the provided keypress
     * @param keypress Keypress number
     * @return Keypress string
     */
    std::string *getKeypress(u8 keypress);

    /**
     * @brief Gets the keypress(es) string for the provided buttons
     * @param keypresses Selected button presses
     * @return Keypress(es) string
     */
    std::string getKeypresses(Buttons keypresses);

    /**
     * @brief Gets the strings for the provided locations/game
     * @param nums Location numbers
     * @param game Game version
     * @return Vector of location strings
     */
    std::vector<std::string> getLocations(const std::vector<u16> &nums, Game game);

    /**
     * @brief Gets the string for the provided nature
     * @param nature Nature number
     * @return Nature string
     */
    std::string *getNature(u8 nature);

    /**
     * @brief Gets all the nature strings
     * @return Vector of nature strings
     */
    std::vector<std::string> *getNatures();

    /**
     * @brief Gets the string for the provided specie
     * @param specie Specie number
     * @return Specie string
     */
    std::string *getSpecie(u16 specie);

    /**
     * @brief Gets the the strings for the provided species
     * @param specie Vector of specie numbers
     * @return Vector of specie strings
     */
    std::vector<std::string> getSpecies(const std::vector<u16> &specie);

    /**
     * @brief Initalize the Translator with what language to use. This must be called before using any other function in the namespace
     * @param locale Language
     */
    void init(const std::string &locale);
}

#endif // TRANSLATOR_HPP
