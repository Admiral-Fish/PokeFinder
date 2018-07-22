/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "ResearcherFrame.hpp"

ResearcherFrame::ResearcherFrame()
{
    for (int i = 0; i < 10; i++)
        custom[i] = 0;

    rng64Bit = false;
    full32 = 0;
    full64 = 0;
    frame = 100000;
}

ResearcherFrame::ResearcherFrame(bool rng64Bit, u32 frame)
{
    for (int i = 0; i < 10; i++)
        custom[i] = 0;

    this->rng64Bit = rng64Bit;
    full32 = 0;
    full64 = 0;
    this->frame = frame;
}
