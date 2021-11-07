/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/Game.hpp>
#include <Core/Resources/i18n.hpp>
#include <algorithm>
#include <map>
#include <sstream>

namespace
{
    std::string language;
    std::vector<std::string> characteristics;
    std::vector<std::string> natures;
    std::vector<std::string> abilities;
    std::vector<std::string> hiddenPowers;
    std::vector<std::string> species;
    const std::vector<std::string> genders = { "♂", "♀", "-" };
    const std::vector<std::string> buttons = { "R", "L", "X", "Y", "A", "B", "Select", "Start", "Right", "Left", "Up", "Down" };

    std::vector<std::string> readFile(const std::string &name)
    {
        const u8 *data;
        size_t size = 0;

        if (language == "de")
        {
            if (name == "characteristic")
            {
                data = characteristic_de.data();
                size = characteristic_de.size();
            }
            else if (name == "natures")
            {
                data = natures_de.data();
                size = natures_de.size();
            }
            else if (name == "abilities")
            {
                data = abilities_de.data();
                size = abilities_de.size();
            }
            else if (name == "powers")
            {
                data = powers_de.data();
                size = powers_de.size();
            }
            else if (name == "species")
            {
                data = species_de.data();
                size = species_de.size();
            }
            else if (name == "frlg")
            {
                data = frlg_de.data();
                size = frlg_de.size();
            }
            else if (name == "rse")
            {
                data = rse_de.data();
                size = rse_de.size();
            }
            else if (name == "dppt")
            {
                data = dppt_de.data();
                size = dppt_de.size();
            }
            else if (name == "hgss")
            {
                data = hgss_de.data();
                size = hgss_de.size();
            }
            else if (name == "swsh")
            {
                data = swsh_de.data();
                size = swsh_de.size();
            }
        }
        else if (language == "en")
        {
            if (name == "characteristic")
            {
                data = characteristic_en.data();
                size = characteristic_en.size();
            }
            else if (name == "natures")
            {
                data = natures_en.data();
                size = natures_en.size();
            }
            else if (name == "abilities")
            {
                data = abilities_en.data();
                size = abilities_en.size();
            }
            else if (name == "powers")
            {
                data = powers_en.data();
                size = powers_en.size();
            }
            else if (name == "species")
            {
                data = species_en.data();
                size = species_en.size();
            }
            else if (name == "frlg")
            {
                data = frlg_en.data();
                size = frlg_en.size();
            }
            else if (name == "rse")
            {
                data = rse_en.data();
                size = rse_en.size();
            }
            else if (name == "dppt")
            {
                data = dppt_en.data();
                size = dppt_en.size();
            }
            else if (name == "hgss")
            {
                data = hgss_en.data();
                size = hgss_en.size();
            }
            else if (name == "swsh")
            {
                data = swsh_en.data();
                size = swsh_en.size();
            }
        }
        else if (language == "es")
        {
            if (name == "characteristic")
            {
                data = characteristic_es.data();
                size = characteristic_es.size();
            }
            else if (name == "natures")
            {
                data = natures_es.data();
                size = natures_es.size();
            }
            else if (name == "abilities")
            {
                data = abilities_es.data();
                size = abilities_es.size();
            }
            else if (name == "powers")
            {
                data = powers_es.data();
                size = powers_es.size();
            }
            else if (name == "species")
            {
                data = species_es.data();
                size = species_es.size();
            }
            else if (name == "frlg")
            {
                data = frlg_es.data();
                size = frlg_es.size();
            }
            else if (name == "rse")
            {
                data = rse_es.data();
                size = rse_es.size();
            }
            else if (name == "dppt")
            {
                data = dppt_es.data();
                size = dppt_es.size();
            }
            else if (name == "hgss")
            {
                data = hgss_es.data();
                size = hgss_es.size();
            }
            else if (name == "swsh")
            {
                data = swsh_es.data();
                size = swsh_es.size();
            }
        }
        else if (language == "fr")
        {
            if (name == "characteristic")
            {
                data = characteristic_fr.data();
                size = characteristic_fr.size();
            }
            else if (name == "natures")
            {
                data = natures_fr.data();
                size = natures_fr.size();
            }
            else if (name == "abilities")
            {
                data = abilities_fr.data();
                size = abilities_fr.size();
            }
            else if (name == "powers")
            {
                data = powers_fr.data();
                size = powers_fr.size();
            }
            else if (name == "species")
            {
                data = species_fr.data();
                size = species_fr.size();
            }
            else if (name == "frlg")
            {
                data = frlg_fr.data();
                size = frlg_fr.size();
            }
            else if (name == "rse")
            {
                data = rse_fr.data();
                size = rse_fr.size();
            }
            else if (name == "dppt")
            {
                data = dppt_fr.data();
                size = dppt_fr.size();
            }
            else if (name == "hgss")
            {
                data = hgss_fr.data();
                size = hgss_fr.size();
            }
            else if (name == "swsh")
            {
                data = swsh_fr.data();
                size = swsh_fr.size();
            }
        }
        else if (language == "it")
        {
            if (name == "characteristic")
            {
                data = characteristic_it.data();
                size = characteristic_it.size();
            }
            else if (name == "natures")
            {
                data = natures_it.data();
                size = natures_it.size();
            }
            else if (name == "abilities")
            {
                data = abilities_it.data();
                size = abilities_it.size();
            }
            else if (name == "powers")
            {
                data = powers_it.data();
                size = powers_it.size();
            }
            else if (name == "species")
            {
                data = species_it.data();
                size = species_it.size();
            }
            else if (name == "frlg")
            {
                data = frlg_it.data();
                size = frlg_it.size();
            }
            else if (name == "rse")
            {
                data = rse_it.data();
                size = rse_it.size();
            }
            else if (name == "dppt")
            {
                data = dppt_it.data();
                size = dppt_it.size();
            }
            else if (name == "hgss")
            {
                data = hgss_it.data();
                size = hgss_it.size();
            }
            else if (name == "swsh")
            {
                data = swsh_it.data();
                size = swsh_it.size();
            }
        }
        else if (language == "ja")
        {
            if (name == "characteristic")
            {
                data = characteristic_ja.data();
                size = characteristic_ja.size();
            }
            else if (name == "natures")
            {
                data = natures_ja.data();
                size = natures_ja.size();
            }
            else if (name == "abilities")
            {
                data = abilities_ja.data();
                size = abilities_ja.size();
            }
            else if (name == "powers")
            {
                data = powers_ja.data();
                size = powers_ja.size();
            }
            else if (name == "species")
            {
                data = species_ja.data();
                size = species_ja.size();
            }
            else if (name == "frlg")
            {
                data = frlg_ja.data();
                size = frlg_ja.size();
            }
            else if (name == "rse")
            {
                data = rse_ja.data();
                size = rse_ja.size();
            }
            else if (name == "dppt")
            {
                data = dppt_ja.data();
                size = dppt_ja.size();
            }
            else if (name == "hgss")
            {
                data = hgss_ja.data();
                size = hgss_ja.size();
            }
            else if (name == "swsh")
            {
                data = swsh_ja.data();
                size = swsh_ja.size();
            }
        }
        else if (language == "ko")
        {
            if (name == "characteristic")
            {
                data = characteristic_ko.data();
                size = characteristic_ko.size();
            }
            else if (name == "natures")
            {
                data = natures_ko.data();
                size = natures_ko.size();
            }
            else if (name == "abilities")
            {
                data = abilities_ko.data();
                size = abilities_ko.size();
            }
            else if (name == "powers")
            {
                data = powers_ko.data();
                size = powers_ko.size();
            }
            else if (name == "species")
            {
                data = species_ko.data();
                size = species_ko.size();
            }
            else if (name == "frlg")
            {
                data = frlg_ko.data();
                size = frlg_ko.size();
            }
            else if (name == "rse")
            {
                data = rse_ko.data();
                size = rse_ko.size();
            }
            else if (name == "dppt")
            {
                data = dppt_ko.data();
                size = dppt_ko.size();
            }
            else if (name == "hgss")
            {
                data = hgss_ko.data();
                size = hgss_ko.size();
            }
            else if (name == "swsh")
            {
                data = swsh_ko.data();
                size = swsh_ko.size();
            }
        }
        else if (language == "zh")
        {
            if (name == "characteristic")
            {
                data = characteristic_zh.data();
                size = characteristic_zh.size();
            }
            else if (name == "natures")
            {
                data = natures_zh.data();
                size = natures_zh.size();
            }
            else if (name == "abilities")
            {
                data = abilities_zh.data();
                size = abilities_zh.size();
            }
            else if (name == "powers")
            {
                data = powers_zh.data();
                size = powers_zh.size();
            }
            else if (name == "species")
            {
                data = species_zh.data();
                size = species_zh.size();
            }
            else if (name == "frlg")
            {
                data = frlg_zh.data();
                size = frlg_zh.size();
            }
            else if (name == "rse")
            {
                data = rse_zh.data();
                size = rse_zh.size();
            }
            else if (name == "dppt")
            {
                data = dppt_zh.data();
                size = dppt_zh.size();
            }
            else if (name == "hgss")
            {
                data = hgss_zh.data();
                size = hgss_zh.size();
            }
            else if (name == "swsh")
            {
                data = swsh_zh.data();
                size = swsh_zh.size();
            }
        }

        std::vector<std::string> strings;
        size_t start = 0;
        for (size_t i = 0; i < size; i++)
        {
            if (data[i] == 0)
            {
                strings.emplace_back((const char *)&data[start], i - start);
                start = i + 1;
            }
        }

        return strings;
    }
}

