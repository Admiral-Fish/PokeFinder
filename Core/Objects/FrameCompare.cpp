/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

FrameCompare::FrameCompare(int hpEvalIndex, int hpNum, int atkEvalIndex, int atkNum, int defEvalIndex, int defNum, int spaEvalIndex, int spaNum, int spdEvalIndex, int spdNum, int speEvalIndex, int speNum, int genderIndex, int genderRatioIndex, int abilityIndex, QComboBox *natureBox, QComboBox *hiddenPowerBox, bool onlyShiny, bool skipCompare)
{
    hp[0] = (uint)hpEvalIndex;
    hp[1] = (uint)hpNum;
    atk[0] = (uint)atkEvalIndex;
    atk[1] = (uint)atkNum;
    def[0] = (uint)defEvalIndex;
    def[1] = (uint)defNum;
    spa[0] = (uint)spaEvalIndex;
    spa[1] = (uint)spaNum;
    spd[0] = (uint)spdEvalIndex;
    spd[1] = (uint)spdNum;
    spe[0] = (uint)speEvalIndex;
    spe[1] = (uint)speNum;

    gender = (uint)genderIndex;
    genderRatio = (uint)genderRatioIndex;
    ability = (uint)abilityIndex;

    for(uint i = 1; i < 26; i++)
        if(natureBox->model()->data(natureBox->model()->index(i, 0), Qt::CheckStateRole).toBool())
            natures.push_back(Nature::GetAdjustedNature(i - 1));

    for(uint i = 1; i < 17; i++)
        if(hiddenPowerBox->model()->data(hiddenPowerBox->model()->index(i, 0), Qt::CheckStateRole).toBool())
            powers.push_back(i - 1);

    shiny = onlyShiny;
    skip = skipCompare;
}

bool FrameCompare::CompareFramePID(Frame frame)
{
    if (skip)
        return true;

    if(shiny && !frame.shiny)
        return false;

    switch(genderRatio)
    {
        case 1:
            switch(gender)
            {
                case 1:
                    if(frame.gender < 127)
                        return false;

                    break;
                case 2:
                    if(frame.gender >= 127)
                        return false;

                    break;
                default:
                    break;
            }
            break;
        case 2:
            switch(gender)
            {
                case 1:
                    if(frame.gender < 191)
                        return false;

                    break;
                case 2:
                    if(frame.gender >= 191)
                        return false;

                    break;
                default:
                    break;
            }
            break;
        case 3:
            switch(gender)
            {
                case 1:
                    if(frame.gender < 63)
                        return false;

                    break;
                case 2:
                    if(frame.gender >= 63)
                        return false;

                    break;
                default:
                    break;
            }
            break;
        case 4:
            switch(gender)
            {
                case 1:
                    if(frame.gender < 31)
                        return false;

                    break;
                case 2:
                    if(frame.gender >= 31)
                        return false;

                    break;
                default:
                    break;
            }
            break;
        case 5:
            switch(gender)
            {
                case 2:
                    return false;
                default:
                    break;
                }
                break;
        case 6:
            switch(gender)
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

    if(ability != 0 && ability - 1 != frame.ability)
        return false;

    if(natures.size() != 0 && std::find(natures.begin(), natures.end(), frame.nature) == natures.end())
        return false;

    return true;
}

bool FrameCompare::CompareFrameIVs(Frame frame)
{
    if (skip)
        return true;

    if(powers.size() != 0 && std::find(powers.begin(), powers.end(), frame.hidden) == powers.end())
        return false;

    switch(hp[0])
    {
        case 1:
            if(frame.hp != hp[1])
                return false;
            break;
        case 2:
            if(frame.hp < hp[1])
                return false;
            break;
        case 3:
            if(frame.hp > hp[1])
                return false;
            break;
        case 4:
            if(frame.hp == hp[1])
                return false;
            break;
        default:
            break;
    }

    switch(atk[0])
    {
        case 1:
            if(frame.atk != atk[1])
                return false;
            break;
        case 2:
            if(frame.atk < atk[1])
                return false;
            break;
        case 3:
            if(frame.atk > atk[1])
                return false;
            break;
        case 4:
            if(frame.atk == atk[1])
                return false;
            break;
        default:
            break;
    }

    switch(def[0])
    {
        case 1:
            if(frame.def != def[1])
                return false;
            break;
        case 2:
            if(frame.def < def[1])
                return false;
            break;
        case 3:
            if(frame.def > def[1])
                return false;
            break;
        case 4:
            if(frame.def == def[1])
                return false;
            break;
        default:
            break;
    }

    switch(spa[0])
    {
        case 1:
            if(frame.spa != spa[1])
                return false;
            break;
        case 2:
            if(frame.spa < spa[1])
                return false;
            break;
        case 3:
            if(frame.spa > spa[1])
                return false;
            break;
        case 4:
            if(frame.spa == spa[1])
                return false;
            break;
        default:
            break;
    }

    switch(spd[0])
    {
        case 1:
            if(frame.spd != spd[1])
                return false;
            break;
        case 2:
            if(frame.spd < spd[1])
                return false;
            break;
        case 3:
            if(frame.spd > spd[1])
                return false;
            break;
        case 4:
            if(frame.spd == spd[1])
                return false;
            break;
        default:
            break;
    }

    switch(spe[0])
    {
        case 1:
            if(frame.spe != spe[1])
                return false;
            break;
        case 2:
            if(frame.spe < spe[1])
                return false;
            break;
        case 3:
            if(frame.spe > spe[1])
                return false;
            break;
        case 4:
            if(frame.spe == spe[1])
                return false;
            break;
        default:
            break;
    }

    return true;
}
