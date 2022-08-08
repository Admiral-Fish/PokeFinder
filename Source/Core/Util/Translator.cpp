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

#include "Translator.hpp"
#include <Core/Enum/Buttons.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Resources/i18n.hpp>
#include <algorithm>
#include <bit>
#include <map>
#include <sstream>

enum class Language
{
    German,
    English,
    Spanish,
    French,
    Italian,
    Japanese,
    Korean,
    Chinese
};

enum class Translation
{
    Ability,
    BDSP,
    BW,
    BW2,
    Characteristic,
    DPPt,
    E,
    FRLG,
    Game,
    HGSS,
    Nature,
    Power,
    RS,
    Specie,
    SwSh
};

namespace
{
    Language language;
    std::vector<std::string> abilities;
    std::vector<std::string> characteristics;
    std::vector<std::string> games;
    std::vector<std::string> hiddenPowers;
    std::vector<std::string> natures;
    std::vector<std::string> species;
    std::vector<std::string> genders = { "♂", "♀", "-" };
    std::vector<std::string> buttons = { "R", "L", "X", "Y", "A", "B", "Select", "Start", "Right", "Left", "Up", "Down" };

    std::vector<std::string> readFile(Translation translation)
    {
        const u8 *data = languages[static_cast<int>(language)][static_cast<int>(translation)];
        size_t size = sizes[static_cast<int>(language)][static_cast<int>(translation)];

        std::vector<std::string> strings;
        for (size_t i = 0, start = 0; i < size; i++)
        {
            if (data[i] == 0)
            {
                std::string t(reinterpret_cast<const char *>(&data[start]), i - start);
                strings.emplace_back(reinterpret_cast<const char *>(&data[start]), i - start);
                start = i + 1;
            }
        }

        return strings;
    }

}

namespace Translator
{
    std::string *getAbility(u16 ability)
    {
        return &abilities[ability];
    }

    std::string *getCharacteristic(u8 characteristic)
    {
        return &characteristics[characteristic];
    }

    std::vector<std::string> *getCharacteristics()
    {
        return &characteristics;
    }

    std::string *getGame(Game version)
    {
        // Strings are ordered to match the enum
        // Use countr_zero to get the bit that is set
        return &games[std::countr_zero(toInt(version))];
    }

    std::string *getGender(u8 gender)
    {
        return &genders[gender];
    }

    std::vector<std::string> *getGenders()
    {
        return &genders;
    }

    std::string *getHiddenPower(u8 power)
    {
        return &hiddenPowers[power];
    }

    std::vector<std::string> *getHiddenPowers()
    {
        return &hiddenPowers;
    }

    std::string *getKeypress(u8 keypress)
    {
        return &buttons[keypress];
    }

    std::string getKeypresses(Buttons keypresses)
    {
        if (keypresses == Buttons::None)
        {
            return "None";
        }

        std::string result;
        for (int i = 0; i < 12; i++)
        {
            if (toInt(keypresses) & (1 << i))
            {
                if (!result.empty())
                {
                    result += " + ";
                }

                result += buttons[i];
            }
        }

        return result;
    }

    std::vector<std::string> getLocations(const std::vector<u16> &nums, Game game)
    {
        std::vector<std::string> strings;
        if ((game & Game::Emerald) != Game::None)
        {
            strings = readFile(Translation::E);
        }
        else if ((game & Game::FRLG) != Game::None)
        {
            strings = readFile(Translation::FRLG);
        }
        else if ((game & Game::RS) != Game::None)
        {
            strings = readFile(Translation::RS);
        }
        else if ((game & Game::DPPt) != Game::None)
        {
            strings = readFile(Translation::DPPt);
        }
        else if ((game & Game::HGSS) != Game::None)
        {
            strings = readFile(Translation::HGSS);
        }
        else if ((game & Game::BW) != Game::None)
        {
            strings = readFile(Translation::BW);
        }
        else if ((game & Game::BW2) != Game::None)
        {
            strings = readFile(Translation::BW2);
        }
        else if ((game & Game::SwSh) != Game::None)
        {
            strings = readFile(Translation::SwSh);
        }
        else
        {
            strings = readFile(Translation::BDSP);
        }

        std::map<int, std::string> map;
        for (const std::string &string : strings)
        {
            std::vector<std::string> entry;

            std::string token;
            std::istringstream stream(string);
            while (std::getline(stream, token, ','))
            {
                entry.emplace_back(token);
            }

            map.emplace(std::stoi(entry[0]), entry[1]);
        }

        std::vector<std::string> locations;
        std::transform(nums.begin(), nums.end(), std::back_inserter(locations), [&map](u16 num) { return map[num]; });

        return locations;
    }

    std::string *getNature(u8 nature)
    {
        return &natures[nature];
    }

    std::vector<std::string> *getNatures()
    {
        return &natures;
    }

    std::string *getSpecie(u16 specie)
    {
        return &species[specie - 1];
    }

    std::vector<std::string> getSpecies(const std::vector<u16> &specie)
    {
        std::vector<std::string> s;
        std::transform(specie.begin(), specie.end(), std::back_inserter(s), [](u16 num) { return species[num - 1]; });
        return s;
    }

    void init(const std::string &locale)
    {
        if (locale == "de")
        {
            language = Language::German;
        }
        else if (locale == "en")
        {
            language = Language::English;
        }
        else if (locale == "es")
        {
            language = Language::Spanish;
        }
        else if (locale == "it")
        {
            language = Language::Italian;
        }
        else if (locale == "fr")
        {
            language = Language::French;
        }
        else if (locale == "ja")
        {
            language = Language::Japanese;
        }
        else if (locale == "ko")
        {
            language = Language::Korean;
        }
        else if (locale == "zh")
        {
            language = Language::Chinese;
        }

        abilities = readFile(Translation::Ability);
        characteristics = readFile(Translation::Characteristic);
        games = readFile(Translation::Game);
        hiddenPowers = readFile(Translation::Power);
        natures = readFile(Translation::Nature);
        species = readFile(Translation::Specie);
    }
}
