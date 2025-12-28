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

#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <Core/Global.hpp>
#include <array>
#include <string>
#include <vector>

enum class Buttons : u16;
enum class Game : u32;

/**
 * @brief Provides methods to get strings in the desired language
 */
namespace Translator
{
    /**
     * @brief Gets the string for the \p ability
     *
     * @param ability Ability number
     *
     * @return Ability string
     */
    const std::string &getAbility(u16 ability);

    /**
     * @brief Gets the string for the \p characteristic
     *
     * @param characteristic Characteristic number
     *
     * @return Characteristic string
     */
    const std::string &getCharacteristic(u8 characteristic);

    /**
     * @brief Gets all the characteristic strings
     *
     * @return Vector of characteristic strings
     */
    const std::vector<std::string> &getCharacteristics();

    /**
     * @brief Gets the string for the \p specie and \p form
     *
     * @param specie Specie number
     * @param form Form number
     *
     * @return Form string
     */
    const std::string &getForm(u16 specie, u8 form);

    /**
     * @brief Gets the string for the \p game
     *
     * @param version Game version
     *
     * @return Game string
     */
    const std::string &getGame(Game version);

    /**
     * @brief Gets the string for the \p gender
     *
     * @param gender Gender number
     *
     * @return Gender string
     */
    const std::string &getGender(u8 gender);

    /**
     * @brief Gets all the gender strings
     *
     * @return Vector of gender strings
     */
    const std::array<std::string, 3> &getGenders();

    /**
     * @brief Gets the string for the \p power
     *
     * @param power Hidden power number
     *
     * @return Hidden power string
     */
    const std::string &getHiddenPower(u8 power);

    /**
     * @brief Gets all the hidden power strings
     *
     * @return Vector of hidden power strings
     */
    const std::vector<std::string> &getHiddenPowers();

    /**
     * @brief Gets the string for the \p item
     *
     * @param item Item number
     *
     * @return Item string
     */
    const std::string &getItem(u16 item);

    /**
     * @brief Gets the the strings for the \p items
     *
     * @param item Vector of item numbers
     *
     * @return Vector of item strings
     */
    std::vector<std::string> getItems(const std::vector<u16> &item);

    /**
     * @brief Gets the string for the \p keypress
     *
     * @param keypress Keypress number
     *
     * @return Keypress string
     */
    const std::string &getKeypress(u8 keypress);

    /**
     * @brief Gets the strings for the \p keypresses
     *
     * @param keypresses Selected button presses
     *
     * @return Keypresses string
     */
    std::string getKeypresses(Buttons keypresses);

    /**
     * @brief Gets the strings for the provided locations/game
     *
     * @param nums Location numbers
     * @param game Game version
     *
     * @return Vector of location strings
     */
    std::vector<std::string> getLocations(const std::vector<u16> &nums, Game game);

    /**
     * @brief Gets the string for the \p move
     *
     * @param move Move number
     *
     * @return Move string
     */
    const std::string &getMove(u16 move);

    /**
     * @brief Gets the string for the \p nature
     *
     * @param nature Nature number
     *
     * @return Nature string
     */
    const std::string &getNature(u8 nature);

    /**
     * @brief Gets all the nature strings
     *
     * @return Vector of nature strings
     */
    const std::vector<std::string> &getNatures();

    /**
     * @brief Gets the string for the \p specie
     *
     * @param specie Specie number
     *
     * @return Specie string
     */
    const std::string &getSpecie(u16 specie);

    /**
     * @brief Gets the string for the \p specie and \p form
     *
     * @param specie Specie number
     * @param form Form number
     *
     * @return Specie string
     */
    std::string getSpecie(u16 specie, u8 form);

    /**
     * @brief Gets the the strings up until the \p max specie
     *
     * @param max Specie number to stop at
     *
     * @return Vector of specie strings
     */
    std::vector<std::string> getSpecies(u16 max);

    /**
     * @brief Gets the the strings for the \p species
     *
     * @param specie Vector of specie numbers
     *
     * @return Vector of specie strings
     */
    std::vector<std::string> getSpecies(const std::vector<u16> &specie);

    /**
     * @brief Initalize the Translator with what \p locale to use.
     * This must be called before using any other function in the namespace.
     *
     * @param locale Language
     */
    void init(const std::string &locale);
}

#endif // TRANSLATOR_HPP
