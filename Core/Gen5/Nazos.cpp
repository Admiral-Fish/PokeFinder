/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later Game::
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

#include "Nazos.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Profile5.hpp>

constexpr std::array<u32, 5> englishBlack = { 0xB0602102, 0xAC612102, 0xAC612102, 0xF8612102, 0xF8612102 };
constexpr std::array<u32, 5> englishBlackDSi = { 0x90017602, 0x8C027602, 0x8C027602, 0xD8027602, 0xD8027602 };
constexpr std::array<u32, 5> englishWhite = { 0xD0602102, 0xCC612102, 0xCC612102, 0x18622102, 0x18622102 };
constexpr std::array<u32, 5> englishWhiteDSi = { 0xB0017602, 0xAC027602, 0xAC027602, 0xF8027602, 0xF8027602 };
constexpr std::array<u32, 5> englishBlack2 = { 0xE8AE0902, 0xE99D0302, 0x10002002, 0x64002002, 0x64002002 };
constexpr std::array<u32, 5> englishBlack2DSi = { 0xE8AE0902, 0xE99D0302, 0x705F7A02, 0xC45F7A02, 0xC45F7A02 };
constexpr std::array<u32, 5> englishWhite2 = { 0x28AF0902, 0x159E0302, 0x50002002, 0xA4002002, 0xA4002002 };
constexpr std::array<u32, 5> englishWhite2DSi = { 0x28AF0902, 0x159E0302, 0x905E7A02, 0xE45E7A02, 0xE45E7A02 };

constexpr std::array<u32, 5> japaneseBlack = { 0x105F2102, 0x0C602102, 0x0C602102, 0x58602102, 0x58602102 };
constexpr std::array<u32, 5> japaneseBlackDSi = { 0x50117602, 0x4C127602, 0x4C127602, 0x98127602, 0x98127602 };
constexpr std::array<u32, 5> japaneseWhite = { 0x305F2102, 0x2C602102, 0x2C602102, 0x78602102, 0x78602102 };
constexpr std::array<u32, 5> japaneseWhiteDSi = { 0x50117602, 0x4C127602, 0x4C127602, 0x98127602, 0x98127602 };
constexpr std::array<u32, 5> japaneseBlack2 = { 0xDCA80902, 0xC99A0302, 0xB0F91F02, 0x04FA1F02, 0x04FA1F02 };
constexpr std::array<u32, 5> japaneseBlack2DSi = { 0xDCA80902, 0xC99A0302, 0x30A77A02, 0x84A77A02, 0x84A77A02 };
constexpr std::array<u32, 5> japaneseWhite2 = { 0xFCA80902, 0xF59A0302, 0xD0F91F02, 0x24FA1F02, 0x24FA1F02 };
constexpr std::array<u32, 5> japaneseWhite2DSi = { 0xFCA80902, 0xF59A0302, 0xF0A57A02, 0x44A67A02, 0x44A67A02 };

constexpr std::array<u32, 5> germanBlack = { 0xF05F2102, 0xEC602102, 0xEC602102, 0x38612102, 0x38612102 };
constexpr std::array<u32, 5> germanBlackDSi = { 0xF0027602, 0xEC037602, 0xEC037602, 0x38047602, 0x38047602 };
constexpr std::array<u32, 5> germanWhite = { 0x10602102, 0x0C612102, 0x0C612102, 0x58612102, 0x58612102 };
constexpr std::array<u32, 5> germanWhiteDSi = { 0xF0027602, 0xEC037602, 0xEC037602, 0x38047602, 0x38047602 };
constexpr std::array<u32, 5> germanBlack2 = { 0x28AE0902, 0x699D0302, 0x50FF1F02, 0xA4FF1F02, 0xA4FF1F02 };
constexpr std::array<u32, 5> germanBlack2DSi = { 0x28AE0902, 0x699D0302, 0x10617A02, 0x64617A02, 0x64617A02 };
constexpr std::array<u32, 5> germanWhite2 = { 0x48AE0902, 0x959D0302, 0x70FF1F02, 0xC4FF1F02, 0xC4FF1F02 };
constexpr std::array<u32, 5> germanWhite2DSi = { 0x48AE0902, 0x959D0302, 0x10607A02, 0x64607A02, 0x64607A02 };

