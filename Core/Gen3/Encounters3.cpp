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

#include "Encounters3.hpp"

Encounters3::Encounters3(Encounter type, Game game)
{
    this->type = type;
    this->game = game;
}

QVector<EncounterArea3> Encounters3::getEncounters()
{
    QVector<EncounterArea3> areas;

    QString path;
    switch (game)
    {
        case Ruby:
            path = ":/encounters/ruby.bin";
            break;
        case Sapphire:
            path = ":/encounters/sapphire.bin";
            break;
        case FireRed:
            path = ":/encounters/firered.bin";
            break;
        case LeafGreen:
            path = ":/encounters/leafgreen.bin";
            break;
        case Emerald:
        default:
            path = ":/encounters/emerald.bin";
            break;
    }

    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        stream >> areas;

        for (int i = 0; i < areas.count();)
        {
            if (areas[i].getType() != type)
            {
                areas.removeAt(i);
            }
            else
            {
                i++;
            }
        }

        file.close();
    }

    return areas;
}
