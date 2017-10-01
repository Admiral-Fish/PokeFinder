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

#include "Nature.hpp"

std::vector<QString> Nature::GetNatures()
{
    std::vector<QString> natures = { QObject::tr("Hardy"), QObject::tr("Lonely"), QObject::tr("Brave"), QObject::tr("Adamant"),
                                     QObject::tr("Naughty"), QObject::tr("Bold"), QObject::tr("Docile"), QObject::tr("Relaxed"),
                                     QObject::tr("Impish"), QObject::tr("Lax"), QObject::tr("Timid"), QObject::tr("Hasty"),
                                     QObject::tr("Serious"), QObject::tr("Jolly"), QObject::tr("Naive"), QObject::tr("Modest"),
                                     QObject::tr("Mild"), QObject::tr("Quiet"), QObject::tr("Bashful"), QObject::tr("Rash"),
                                     QObject::tr("Calm"), QObject::tr("Gentle"), QObject::tr("Sassy"), QObject::tr("Careful"),
                                     QObject::tr("Quirky") };
    return natures;
}