constexpr std::array<u32, 5> spanishBlack = { 0x50602102, 0x4C612102, 0x4C612102, 0x98612102, 0x98612102 };
constexpr std::array<u32, 5> spanishBlackDSi = { 0xF0017602, 0xEC027602, 0xEC027602, 0x38037602, 0x38037602 };
constexpr std::array<u32, 5> spanishWhite = { 0x70602102, 0x6C612102, 0x6C612102, 0xB8612102, 0xB8612102 };
constexpr std::array<u32, 5> spanishWhiteDSi = { 0xF0017602, 0xEC027602, 0xEC027602, 0x38037602, 0x38037602 };
constexpr std::array<u32, 5> spanishBlack2 = { 0xA8AE0902, 0xB99D0302, 0xD0FF1F02, 0x24002002, 0x24002002 };
constexpr std::array<u32, 5> spanishBlack2DSi = { 0xA8AE0902, 0xB99D0302, 0x70607A02, 0xC4607A02, 0xC4607A02 };
constexpr std::array<u32, 5> spanishWhite2 = { 0xC8AE0902, 0xE59D0302, 0xF0FF1F02, 0x44002002, 0x44002002 };
constexpr std::array<u32, 5> spanishWhite2DSi = { 0xC8AE0902, 0xE59D0302, 0xB05F7A02, 0x04607A02, 0x04607A02 };

constexpr std::array<u32, 5> frenchBlack = { 0x30602102, 0x2C612102, 0x2C612102, 0x78612102, 0x78612102 };
constexpr std::array<u32, 5> frenchBlackDSi = { 0x30027602, 0x2C037602, 0x2C037602, 0x78037602, 0x78037602 };
constexpr std::array<u32, 5> frenchWhite = { 0x50602102, 0x4C612102, 0x4C612102, 0x98612102, 0x98612102 };
constexpr std::array<u32, 5> frenchWhiteDSi = { 0x50027602, 0x4C037602, 0x4C037602, 0x98037602, 0x98037602 };
constexpr std::array<u32, 5> frenchBlack2 = { 0x08AF0902, 0xF99D0302, 0x30002002, 0x84002002, 0x84002002 };
constexpr std::array<u32, 5> frenchBlack2DSi = { 0x08AF0902, 0xF99D0302, 0x905F7A02, 0xE45F7A02, 0xE45F7A02 };
constexpr std::array<u32, 5> frenchWhite2 = { 0x28AF0902, 0x259E0302, 0x50002002, 0xA4002002, 0xA4002002 };
constexpr std::array<u32, 5> frenchWhite2DSi = { 0x28AF0902, 0x259E0302, 0xF05E7A02, 0x445F7A02, 0x445F7A02 };

constexpr std::array<u32, 5> italianBlack = { 0xB05F2102, 0xAC602102, 0xAC602102, 0xF8602102, 0xF8602102 };
constexpr std::array<u32, 5> italianBlackDSi = { 0xD0017602, 0xCC027602, 0xCC027602, 0x18037602, 0x18037602 };
constexpr std::array<u32, 5> italianWhite = { 0xD05F2102, 0xCC602102, 0xCC602102, 0x18612102, 0x18612102 };
constexpr std::array<u32, 5> italianWhiteDSi = { 0xD0017602, 0xCC027602, 0xCC027602, 0x18037602, 0x18037602 };
constexpr std::array<u32, 5> italianBlack2 = { 0xE8AD0902, 0x699D0302, 0x10FF1F02, 0x64FF1F02, 0x64FF1F02 };
constexpr std::array<u32, 5> italianBlack2DSi = { 0xE8AD0902, 0x699D0302, 0x705F7A02, 0xC45F7A02, 0xC45F7A02 };
constexpr std::array<u32, 5> italianWhite2 = { 0x28AE0902, 0x959D0302, 0x50FF1F02, 0xA4FF1F02, 0xA4FF1F02 };
constexpr std::array<u32, 5> italianWhite2DSi = { 0x28AE0902, 0x959D0302, 0xD05E7A02, 0x245F7A02, 0x245F7A02 };

