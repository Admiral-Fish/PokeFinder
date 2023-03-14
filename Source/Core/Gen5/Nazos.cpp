/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Language.hpp>
#include <Core/Gen5/Profile5.hpp>

consteval static u32 changeEndian(u32 val)
{
    val = ((val << 8) & 0xff00ff00) | ((val >> 8) & 0xff00ff);
    return (val << 16) | (val >> 16);
}

consteval static std::array<u32, 5> computeNazoBW(u32 nazo)
{
    constexpr u32 offset1 = 0xfc;
    constexpr u32 offset2 = offset1 + 0x4c;

    std::array<u32, 5> nazos;

    nazos[0] = changeEndian(nazo);
    nazos[1] = nazos[2] = changeEndian(nazo + offset1);
    nazos[3] = nazos[4] = changeEndian(nazo + offset2);

    return nazos;
}

consteval static std::array<u32, 5> computeNazoBW2(u32 nazo, u32 nazo0, u32 nazo1)
{
    constexpr u32 offset = 0x54;

    std::array<u32, 5> nazos;

    nazos[0] = changeEndian(nazo0);
    nazos[1] = changeEndian(nazo1);
    nazos[2] = changeEndian(nazo);
    nazos[3] = nazos[4] = changeEndian(nazo + offset);

    return nazos;
}

// clang-format off
constexpr std::array<u32, 5> englishBlack      = computeNazoBW(0x022160b0);
constexpr std::array<u32, 5> englishWhite      = computeNazoBW(0x022160d0);
constexpr std::array<u32, 5> englishBlackDSi   = computeNazoBW(0x02760190);
constexpr std::array<u32, 5> englishWhiteDSi   = computeNazoBW(0x027601b0);
constexpr std::array<u32, 5> englishBlack2     = computeNazoBW2(0x02200010, 0x0209aee8, 0x02039de9);
constexpr std::array<u32, 5> englishWhite2     = computeNazoBW2(0x02200050, 0x0209af28, 0x02039e15);
constexpr std::array<u32, 5> englishBlack2DSi  = computeNazoBW2(0x027a5f70, 0x0209aee8, 0x02039de9);
constexpr std::array<u32, 5> englishWhite2DSi  = computeNazoBW2(0x027a5e90, 0x0209af28, 0x02039e15);

constexpr std::array<u32, 5> japaneseBlack     = computeNazoBW(0x02215f10);
constexpr std::array<u32, 5> japaneseWhite     = computeNazoBW(0x02215f30);
constexpr std::array<u32, 5> japaneseBlackDSi  = computeNazoBW(0x02761150);
constexpr std::array<u32, 5> japaneseWhiteDSi  = computeNazoBW(0x02761150);
constexpr std::array<u32, 5> japaneseBlack2    = computeNazoBW2(0x021ff9b0, 0x0209a8dc, 0x02039ac9);
constexpr std::array<u32, 5> japaneseWhite2    = computeNazoBW2(0x021ff9d0, 0x0209a8fc, 0x02039af5);
constexpr std::array<u32, 5> japaneseBlack2DSi = computeNazoBW2(0x027aa730, 0x0209a8dc, 0x02039ac9);
constexpr std::array<u32, 5> japaneseWhite2DSi = computeNazoBW2(0x027aa5f0, 0x0209a8fc, 0x02039af5);

constexpr std::array<u32, 5> germanBlack       = computeNazoBW(0x02215ff0);
constexpr std::array<u32, 5> germanWhite       = computeNazoBW(0x02216010);
constexpr std::array<u32, 5> germanBlackDSi    = computeNazoBW(0x027602f0);
constexpr std::array<u32, 5> germanWhiteDSi    = computeNazoBW(0x027602f0);
constexpr std::array<u32, 5> germanBlack2      = computeNazoBW2(0x021fff50, 0x0209ae28, 0x02039d69);
constexpr std::array<u32, 5> germanWhite2      = computeNazoBW2(0x021fff70, 0x0209ae48, 0x02039d95);
constexpr std::array<u32, 5> germanBlack2DSi   = computeNazoBW2(0x027a6110, 0x0209ae28, 0x02039d69);
constexpr std::array<u32, 5> germanWhite2DSi   = computeNazoBW2(0x027a6010, 0x0209ae48, 0x02039d95);

constexpr std::array<u32, 5> spanishBlack      = computeNazoBW(0x02216070);
constexpr std::array<u32, 5> spanishWhite      = computeNazoBW(0x02216070);
constexpr std::array<u32, 5> spanishBlackDSi   = computeNazoBW(0x027601f0);
constexpr std::array<u32, 5> spanishWhiteDSi   = computeNazoBW(0x027601f0);
constexpr std::array<u32, 5> spanishBlack2     = computeNazoBW2(0x021fffd0, 0x0209aea8, 0x02039db9);
constexpr std::array<u32, 5> spanishWhite2     = computeNazoBW2(0x021ffff0, 0x0209aec8, 0x02039de5);
constexpr std::array<u32, 5> spanishBlack2DSi  = computeNazoBW2(0x027a6070, 0x0209aea8, 0x02039db9);
constexpr std::array<u32, 5> spanishWhite2DSi  = computeNazoBW2(0x027a5fb0,  0x0209aec8, 0x02039de5);

