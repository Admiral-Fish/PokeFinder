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

#ifndef METHOD_HPP
#define METHOD_HPP

// Different method types for various games
enum Method
{
    Method1             = 1 << 0,
    Method1Reverse      = 1 << 1,
    Method2             = 1 << 2,
    Method4             = 1 << 3,
    MethodH1            = 1 << 4,
    MethodH2            = 1 << 5,
    MethodH4            = 1 << 6,
    XD                  = 1 << 7, // Used to signify XD shadows
    Colo                = 1 << 8, // Used to signify Colo shadows
    XDColo              = 1 << 9,
    Channel             = 1 << 10,
    EBred               = 1 << 11,
    EBredSplit          = 1 << 12,
    EBredAlternate      = 1 << 13,
    EBredPID            = 1 << 14,
    RSBred              = 1 << 15,
    RSBredAlternate     = 1 << 16,
    FRLGBred            = 1 << 17,
    FRLGBredAlternate   = 1 << 18,
    MethodJ             = 1 << 19,
    MethodK             = 1 << 20,
    ChainedShiny        = 1 << 21,
    WondercardIVs       = 1 << 22,
    Gen4Normal          = 1 << 23,
    Gen4Masuada         = 1 << 24,
    DPPtIVs             = 1 << 25,
    HGSSIVs             = 1 << 26
};

#endif // METHOD_HPP
