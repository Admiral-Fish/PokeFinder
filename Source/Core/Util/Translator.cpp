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
    Characteristic,
    Nature,
    Ability,
    Power,
    Specie,
    E,
    FRLG,
    RS,
    DPPt,
    HGSS,
    BW,
    BW2,
    SwSh,
    BDSP
};

const static u8 *datas[8][14]
    = { { characteristic_de.data(), natures_de.data(), abilities_de.data(), powers_de.data(), species_de.data(), e_de.data(),
          frlg_de.data(), rs_de.data(), dppt_de.data(), hgss_de.data(), bw_de.data(), bw2_de.data(), swsh_de.data(), bdsp_de.data() },
        { characteristic_en.data(), natures_en.data(), abilities_en.data(), powers_en.data(), species_en.data(), e_en.data(),
          frlg_en.data(), rs_en.data(), dppt_en.data(), hgss_en.data(), bw_en.data(), bw2_en.data(), swsh_en.data(), bdsp_en.data() },
        { characteristic_es.data(), natures_es.data(), abilities_es.data(), powers_es.data(), species_es.data(), e_es.data(),
          frlg_es.data(), rs_es.data(), dppt_es.data(), hgss_es.data(), bw_es.data(), bw2_es.data(), swsh_es.data(), bdsp_es.data() },
        { characteristic_fr.data(), natures_fr.data(), abilities_fr.data(), powers_fr.data(), species_fr.data(), e_fr.data(),
          frlg_fr.data(), rs_fr.data(), dppt_fr.data(), hgss_fr.data(), bw_fr.data(), bw2_fr.data(), swsh_fr.data(), bdsp_fr.data() },
        { characteristic_it.data(), natures_it.data(), abilities_it.data(), powers_it.data(), species_it.data(), e_it.data(),
          frlg_it.data(), rs_it.data(), dppt_it.data(), hgss_it.data(), bw_it.data(), bw2_it.data(), swsh_it.data(), bdsp_it.data() },
        { characteristic_ja.data(), natures_ja.data(), abilities_ja.data(), powers_ja.data(), species_ja.data(), e_ja.data(),
          frlg_ja.data(), rs_ja.data(), dppt_ja.data(), hgss_ja.data(), bw_ja.data(), bw2_ja.data(), swsh_ja.data(), bdsp_ja.data() },
        { characteristic_ko.data(), natures_ko.data(), abilities_ko.data(), powers_ko.data(), species_ko.data(), e_ko.data(),
          frlg_ko.data(), rs_ko.data(), dppt_ko.data(), hgss_ko.data(), bw_ko.data(), bw2_ko.data(), swsh_ko.data(), bdsp_ko.data() },
        { characteristic_zh.data(), natures_zh.data(), abilities_zh.data(), powers_zh.data(), species_zh.data(), e_zh.data(),
          frlg_zh.data(), rs_zh.data(), dppt_zh.data(), hgss_zh.data(), bw_zh.data(), bw2_zh.data(), swsh_zh.data(), bdsp_zh.data() } };