constexpr std::array<u32, 5> frenchBlack       = computeNazoBW(0x02216030);
constexpr std::array<u32, 5> frenchWhite       = computeNazoBW(0x02216050);
constexpr std::array<u32, 5> frenchBlackDSi    = computeNazoBW(0x02760230);
constexpr std::array<u32, 5> frenchWhiteDSi    = computeNazoBW(0x02760250);
constexpr std::array<u32, 5> frenchBlack2      = computeNazoBW2(0x02200030, 0x0209af08, 0x02039df9);
constexpr std::array<u32, 5> frenchWhite2      = computeNazoBW2(0x02200050, 0x0209af28, 0x02039e25);
constexpr std::array<u32, 5> frenchBlack2DSi   = computeNazoBW2(0x027a5f90, 0x0209af08, 0x02039df9);
constexpr std::array<u32, 5> frenchWhite2DSi   = computeNazoBW2(0x027a5ef0, 0x0209af28, 0x02039e25);

constexpr std::array<u32, 5> italianBlack      = computeNazoBW(0x02215fb0);
constexpr std::array<u32, 5> italianWhite      = computeNazoBW(0x02215fd0);
constexpr std::array<u32, 5> italianBlackDSi   = computeNazoBW(0x027601d0);
constexpr std::array<u32, 5> italianWhiteDSi   = computeNazoBW(0x027601d0);
constexpr std::array<u32, 5> italianBlack2     = computeNazoBW2(0x021fff10, 0x0209ade8, 0x02039d69);
constexpr std::array<u32, 5> italianWhite2     = computeNazoBW2(0x021fff50, 0x0209ae28, 0x02039d95);
constexpr std::array<u32, 5> italianBlack2DSi  = computeNazoBW2(0x027a5f70, 0x0209ade8, 0x02039d69);
constexpr std::array<u32, 5> italianWhite2DSi  = computeNazoBW2(0x027a5ed0, 0x0209ae28, 0x02039d95);

constexpr std::array<u32, 5> koreanBlack       = computeNazoBW(0x022167b0);
constexpr std::array<u32, 5> koreanWhite       = computeNazoBW(0x022167b0);
constexpr std::array<u32, 5> koreanBlackDSi    = computeNazoBW(0x02761150);
constexpr std::array<u32, 5> koreanWhiteDSi    = computeNazoBW(0x02761150);
constexpr std::array<u32, 5> koreanBlack2      = computeNazoBW2(0x02200750, 0x0209b60c, 0x0203a4d5);
constexpr std::array<u32, 5> koreanWhite2      = computeNazoBW2(0x02200770, 0x0209b62c, 0x0203a501);
constexpr std::array<u32, 5> koreanBlack2DSi   = computeNazoBW2(0x02200770, 0x0209b60c, 0x0203a4d5);
constexpr std::array<u32, 5> koreanWhite2DSi   = computeNazoBW2(0x027a57b0, 0x0209b62c, 0x0203a501);
// clang-format on

std::array<u32, 5> Nazos::getNazo(Game version, Language language, DSType dsType)
{
    switch (language)
    {
    case Language::English:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DS ? englishBlack : englishBlackDSi;
        case Game::White:
            return dsType == DSType::DS ? englishWhite : englishWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DS ? englishBlack2 : englishBlack2DSi;
        case Game::White2:
            return dsType == DSType::DS ? englishWhite2 : englishWhite2DSi;
        default:
            break;
        }
        break;
    case Language::Japanese:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DS ? japaneseBlack : japaneseBlackDSi;
        case Game::White:
            return dsType == DSType::DS ? japaneseWhite : japaneseWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DS ? japaneseBlack2 : japaneseBlack2DSi;
        case Game::White2:
            return dsType == DSType::DS ? japaneseWhite2 : japaneseWhite2DSi;
        default:
            break;
        }
        break;
    case Language::German:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DS ? germanBlack : germanBlackDSi;
        case Game::White:
            return dsType == DSType::DS ? germanWhite : germanWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DS ? germanBlack2 : germanBlack2DSi;
        case Game::White2:
            return dsType == DSType::DS ? germanWhite2 : germanWhite2DSi;
        default:
            break;
        }
        break;
    case Language::Spanish:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DS ? spanishBlack : spanishBlackDSi;
        case Game::White:
            return dsType == DSType::DS ? spanishWhite : spanishWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DS ? spanishBlack2 : spanishBlack2DSi;
        case Game::White2:
            return dsType == DSType::DS ? spanishWhite2 : spanishWhite2DSi;
        default:
            break;
        }
        break;
    case Language::French:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DS ? frenchBlack : frenchBlackDSi;
        case Game::White:
            return dsType == DSType::DS ? frenchWhite : frenchWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DS ? frenchBlack2 : frenchBlack2DSi;
        case Game::White2:
            return dsType == DSType::DS ? frenchWhite2 : frenchWhite2DSi;
        default:
            break;
        }
        break;
    case Language::Italian:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DS ? italianBlack : italianBlackDSi;
        case Game::White:
            return dsType == DSType::DS ? italianWhite : italianWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DS ? italianBlack2 : italianBlack2DSi;
        case Game::White2:
            return dsType == DSType::DS ? italianWhite2 : italianWhite2DSi;
        default:
            break;
        }
        break;
    case Language::Korean:
        switch (version)
        {
        case Game::Black:
            return dsType == DSType::DS ? koreanBlack : koreanBlackDSi;
        case Game::White:
            return dsType == DSType::DS ? koreanWhite : koreanWhiteDSi;
        case Game::Black2:
            return dsType == DSType::DS ? koreanBlack2 : koreanBlack2DSi;
        case Game::White2:
            return dsType == DSType::DS ? koreanWhite2 : koreanWhite2DSi;
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
