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

#ifndef FRAMECOMPARE_HPP
#define FRAMECOMPARE_HPP
#include <QString>
#include <QTranslator>
#include <cstdint>
#include <vector>
#include <Forms/mainwindow.h>
#include <Core/Objects/Frame.hpp>

class FrameCompare
{
private:

    int hp[2];
    int atk[2];
    int def[2];
    int spa[2];
    int spd[2];
    int spe[2];

    int gender;
    int genderRatio;
    int ability;
    std::vector<int> natures;
    int hiddenPower;

    bool shiny;

public:
    FrameCompare(int hpEvalIndex, int hpNum, int atkEvalIndex, int atkNum, int defEvalIndex, int defNum, int spaEvalIndex, int spaNum, int spdEvalIndex, int spdNum, int speEvalIndex, int speNum, int genderIndex, int genderRatioIndex, int abilityIndex, std::vector<int> natureIndex, int hiddenPowerIndex, bool onlyShiny);

    bool compareFrame(Frame frame);

};

#endif // FRAMECOMPARE_HPP