constexpr size_t sizes[8][14]
    = { { characteristic_de.size(), natures_de.size(), abilities_de.size(), powers_de.size(), species_de.size(), e_de.size(),
          frlg_de.size(), rs_de.size(), dppt_de.size(), hgss_de.size(), bw_de.size(), bw2_de.size(), swsh_de.size(), bdsp_de.size() },
        { characteristic_en.size(), natures_en.size(), abilities_en.size(), powers_en.size(), species_en.size(), e_en.size(),
          frlg_en.size(), rs_en.size(), dppt_en.size(), hgss_en.size(), bw_en.size(), bw2_en.size(), swsh_en.size(), bdsp_en.size() },
        { characteristic_es.size(), natures_es.size(), abilities_es.size(), powers_es.size(), species_es.size(), e_es.size(),
          frlg_es.size(), rs_es.size(), dppt_es.size(), hgss_es.size(), bw_es.size(), bw2_es.size(), swsh_es.size(), bdsp_es.size() },
        { characteristic_fr.size(), natures_fr.size(), abilities_fr.size(), powers_fr.size(), species_fr.size(), e_fr.size(),
          frlg_fr.size(), rs_fr.size(), dppt_fr.size(), hgss_fr.size(), bw_fr.size(), bw2_fr.size(), swsh_fr.size(), bdsp_fr.size() },
        { characteristic_it.size(), natures_it.size(), abilities_it.size(), powers_it.size(), species_it.size(), e_it.size(),
          frlg_it.size(), rs_it.size(), dppt_it.size(), hgss_it.size(), bw_it.size(), bw2_it.size(), swsh_it.size(), bdsp_it.size() },
        { characteristic_ja.size(), natures_ja.size(), abilities_ja.size(), powers_ja.size(), species_ja.size(), e_ja.size(),
          frlg_ja.size(), rs_ja.size(), dppt_ja.size(), hgss_ja.size(), bw_ja.size(), bw2_ja.size(), swsh_ja.size(), bdsp_ja.size() },
        { characteristic_ko.size(), natures_ko.size(), abilities_ko.size(), powers_ko.size(), species_ko.size(), e_ko.size(),
          frlg_ko.size(), rs_ko.size(), dppt_ko.size(), hgss_ko.size(), bw_ko.size(), bw2_ko.size(), swsh_ko.size(), bdsp_ko.size() },
        { characteristic_zh.size(), natures_zh.size(), abilities_zh.size(), powers_zh.size(), species_zh.size(), e_zh.size(),
          frlg_zh.size(), rs_zh.size(), dppt_zh.size(), hgss_zh.size(), bw_zh.size(), bw2_zh.size(), swsh_zh.size(), bdsp_zh.size() } };

namespace
{
    Language language;
    std::vector<std::string> characteristics;
    std::vector<std::string> natures;
    std::vector<std::string> abilities;
    std::vector<std::string> hiddenPowers;
    std::vector<std::string> species;
    std::vector<std::string> genders = { "♂", "♀", "-" };
    std::vector<std::string> buttons = { "R", "L", "X", "Y", "A", "B", "Select", "Start", "Right", "Left", "Up", "Down" };

    std::vector<std::string> readFile(Translation translation)
    {
        const u8 *data = datas[static_cast<int>(language)][static_cast<int>(translation)];
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

        characteristics = readFile(Translation::Characteristic);
        natures = readFile(Translation::Nature);
        abilities = readFile(Translation::Ability);
        hiddenPowers = readFile(Translation::Power);
        species = readFile(Translation::Specie);
    }

    std::vector<std::string> *getCharacteristic()
    {
        return &characteristics;
    }

    std::string *getCharacteristic(u8 characteristic)
    {
        return &characteristics[characteristic];
    }

    std::vector<std::string> *getNatures()
    {
        return &natures;
    }

    std::string *getNature(u8 nature)
    {
        return &natures[nature];
    }

    std::string *getAbility(u16 ability)
    {
        return &abilities[ability];
    }

    std::vector<std::string> *getHiddenPowers()
    {
        return &hiddenPowers;
    }

    std::string *getHiddenPower(u8 power)
    {
        return &hiddenPowers[power];
    }

    std::string *getSpecies(u16 specie)
    {
        return &species[specie - 1];
    }

    std::vector<std::string> getSpecies(const std::vector<u16> &nums)
    {
        std::vector<std::string> s;
        std::transform(nums.begin(), nums.end(), std::back_inserter(s), [](u16 num) { return species[num - 1]; });
        return s;
    }

    std::string *getGender(u8 gender)
    {
        return &genders[gender];
    }

    std::vector<std::string> *getGenders()
    {
        return &genders;
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

    std::string *getKeypress(u8 keypress)
    {
        return &buttons[keypress];
    }

    std::string getKeypresses(Buttons keypresses)
    {
        if (toInt(keypresses) == 0)
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
}
