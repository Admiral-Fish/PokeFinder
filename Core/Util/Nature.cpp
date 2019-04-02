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

#include "Nature.hpp"

QStringList Nature::getNatures()
{
    const QStringList natures =
    {
        QObject::tr("Adamant"), QObject::tr("Bold"), QObject::tr("Brave"), QObject::tr("Calm"),
        QObject::tr("Careful"), QObject::tr("Hasty"), QObject::tr("Impish"), QObject::tr("Jolly"),
        QObject::tr("Lonely"), QObject::tr("Mild"), QObject::tr("Modest"), QObject::tr("Naive"),
        QObject::tr("Naughty"), QObject::tr("Quiet"), QObject::tr("Rash"), QObject::tr("Relaxed"),
        QObject::tr("Sassy"), QObject::tr("Timid"), QObject::tr("Gentle"), QObject::tr("Lax"),
        QObject::tr("Bashful"), QObject::tr("Docile"), QObject::tr("Hardy"), QObject::tr("Quirky"),
        QObject::tr("Serious")
    };
    return natures;
}

QString Nature::getNature(u8 nature)
{
    return getFrameNatures().at(nature);
}

QStringList Nature::getFrameNatures()
{
    const QStringList natures =
    {
        QObject::tr("Hardy"), QObject::tr("Lonely"), QObject::tr("Brave"), QObject::tr("Adamant"),
        QObject::tr("Naughty"), QObject::tr("Bold"), QObject::tr("Docile"), QObject::tr("Relaxed"),
        QObject::tr("Impish"), QObject::tr("Lax"), QObject::tr("Timid"), QObject::tr("Hasty"),
        QObject::tr("Serious"), QObject::tr("Jolly"), QObject::tr("Naive"), QObject::tr("Modest"),
        QObject::tr("Mild"), QObject::tr("Quiet"), QObject::tr("Bashful"), QObject::tr("Rash"),
        QObject::tr("Calm"), QObject::tr("Gentle"), QObject::tr("Sassy"), QObject::tr("Careful"),
        QObject::tr("Quirky")
    };
    return natures;
}

u8 Nature::getAdjustedNature(u8 nature)
{
    const u8 vals[25] = { 3, 5, 2, 20, 23, 11, 8, 13, 1, 16, 15, 14, 4, 17, 19, 7, 22, 10, 21, 9, 18, 6, 0, 24, 12 };
    return vals[nature];
}

u8 Nature::getReversedNature(u8 nature)
{
    const u8 vals[25] = { 22, 8, 2, 0, 12, 1, 21, 15, 6, 19, 17, 5, 24, 7, 11, 10, 9, 13, 20, 14, 3, 18, 16, 4, 23 };
    return vals[nature];
}

QVector<double> Nature::getNatureModifier(u8 nature)
{
    const QVector<QVector<double>> modifiers =
    {
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Hardy
        { 1.0, 1.1, 0.9, 1.0, 1.0, 1.0 }, // Lonely
        { 1.0, 1.1, 1.0, 1.0, 1.0, 0.9 }, // Brave
        { 1.0, 1.1, 1.0, 0.9, 1.0, 1.0 }, // Adamant
        { 1.0, 1.1, 1.0, 1.0, 0.9, 1.0 }, // Naughty
        { 1.0, 0.9, 1.1, 1.0, 1.0, 1.0 }, // Bold
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Docile
        { 1.0, 1.0, 1.1, 1.0, 1.0, 0.9 }, // Relaxed
        { 1.0, 1.0, 1.1, 0.9, 1.0, 1.0 }, // Impish
        { 1.0, 1.0, 1.1, 1.0, 0.9, 1.0 }, // Lax
        { 1.0, 0.9, 1.0, 1.0, 1.0, 1.1 }, // Timid
        { 1.0, 1.0, 0.9, 1.0, 1.0, 1.1 }, // Hasty
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Serious
        { 1.0, 1.0, 1.0, 0.9, 1.0, 1.1 }, // Jolly
        { 1.0, 1.0, 1.0, 1.0, 0.9, 1.1 }, // Naive
        { 1.0, 0.9, 1.0, 1.1, 1.0, 1.0 }, // Modest
        { 1.0, 1.0, 0.9, 1.1, 1.0, 1.0 }, // Mild
        { 1.0, 1.0, 1.0, 1.1, 1.0, 0.9 }, // Quiet
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Bashful
        { 1.0, 1.0, 1.0, 1.1, 0.9, 1.0 }, // Rash
        { 1.0, 0.9, 1.0, 1.0, 1.1, 1.0 }, // Calm
        { 1.0, 1.0, 0.9, 1.0, 1.1, 1.0 }, // Gentle
        { 1.0, 1.0, 1.0, 1.0, 1.1, 0.9 }, // Sassy
        { 1.0, 1.0, 1.0, 0.9, 1.1, 1.0 }, // Careful
        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }  // Quirky
    };
    return modifiers.at(nature);
}
