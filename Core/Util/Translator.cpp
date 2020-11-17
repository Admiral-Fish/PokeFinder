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
#include <fstream>
#include <map>
#include <sstream>

#include <QDebug>

namespace
{
    std::string language;
    std::string folder;
    std::vector<std::string> characteristics;
    std::vector<std::string> natures;
    std::vector<std::string> hiddenPowers;
    std::vector<std::string> species;
    const std::vector<std::string> genders = { "♂", "♀", "-" };
    const std::vector<std::string> buttons = { "R", "L", "X", "Y", "A", "B", "Select", "Start", "Right", "Left", "Up", "Down" };

    std::vector<std::string> readFile(const std::string &name)
    {
        std::string path = folder + "/i18n/" + language + "/" + name + language + ".txt";
        std::ifstream file(path);

        std::vector<std::string> input;
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                input.emplace_back(line);
            }
        }
        return input;
    }
}

namespace Translator
{

    void init(const std::string &locale, const std::string &location)
    {
        language = locale;
        folder = location;

        characteristics = readFile("characteristic_");
        natures = readFile("natures_");
        hiddenPowers = readFile("powers_");
        species = readFile("species_");
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
        return natures.at(nature);
    }

    std::vector<std::string> getHiddenPowers()
    {
        return hiddenPowers;
    }

    std::string getHiddenPower(u8 power)
    {
        return hiddenPowers.at(power);
    }

    std::string getSpecies(u16 specie)
    {
        return species.at(specie - 1);
    }

    std::vector<std::string> getSpecies(const std::vector<u16> &nums)
    {
        std::vector<std::string> s;
        for (u16 num : nums)
        {
            s.emplace_back(species.at(num - 1));
        }
        return s;
    }

    std::string getGender(u8 gender)
    {
        return genders.at(gender);
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
            strings = readFile("frlg_");
        }
        else if (game & Game::RSE)
        {
            strings = readFile("rse_");
        }
        else if (game & Game::DPPt)
        {
            strings = readFile("dppt_");
        }
        else
        {
            strings = readFile("hgss_");
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

            map.emplace(std::stoi(entry.at(0)), entry.at(1));
        }

        std::vector<std::string> locations;
        for (const u8 num : nums)
        {
            locations.emplace_back(map[num]);
        }

        return locations;
    }

    std::string getKeypress(u8 keypress)
    {
        return buttons.at(keypress);
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

                result += buttons.at(i);
            }
        }

        return result;
    }
}
