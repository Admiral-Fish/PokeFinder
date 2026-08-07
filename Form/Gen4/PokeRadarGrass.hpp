/*
 * This file is part of Pok?Finder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 */

#ifndef POKERADARGRASS_HPP
#define POKERADARGRASS_HPP

#include <Core/Enum/Game.hpp>
#include <Core/Global.hpp>
#include <QPoint>
#include <QSize>
#include <vector>

QSize getPokeRadarGrassFieldSize(u16 location, u8 occurrence, Game version);
std::vector<bool> getPokeRadarGrassField(u16 location, u8 occurrence, Game version);
QPoint getPokeRadarGrassFieldStart(u16 location, u8 occurrence, Game version);

#endif // POKERADARGRASS_HPP