namespace Translator
{
    void init(const std::string &locale)
    {
        language = locale;

        characteristics = readFile("characteristic");
        natures = readFile("natures");
        abilities = readFile("abilities");
        hiddenPowers = readFile("powers");
        species = readFile("species");
    }

    std::vector<std::string> getCharacteristic()
    {
        return characteristics;
    }

    std::vector<std::string> getNatures()
    {
        return natures;
    }

    std::string getNature(u8 nature)
    {
        return natures[nature];
    }

    std::string getAbility(u16 ability)
    {
        return abilities[ability];
    }

    std::vector<std::string> getHiddenPowers()
    {
        return hiddenPowers;
    }

    std::string getHiddenPower(u8 power)
    {
        return hiddenPowers[power];
    }

    std::string getSpecies(u16 specie)
    {
        return species[specie - 1];
    }

    std::vector<std::string> getSpecies(const std::vector<u16> &nums)
    {
        std::vector<std::string> s;
        std::transform(nums.begin(), nums.end(), std::back_inserter(s), [](u16 num) { return species[num - 1]; });
        return s;
    }

    std::string getGender(u8 gender)
    {
        return genders[gender];
    }

    std::vector<std::string> getGenders()
    {
        return genders;
    }

    std::vector<std::string> getLocations(const std::vector<u8> &nums, Game game)
    {
        std::vector<std::string> strings;
        if (game & Game::FRLG)
        {
            strings = readFile("frlg");
        }
        else if (game & Game::RSE)
        {
            strings = readFile("rse");
        }
        else if (game & Game::DPPt)
        {
            strings = readFile("dppt");
        }
        else if (game & Game::HGSS)
        {
            strings = readFile("hgss");
        }
        else
        {
            strings = readFile("swsh");
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
        std::transform(nums.begin(), nums.end(), std::back_inserter(locations), [&map](u8 num) { return map[num]; });

        return locations;
    }

    std::string getKeypress(u8 keypress)
    {
        return buttons[keypress];
    }

    std::string getKeypresses(u16 keypresses)
    {
        if (keypresses == 0)
        {
            return "None";
        }

        std::string result;
        for (int i = 0; i < 12; i++)
        {
            if (keypresses & (1 << i))
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
}