constexpr std::array<u32, 5> koreanBlack = { 0xB0672102, 0xAC682102, 0xAC682102, 0xF8682102, 0xF8682102 };
constexpr std::array<u32, 5> koreanBlackDSi = { 0x50117602, 0x4C127602, 0x4C127602, 0x98127602, 0x98127602 };
constexpr std::array<u32, 5> koreanWhite = { 0xB0672102, 0xAC682102, 0xAC682102, 0xF8682102, 0xF8682102 };
constexpr std::array<u32, 5> koreanWhiteDSi = { 0x50117602, 0x4C127602, 0x4C127602, 0x98127602, 0x9812760 };
constexpr std::array<u32, 5> koreanWhite2 = { 0x2CB60902, 0x01A50302, 0x70072002, 0xC4072002, 0xC4072002 };
constexpr std::array<u32, 5> koreanWhite2DSi = { 0x2CB60902, 0x01A50302, 0xB0577A02, 0x04587A02, 0x04587A02 };
constexpr std::array<u32, 5> koreanBlack2 = { 0x0CB60902, 0xD5A40302, 0x50072002, 0xA4072002, 0xA4072002 };
constexpr std::array<u32, 5> koreanBlack2DSi = { 0x0CB60902, 0xD5A40302, 0x70072002, 0xC4072002, 0xC4072002 };

std::array<u32, 5> Nazos::getNazo(Game version, Language language, DSType dsType)
{
    switch (language)
    {
    case Language::English:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DSOriginal ? englishBlack : englishBlackDSi;
        case Game::White:
            return dsType == DSType::DSOriginal ? englishWhite : englishWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DSOriginal ? englishBlack2 : englishBlack2DSi;
        case Game::White2:
            return dsType == DSType::DSOriginal ? englishWhite2 : englishWhite2DSi;
        default:
            break;
        }
        break;
    case Language::Japanese:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DSOriginal ? japaneseBlack : japaneseBlackDSi;
        case Game::White:
            return dsType == DSType::DSOriginal ? japaneseWhite : japaneseWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DSOriginal ? japaneseBlack2 : japaneseBlack2DSi;
        case Game::White2:
            return dsType == DSType::DSOriginal ? japaneseWhite2 : japaneseWhite2DSi;
        default:
            break;
        }
        break;
    case Language::German:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DSOriginal ? germanBlack : germanBlackDSi;
        case Game::White:
            return dsType == DSType::DSOriginal ? germanWhite : germanWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DSOriginal ? germanBlack2 : germanBlack2DSi;
        case Game::White2:
            return dsType == DSType::DSOriginal ? germanWhite2 : germanWhite2DSi;
        default:
            break;
        }
        break;
    case Language::Spanish:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DSOriginal ? spanishBlack : spanishBlackDSi;
        case Game::White:
            return dsType == DSType::DSOriginal ? spanishWhite : spanishWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DSOriginal ? spanishBlack2 : spanishBlack2DSi;
        case Game::White2:
            return dsType == DSType::DSOriginal ? spanishWhite2 : spanishWhite2DSi;
        default:
            break;
        }
        break;
    case Language::French:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DSOriginal ? frenchBlack : frenchBlackDSi;
        case Game::White:
            return dsType == DSType::DSOriginal ? frenchWhite : frenchWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DSOriginal ? frenchBlack2 : frenchBlack2DSi;
        case Game::White2:
            return dsType == DSType::DSOriginal ? frenchWhite2 : frenchWhite2DSi;
        default:
            break;
        }
        break;
    case Language::Italian:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DSOriginal ? italianBlack : italianBlackDSi;
        case Game::White:
            return dsType == DSType::DSOriginal ? italianWhite : italianWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DSOriginal ? italianBlack2 : italianBlack2DSi;
        case Game::White2:
            return dsType == DSType::DSOriginal ? italianWhite2 : italianWhite2DSi;
        default:
            break;
        }
        break;
    case Language::Korean:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DSOriginal ? koreanBlack : koreanBlackDSi;
        case Game::White:
            return dsType == DSType::DSOriginal ? koreanWhite : koreanWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DSOriginal ? koreanBlack2 : koreanBlack2DSi;
        case Game::White2:
            return dsType == DSType::DSOriginal ? koreanWhite2 : koreanWhite2DSi;
        default:
            break;
        }
        break;
    }

    return std::array<u32, 5>();
}

std::array<u32, 5> Nazos::getNazo(const Profile5 &profile)
{
    return getNazo(profile.getVersion(), profile.getLanguage(), profile.getDSType());
}
