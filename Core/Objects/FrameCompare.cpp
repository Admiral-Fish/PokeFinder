/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "FrameCompare.hpp"

FrameCompare::FrameCompare(const QVector<u8> &min, const QVector<u8> &max, int genderIndex, int genderRatioIndex, int abilityIndex,
                           const QVector<bool> &nature, const QVector<bool> &power, bool onlyShiny, bool skipCompare)
{
    this->min = min;
    this->max = max;

    gender = genderIndex;
    genderRatio = genderRatioIndex;
    ability = abilityIndex;

    natures.resize(25);
    for (u8 i = 0; i < 25; i++)
    {
        natures[Nature::getAdjustedNature(i)] = nature.at(i);
    }

    powers = power;

    shiny = onlyShiny;
    skip = skipCompare;
}

FrameCompare::FrameCompare(const QVector<u8> &min, const QVector<u8> &max, int genderIndex, int genderRatioIndex, int abilityIndex,
                           const QVector<bool> &nature, const QVector<bool> &power, bool onlyShiny, bool skipCompare, const QVector<bool> &encounter)
{
    this->min = min;
    this->max = max;

    gender = genderIndex;
    genderRatio = genderRatioIndex;
    ability = abilityIndex;

    natures.resize(25);
    for (u32 i = 0; i < 25; i++)
    {
        natures[Nature::getAdjustedNature(i)] = nature.at(i);
    }

    powers = power;
    encounterSlots = encounter;

    shiny = onlyShiny;
    skip = skipCompare;
}

FrameCompare::FrameCompare(int genderIndex, int genderRatioIndex, int abilityIndex, QVector<bool> nature, bool onlyShiny)
{
    gender = genderIndex;
    genderRatio = genderRatioIndex;
    ability = abilityIndex;

    natures.resize(25);
    for (u32 i = 0; i < 25; i++)
    {
        natures[Nature::getAdjustedNature(i)] = nature[i];
    }

    shiny = onlyShiny;
    skip = false;
}

FrameCompare::FrameCompare(const QVector<u8> &min, const QVector<u8> &max, const QVector<bool> &power)
{
    this->min = min;
    this->max = max;
    powers = power;
}

bool FrameCompare::comparePID(const Frame &frame) const
{
    if (skip)
    {
        return true;
    }

    if (shiny && !frame.getShiny())
    {
        return false;
    }

    if (!natures.at(frame.getNature()))
    {
        return false;
    }

    if (ability != 0 && ability - 1 != frame.getAbility())
    {
        return false;
    }

    if (!compareGender(frame))
    {
        return false;
    }

    return true;
}

bool FrameCompare::compareIVs(const Frame &frame) const
{
    if (skip)
    {
        return true;
    }

    if (!powers.at(frame.getHidden()))
    {
        return false;
    }

    for (int i = 0; i < 6; i++)
    {
        u8 iv = frame.getIV(i);

        if (iv < min[i] || iv > max[i])
        {
            return false;
        }
    }

    return true;
}

bool FrameCompare::compareNature(const Frame &frame) const
{
    return natures.at(frame.getNature());
}

bool FrameCompare::compareHiddenPower(const Frame &frame) const
{
    return powers.at(frame.getHidden());
}

bool FrameCompare::compareSlot(const Frame &frame) const
{
    return encounterSlots.at(frame.getEncounterSlot());
}

bool FrameCompare::compareGender(const Frame &frame) const
{
    switch (genderRatio)
    {
        case 1:
            switch (gender)
            {
                case 1:
                    if (frame.getGender() < 127)
                    {
                        return false;
                    }
                    break;
                case 2:
                    if (frame.getGender() >= 127)
                    {
                        return false;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 2:
            switch (gender)
            {
                case 1:
                    if (frame.getGender() < 191)
                    {
                        return false;
                    }
                    break;
                case 2:
                    if (frame.getGender() >= 191)
                    {
                        return false;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 3:
            switch (gender)
            {
                case 1:
                    if (frame.getGender() < 63)
                    {
                        return false;
                    }
                    break;
                case 2:
                    if (frame.getGender() >= 63)
                    {
                        return false;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 4:
            switch (gender)
            {
                case 1:
                    if (frame.getGender() < 31)
                    {
                        return false;
                    }
                    break;
                case 2:
                    if (frame.getGender() >= 31)
                    {
                        return false;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 5:
            switch (gender)
            {
                case 2:
                    return false;
                default:
                    break;
            }
            break;
        case 6:
            switch (gender)
            {
                case 1:
                    return false;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return true;
}

bool FrameCompare::compareFrame(const Frame &frame) const
{
    if (skip)
    {
        return true;
    }

    if (!comparePID(frame))
    {
        return false;
    }

    if (!compareIVs(frame))
    {
        return false;
    }

    return true;
}

u8 FrameCompare::getGenderRatio() const
{
    return genderRatio;
}
