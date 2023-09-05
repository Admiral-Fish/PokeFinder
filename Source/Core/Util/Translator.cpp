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

#include "Translator.hpp"
#include <Core/Enum/Buttons.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Resources/i18n.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <bit>
#include <iterator>
#include <map>

enum class Language
{
    German,
    English,
    Spanish,
    French,
    Italian,
    Japanese,
    Korean,
    Chinese,
    Count
};

enum class Translation
{
    Ability,
    BDSP,
    BW2,
    BW,
    Characteristic,
    DPPt,
    E,
    Form,
    FRLG,
    Gales,
    Game,
    HGSS,
    Item,
    Move,
    Nature,
    Power,
    RS,
    Specie,
    SwSh,
    Count
};

static Language language;
static std::vector<std::string> abilities;
static std::vector<std::string> characteristics;
static std::map<u16, std::string> forms;
static std::vector<std::string> games;
static std::vector<std::string> hiddenPowers;
static std::map<u16, std::string> items;
static std::vector<std::string> moves;
static std::vector<std::string> natures;
static std::vector<std::string> species;
const static std::array<std::string, 3> genders = { "♂", "♀", "-" };
const static std::array<std::string, 12> buttons = { "R", "L", "X", "Y", "A", "B", "Select", "Start", "Right", "Left", "Up", "Down" };

/**
 * @brief Reads strings from the \p translation in the languaged specified by Translator::init()
 *
 * @param translation String category to read from
 * @param strings Vector to write strings out to
 */
static void readFile(Translation translation, std::vector<std::string> &strings)
{
    int index = (static_cast<int>(language) * static_cast<int>(Translation::Count)) + static_cast<int>(translation);
    u32 start = INDICES[index];
    u32 end = INDICES[index + 1];

    const char *compressedData = reinterpret_cast<const char *>(I18N + start);
    u32 compressedLength = end - start;

    u32 length;
    char *data = Utilities::decompress(compressedData, compressedLength, length);

    for (u32 i = 0; i < length;)
    {
        char *it = std::find(data + i, data + length, 0);
        u32 len = it - &data[i];
        strings.emplace_back(data + i, len);
        i += len + 1;
    }

    delete[] data;
}

/**
 * @brief Reads string mapping from the \p translation in the languaged specified by Translator::init()
 *
 * @param translation String category to read from
 *
 * @return Map of translated strings
 */
static std::map<u16, std::string> readFile(Translation translation)
{
    int index = (static_cast<int>(language) * static_cast<int>(Translation::Count)) + static_cast<int>(translation);
    u32 start = INDICES[index];
    u32 end = INDICES[index + 1];

    const char *compressedData = reinterpret_cast<const char *>(I18N + start);
    u32 compressedLength = end - start;

    u32 length;
    char *data = Utilities::decompress(compressedData, compressedLength, length);

    std::map<u16, std::string> strings;
    for (u32 i = 0; i < length;)
    {
        char *it = std::find(data + i, data + length, 0);
        u32 len = it - &data[i];

        char *word;
        u16 num = std::strtoul(&data[i], &word, 10);
        strings[num] = std::string(word + 1, it - word - 1);

        i += len + 1;
    }

    delete[] data;
    return strings;
}

namespace Translator
{
    const std::string &getAbility(u16 ability)
    {
        return abilities[ability - 1];
    }

    const std::string &getCharacteristic(u8 characteristic)
    {
        return characteristics[characteristic];
    }

    const std::vector<std::string> &getCharacteristics()
    {
        return characteristics;
    }

    const std::string &getForm(u16 specie, u8 form)
    {
        return forms[(form << 11) | specie];
    }

    const std::string &getGame(Game version)
    {
        // Strings are ordered to match the enum
        // Use countr_zero to get the bit that is set
        int index = std::countr_zero(toInt(version));
        if (index == 32)
        {
            return games[games.size() - 1];
        }
        return games[index];
    }

    const std::string &getGender(u8 gender)
    {
        return genders[gender];
    }

    const std::array<std::string, 3> &getGenders()
    {
        return genders;
    }

    const std::string &getHiddenPower(u8 power)
    {
        return hiddenPowers[power];
    }

    const std::vector<std::string> &getHiddenPowers()
    {
        return hiddenPowers;
    }

    const std::string &getItem(u16 item)
    {
        return items[item];
    }

    std::vector<std::string> getItems(const std::vector<u16> &item)
    {
        std::vector<std::string> s;
        s.reserve(item.size());
        std::transform(item.begin(), item.end(), std::back_inserter(s), [](u16 num) { return items[num]; });
        return s;
    }

    const std::string &getKeypress(u8 keypress)
    {
        return buttons[keypress];
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
        Translation translation;
        if ((game & Game::Emerald) != Game::None)
        {
            translation = Translation::E;
        }
        else if ((game & Game::FRLG) != Game::None)
        {
            translation = Translation::FRLG;
        }
        else if ((game & Game::RS) != Game::None)
        {
            translation = Translation::RS;
        }
        else if ((game & Game::Gales) != Game::None)
        {
            translation = Translation::Gales;
        }
        else if ((game & Game::DPPt) != Game::None)
        {
            translation = Translation::DPPt;
        }
        else if ((game & Game::HGSS) != Game::None)
        {
            translation = Translation::HGSS;
        }
        else if ((game & Game::BW) != Game::None)
        {
            translation = Translation::BW;
        }
        else if ((game & Game::BW2) != Game::None)
        {
            translation = Translation::BW2;
        }
        else if ((game & Game::SwSh) != Game::None)
        {
            translation = Translation::SwSh;
        }
        else
        {
            translation = Translation::BDSP;
        }

        std::map<u16, std::string> map = readFile(translation);
        std::vector<std::string> locations;
        locations.reserve(nums.size());
        std::transform(nums.begin(), nums.end(), std::back_inserter(locations), [&map](u16 num) { return map[num]; });
        return locations;
    }

    const std::string &getMove(u16 move)
    {
        return moves[move];
    }

    const std::string &getNature(u8 nature)
    {
        return natures[nature];
    }

    const std::vector<std::string> &getNatures()
    {
        return natures;
    }

    const std::string &getSpecie(u16 specie)
    {
        return species[specie - 1];
    }

    std::string getSpecie(u16 specie, u8 form)
    {
        auto it = forms.find((form << 11) | specie);
        if (it != forms.end())
        {
            return species[specie - 1] + " (" + it->second + ")";
        }
        else
        {
            return species[specie - 1];
        }
    }

    std::vector<std::string> getSpecies(const std::vector<u16> &specie)
    {
        std::vector<std::string> s;
        s.reserve(specie.size());
        std::transform(specie.begin(), specie.end(), std::back_inserter(s), [](u16 num) { return getSpecie(num & 0x7ff, num >> 11); });
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

        readFile(Translation::Ability, abilities);
        readFile(Translation::Characteristic, characteristics);
        forms = readFile(Translation::Form);
        readFile(Translation::Game, games);
        readFile(Translation::Power, hiddenPowers);
        items = readFile(Translation::Item);
        readFile(Translation::Move, moves);
        readFile(Translation::Nature, natures);
        readFile(Translation::Specie, species);
    }
}
