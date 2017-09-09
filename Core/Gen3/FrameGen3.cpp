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

#include "FrameGen3.hpp"

FrameGen3::FrameGen3(uint32_t tid, uint32_t sid, uint32_t psv)
{
    this->tid = tid;
    this->sid = sid;
    this->psv = psv;
}

// Sets IVs for Channel method and calculates characteristics based on IVs
void FrameGen3::setIVsChannel(uint32_t iv1, uint32_t iv2, uint32_t iv3, uint32_t iv4, uint32_t iv5, uint32_t iv6)
{
    hp = iv1;
    atk = iv2;
    def = iv3;
    spa = iv4;
    spd = iv5;
    spe = iv6;
    hidden = ((((hp & 1) + 2 * (atk & 1) + 4 * (def & 1) + 8 * (spe & 1) + 16 * (spa & 1) + 32 * (spd & 1)) * 15) / 63);
    power = (30 + ((((hp >> 1) & 1) + 2 * ((atk >> 1) & 1) + 4 * ((def >> 1) & 1) + 8 * ((spe >> 1) & 1) + 16 * ((spa >> 1) & 1) + 32 * ((spd >> 1) & 1)) * 40 / 63));
}

// Change the tid/sid (mostly used for Channel)
void FrameGen3::setIDs(uint32_t tid, uint32_t sid)
{
    this->tid = tid;
    this->sid = sid;
    psv = tid ^ sid;
}

// Returns real time for a given frame
std::string FrameGen3::getTime()
{
    int32_t minutes = frame / 3600;
    int32_t seconds = (frame - (3600 * minutes)) / 60;
    int32_t milliseconds = ((frame % 60) * 100) / 60;
    return "temp";
    //return (boost::format("%d:%02d.%02d") % minutes % seconds % milliseconds).str();
}

