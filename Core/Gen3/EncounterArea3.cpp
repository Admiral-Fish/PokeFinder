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

#include "EncounterArea3.hpp"

EncounterArea3::EncounterArea3(u8 location, u16 delay, Encounter type, const QVector<Slot> &pokemon)
    : EncounterArea(location, type, pokemon)
{
    this->delay = delay;
}

u8 EncounterArea3::calcLevel(u8 index, u16 prng) const
{
    return (prng % (pokemon.at(index).getMaxLevel() - pokemon.at(index).getMinLevel() + 1)) + pokemon.at(index).getMinLevel();
}

u8 EncounterArea3::calcLevel(u8 index) const
{
    return pokemon.at(index).getMinLevel();
}

// Only for Rock Smash since all other encounters can be forced
u8 EncounterArea3::getEncounterRate() const
{
    switch (location)
    {
        case 18: // Route 111
        case 21: // Route 114
        case 47: // Granite Cave B2F
        case 72: // Victory Road B1F (RSE)
            return 20;
        case 51: // Safari Zone 4
        case 53: // Safari Zone 6
            return 25;
        // FRLG doesn't force encounter
        // Uses a different RNG to proc the encounter
        case 134: // Rock Tunnel B1F (50)
        case 145: // Cerulean Cave 1F  (50)
        case 146: // Cerulean Cave 2F (50)
        case 147: // Cerulean Cave B1F (50)
        case 152: // Kindle Road (25)
        case 167: // Sevault Canyon (25)
        case 169: // Mt. Ember (50)
        case 171: // Mt. Ember Room 2 (50)
        case 173: // Mt. Ember 1F (50)
        case 174: // Mt. Ember B1F (50)
        case 175: // Mt. Ember B2F (50)
        case 176: // Mt. Ember B3F (50)
        case 177: // Mt. Ember B4F (50)
        case 178: // Mt. Ember B5F (50)
            return 180;
        default:
            return 0;
    }
}

u16 EncounterArea3::getDelay() const
{
    return delay;
}

QDataStream &operator>>(QDataStream &in, EncounterArea3 &encounter)
{
    QVariant location, delay, type;
    in >> location >> delay >> type;

    int size;
    switch (static_cast<Encounter>(type.toInt()))
    {
        case Encounter::Grass:
        case Encounter::SafariZone:
            size = 12;
            break;
        case Encounter::OldRod:
            size = 2;
            break;
        case Encounter::GoodRod:
            size = 3;
            break;
        case Encounter::RockSmash:
        case Encounter::SuperRod:
        case Encounter::Surfing:
            size = 5;
            break;
        default:
            size = 0;
            break;
    }

    QVector<Slot> pokemon;
    for (int i = 0; i < size; i++)
    {
        QVariant specie, min, max;
        in >> specie >> min >> max;
        pokemon.append(Slot(specie.toUInt(), min.toUInt(), max.toUInt()));
    }

    encounter = EncounterArea3(location.toUInt(), delay.toUInt(), static_cast<Encounter>(type.toInt()), pokemon);
    return in;
}
