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
#include "ui_mainwindow.h"

FrameCompare::FrameCompare(int hpEvalIndex, int hpNum, int atkEvalIndex, int atkNum, int defEvalIndex, int defNum, int spaEvalIndex, int spaNum, int spdEvalIndex, int spdNum, int speEvalIndex, int speNum, int genderIndex, int genderRatioIndex, int abilityIndex, std::vector<int> natureIndicies, int hiddenPowerIndex, bool onlyShiny)
{
    hp[0] = hpEvalIndex;
    hp[1] = hpNum;
    atk[0] = atkEvalIndex;
    atk[1] = atkNum;
    def[0] = defEvalIndex;
    def[1] = defNum;
    spa[0] = spaEvalIndex;
    spa[1] = spaNum;
    spd[0] = spdEvalIndex;
    spd[1] = spdNum;
    spe[0] = speEvalIndex;
    spe[1] = speNum;

    gender = genderIndex;
    genderRatio = genderRatioIndex;
    ability = abilityIndex;
    natures = natureIndicies;
    hiddenPower = hiddenPowerIndex;

    shiny = onlyShiny;

}

bool FrameCompare::compareFrame(Frame frame)
{
    if(frame.shiny != shiny)
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

    if(ability != 0 && ability - 1 != (int)frame.ability)
        return false;

    if(natures.size() != 0 && std::find(natures.begin(), natures.end(), (int)frame.nature) == natures.end())
        return false;

    if(hiddenPower != 0 && hiddenPower - 1 != (int)frame.hidden)
        return false;

    switch(hp[0])
    {
    case 1:
        if((int)frame.hp != hp[1])
            return false;
        break;
    case 2:
        if((int)frame.hp < hp[1])
            return false;
        break;
    case 3:
        if((int)frame.hp > hp[1])
            return false;
        break;
    case 4:
        if((int)frame.hp == hp[1])
            return false;
        break;
    default:
        break;
    }

    switch(atk[0])
    {
    case 1:
        if((int)frame.atk != atk[1])
            return false;
        break;
    case 2:
        if((int)frame.atk < atk[1])
            return false;
        break;
    case 3:
        if((int)frame.atk > atk[1])
            return false;
        break;
    case 4:
        if((int)frame.atk == atk[1])
            return false;
        break;
    default:
        break;
    }

    switch(def[0])
    {
    case 1:
        if((int)frame.def != def[1])
            return false;
        break;
    case 2:
        if((int)frame.def < def[1])
            return false;
        break;
    case 3:
        if((int)frame.def > def[1])
            return false;
        break;
    case 4:
        if((int)frame.def == def[1])
            return false;
        break;
    default:
        break;
    }

    switch(spa[0])
    {
    case 1:
        if((int)frame.spa != spa[1])
            return false;
        break;
    case 2:
        if((int)frame.spa < spa[1])
            return false;
        break;
    case 3:
        if((int)frame.spa > spa[1])
            return false;
        break;
    case 4:
        if((int)frame.spa == spa[1])
            return false;
        break;
    default:
        break;
    }

    switch(spd[0])
    {
    case 1:
        if((int)frame.spd != spd[1])
            return false;
        break;
    case 2:
        if((int)frame.spd < spd[1])
            return false;
        break;
    case 3:
        if((int)frame.spd > spd[1])
            return false;
        break;
    case 4:
        if((int)frame.spd == spd[1])
            return false;
        break;
    default:
        break;
    }

    switch(spe[0])
    {
    case 1:
        if((int)frame.spe != spe[1])
            return false;
        break;
    case 2:
        if((int)frame.spe < spe[1])
            return false;
        break;
    case 3:
        if((int)frame.spe > spe[1])
            return false;
        break;
    case 4:
        if((int)frame.spe == spe[1])
            return false;
        break;
    default:
        break;
    }

    return true;
}
