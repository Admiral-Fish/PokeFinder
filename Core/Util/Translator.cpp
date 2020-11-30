/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Resources/I18n.hpp>
#include <algorithm>
#include <map>
#include <sstream>

namespace
{
    std::string language;
    std::vector<std::string> characteristics;
    std::vector<std::string> natures;
    std::vector<std::string> hiddenPowers;
    std::vector<std::string> species;
    const std::vector<std::string> genders = { "♂", "♀", "-" };
    const std::vector<std::string> buttons = { "R", "L", "X", "Y", "A", "B", "Select", "Start", "Right", "Left", "Up", "Down" };

    std::vector<std::string> readFile(const std::string &name)
    {
        if (language == "de")
        {
            if (name == "characteristic")
            {
                return characteristic_de;
            }
            else if (name == "natures")
            {
                return natures_de;
            }
            else if (name == "powers")
            {
                return powers_de;
            }
            else if (name == "species")
            {
                return species_de;
            }
            else if (name == "frlg")
            {
                return frlg_de;
            }
            else if (name == "rse")
            {
                return rse_de;
            }
            else if (name == "dppt")
            {
                return dppt_de;
            }
            else if (name == "hgss")
            {
                return hgss_de;
            }
        }
        else if (language == "en")
        {
            if (name == "characteristic")
            {
                return characteristic_en;
            }
            else if (name == "natures")
            {
                return natures_en;
            }
            else if (name == "powers")
            {
                return powers_en;
            }
            else if (name == "species")
            {
                return species_en;
            }
            else if (name == "frlg")
            {
                return frlg_en;
            }
            else if (name == "rse")
            {
                return rse_en;
            }
            else if (name == "dppt")
            {
                return dppt_en;
            }
            else if (name == "hgss")
            {
                return hgss_en;
            }
        }
        else if (language == "es")
        {
            if (name == "characteristic")
            {
                return characteristic_en;
            }
            else if (name == "natures")
            {
                return natures_en;
            }
            else if (name == "powers")
            {
                return powers_en;
            }
            else if (name == "species")
            {
                return species_en;
            }
            else if (name == "frlg")
            {
                return frlg_en;
            }
            else if (name == "rse")
            {
                return rse_en;
            }
            else if (name == "dppt")
            {
                return dppt_en;
            }
            else if (name == "hgss")
            {
                return hgss_en;
            }
        }
        else if (language == "fr")
        {
            if (name == "characteristic")
            {
                return characteristic_fr;
            }
            else if (name == "natures")
            {
                return natures_fr;
            }
            else if (name == "powers")
            {
                return powers_fr;
            }
            else if (name == "species")
            {
                return species_fr;
            }
            else if (name == "frlg")
            {
                return frlg_fr;
            }
            else if (name == "rse")
            {
                return rse_fr;
            }
            else if (name == "dppt")
            {
                return dppt_fr;
            }
            else if (name == "hgss")
            {
                return hgss_fr;
            }
        }
        else if (language == "it")
        {
            if (name == "characteristic")
            {
                return characteristic_it;
            }
            else if (name == "natures")
            {
                return natures_it;
            }
            else if (name == "powers")
            {
                return powers_it;
            }
            else if (name == "species")
            {
                return species_it;
            }
            else if (name == "frlg")
            {
                return frlg_it;
            }
            else if (name == "rse")
            {
                return rse_it;
            }
            else if (name == "dppt")
            {
                return dppt_it;
            }
            else if (name == "hgss")
            {
                return hgss_it;
            }
        }
        else if (language == "ja")
        {
            if (name == "characteristic")
            {
                return characteristic_ja;
            }
            else if (name == "natures")
            {
                return natures_ja;
            }
            else if (name == "powers")
            {
                return powers_ja;
            }
            else if (name == "species")
            {
                return species_ja;
            }
            else if (name == "frlg")
            {
                return frlg_ja;
            }
            else if (name == "rse")
            {
                return rse_ja;
            }
            else if (name == "dppt")
            {
                return dppt_ja;
            }
            else if (name == "hgss")
            {
                return hgss_ja;
            }
        }
        else if (language == "ko")
        {
            if (name == "characteristic")
            {
                return characteristic_ko;
            }
            else if (name == "natures")
            {
                return natures_ko;
            }
            else if (name == "powers")
            {
                return powers_ko;
            }
            else if (name == "species")
            {
                return species_ko;
            }
            else if (name == "frlg")
            {
                return frlg_ko;
            }
            else if (name == "rse")
            {
                return rse_ko;
            }
            else if (name == "dppt")
            {
                return dppt_ko;
            }
            else if (name == "hgss")
            {
                return hgss_ko;
            }
        }
        else if (language == "zh")
        {
            if (name == "characteristic")
            {
                return characteristic_zh;
            }
            else if (name == "natures")
            {
                return natures_zh;
            }
            else if (name == "powers")
            {
                return powers_zh;
            }
            else if (name == "species")
            {
                return species_zh;
            }
            else if (name == "frlg")
            {
                return frlg_zh;
            }
            else if (name == "rse")
            {
                return rse_zh;
            }
            else if (name == "dppt")
            {
                return dppt_zh;
            }
            else if (name == "hgss")
            {
                return hgss_zh;
            }
        }

        return std::vector<std::string>();
    }
}

namespace Translator
{
    void init(const std::string &locale)
    {
        language = locale;

        characteristics = readFile("characteristic");
        natures = readFile("natures");
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
        else
        {
            strings = readFile("hgss");
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

        std::string result = "";

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
