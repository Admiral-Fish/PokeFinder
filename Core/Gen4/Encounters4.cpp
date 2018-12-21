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

#include "Encounters4.hpp"

Encounters4::Encounters4(Encounter type, int time, const Profile4 &profile)
{
    this->type = type;
    this->time = time;
    this->profile = profile;
}

QVector<EncounterArea4> Encounters4::getEncounters()
{
    QVector<EncounterArea4> encounters;
    for (const auto &data : getData())
    {
        if (profile.getVersion() & Game::HGSS)
        {
            for (const auto &encounter : getHGSS(data))
            {
                if (encounter.getType() == type)
                {
                    encounters.append(encounter);
                }
            }
        }
        else
        {
            for (const auto &encounter : getDPPt(data))
            {
                if (encounter.getType() == type)
                {
                    encounters.append(encounter);
                }
            }
        }
    }

    return encounters;
}

QByteArrayList Encounters4::getData() const
{
    QString path;
    int size;
    switch (profile.getVersion())
    {
        case Game::Diamond:
            path = ":/encounters/diamond.bin";
            size = 159;
            break;
        case Game::Pearl:
            path = ":/encounters/pearl.bin";
            size = 159;
            break;
        case Game::Platinum:
            path = ":/encounters/platinum.bin";
            size = 159;
            break;
        case Game::HeartGold:
            path = ":/encounters/heartgold.bin";
            size = 192;
            break;
        case Game::SoulSilver:
        default:
            path = ":/encounters/soulsilver.bin";
            size = 192;
            break;
    }

    QByteArray data;
    if (QFile file(path); file.open(QIODevice::ReadOnly))
    {
        data = file.readAll();
        file.close();
    }

    QByteArrayList encounters;
    for (int i = 0; i < data.size(); i += size)
    {
        encounters.append(data.mid(i, size));
    }

    return encounters;
}

QVector<EncounterArea4> Encounters4::getHGSS(const QByteArray &data) const
{
    QVector<EncounterArea4> encounters;
    u8 location = getValue(data, 0, 1);

    if (getValue(data, 1, 1) == 1)
    {
        int t;
        if (time == 0)
            t = 1;
        else if (time == 1)
            t = 3;
        else
            t = 5;

        QVector<Slot> pokemon;
        for (int i = 0; i < 12; i++)
        {
            u8 level = getValue(data, 4 + i * 7, 1);
            u16 specie = getValue(data, 4 + t + i * 7, 2);
            pokemon.append(Slot(specie, level));
        }

        modifyRadio(pokemon, data);
        modifySwarmHGSS(pokemon, data);

        encounters.append(EncounterArea4(location, Encounter::Grass, pokemon));
    }
    if (getValue(data, 2, 1) == 1)
    {
        QVector<Slot> pokemon;
        for (int i = 0; i < 2; i++)
        {
            u8 min = getValue(data, 96 + i * 4, 1);
            u8 max = getValue(data, 97 + i * 4, 1);
            u16 specie = getValue(data, 98 + i * 4, 2);
            pokemon.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(location, Encounter::RockSmash, pokemon));
    }
    if (getValue(data, 3, 1) == 1)
    {
        QVector<Slot> surf;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 104 + i * 4, 1);
            u8 max = getValue(data, 105 + i * 4, 1);
            u16 specie = getValue(data, 106 + i * 4, 2);
            surf.append(Slot(specie, min, max));
        }
        modifySwarmHGSS(surf, data);
        encounters.append(EncounterArea4(location, Encounter::Surfing, surf));

        QVector<Slot> old;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 124 + i * 4, 1);
            u8 max = getValue(data, 125 + i * 4, 1);
            u16 specie = getValue(data, 126 + i * 4, 2);
            old.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(location, Encounter::OldRod, old));

        QVector<Slot> good;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 144 + i * 4, 1);
            u8 max = getValue(data, 145 + i * 4, 1);
            u16 specie = getValue(data, 146 + i * 4, 2);
            good.append(Slot(specie, min, max));
        }
        modifySwarmHGSS(good, data);
        encounters.append(EncounterArea4(location, Encounter::GoodRod, good));

        QVector<Slot> super;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 164 + i * 4, 1);
            u8 max = getValue(data, 165 + i * 4, 1);
            u16 specie = getValue(data, 166 + i * 4, 2);
            super.append(Slot(specie, min, max));
        }
        modifySwarmHGSS(super, data);
        encounters.append(EncounterArea4(location, Encounter::SuperRod, super));
    }
    return encounters;
}

QVector<EncounterArea4> Encounters4::getDPPt(const QByteArray &data) const
{
    QVector<EncounterArea4> encounters;
    u8 location = getValue(data, 0, 1);

    if (getValue(data, 1, 1) == 1)
    {
        QVector<Slot> pokemon;
        for (int i = 0; i < 12; i++)
        {
            u8 level = getValue(data, 3 + i * 3, 1);
            u16 specie = getValue(data, 4 + i * 3, 2);
            pokemon.append(Slot(specie, level));
        }

        modifyTime(pokemon, data);
        modifyDual(pokemon, data);
        modifyRadar(pokemon, data);
        modifySwarmDPPt(pokemon, data);

        encounters.append(EncounterArea4(location, Encounter::Grass, pokemon));
    }
    if (getValue(data, 2, 1) == 1)
    {
        QVector<Slot> surf;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 79 + i * 4, 1);
            u8 max = getValue(data, 80 + i * 4, 1);
            u16 specie = getValue(data, 81 + i * 4, 2);
            surf.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(location, Encounter::Surfing, surf));

        QVector<Slot> old;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 99 + i * 4, 1);
            u8 max = getValue(data, 100 + i * 4, 1);
            u16 specie = getValue(data, 101 + i * 4, 2);
            old.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(location, Encounter::OldRod, old));

        QVector<Slot> good;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 119 + i * 4, 1);
            u8 max = getValue(data, 120 + i * 4, 1);
            u16 specie = getValue(data, 121 + i * 4, 2);
            good.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(location, Encounter::GoodRod, good));

        QVector<Slot> super;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 139 + i * 4, 1);
            u8 max = getValue(data, 140 + i * 4, 1);
            u16 specie = getValue(data, 141 + i * 4, 2);
            super.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(location, Encounter::SuperRod, super));
    }
    return encounters;
}

void Encounters4::modifyRadio(QVector<Slot> &pokemon, const QByteArray &data) const
{
    if (profile.getRadio() == 1)
    {
        pokemon[2].setSpecie(getValue(data, 88, 2));
        pokemon[3].setSpecie(getValue(data, 88, 2));
        pokemon[4].setSpecie(getValue(data, 90, 2));
        pokemon[5].setSpecie(getValue(data, 90, 2));
    }
    else if (profile.getRadio() == 2)
    {
        pokemon[2].setSpecie(getValue(data, 92, 2));
        pokemon[3].setSpecie(getValue(data, 92, 2));
        pokemon[4].setSpecie(getValue(data, 94, 2));
        pokemon[5].setSpecie(getValue(data, 94, 2));
    }
}

void Encounters4::modifyTime(QVector<Slot> &pokemon, const QByteArray &data) const
{
    if (time == 1)
    {
        pokemon[2].setSpecie(getValue(data, 43, 2));
        pokemon[3].setSpecie(getValue(data, 45, 2));
    }
    else if (time == 2)
    {
        pokemon[2].setSpecie(getValue(data, 47, 2));
        pokemon[3].setSpecie(getValue(data, 49, 2));
    }
}

void Encounters4::modifyDual(QVector<Slot> &pokemon, const QByteArray &data) const
{
    Game dual = profile.getDualSlot();
    if (dual == Game::Ruby)
    {
        pokemon[8].setSpecie(getValue(data, 59, 2));
        pokemon[9].setSpecie(getValue(data, 61, 2));
    }
    else if (dual == Game::Sapphire)
    {
        pokemon[8].setSpecie(getValue(data, 63, 2));
        pokemon[9].setSpecie(getValue(data, 65, 2));
    }
    else if (dual == Game::Emerald)
    {
        pokemon[8].setSpecie(getValue(data, 67, 2));
        pokemon[9].setSpecie(getValue(data, 69, 2));
    }
    else if (dual == Game::FireRed)
    {
        pokemon[8].setSpecie(getValue(data, 71, 2));
        pokemon[9].setSpecie(getValue(data, 73, 2));
    }
    else if (dual == Game::LeafGreen)
    {
        pokemon[8].setSpecie(getValue(data, 75, 2));
        pokemon[9].setSpecie(getValue(data, 77, 2));
    }
}

void Encounters4::modifyRadar(QVector<Slot> &pokemon, const QByteArray &data) const
{
    if (profile.getRadar())
    {
        pokemon[4].setSpecie(getValue(data, 51, 2));
        pokemon[5].setSpecie(getValue(data, 53, 2));
        pokemon[10].setSpecie(getValue(data, 55, 2));
        pokemon[11].setSpecie(getValue(data, 57, 2));
    }
}

void Encounters4::modifySwarmHGSS(QVector<Slot> &pokemon, const QByteArray &data) const
{
    if (profile.getSwarm())
    {
        if (type == Encounter::Grass)
        {
            pokemon[0].setSpecie(getValue(data, 184, 2));
            pokemon[1].setSpecie(getValue(data, 184, 2));
        }
        else if (type == Encounter::Surfing)
        {
            pokemon[0].setSpecie(getValue(data, 186, 2));
            pokemon[1].setSpecie(getValue(data, 186, 2));
        }
        else if (type == Encounter::GoodRod)
        {
            pokemon[0].setSpecie(getValue(data, 188, 2));
            pokemon[1].setSpecie(getValue(data, 188, 2));
        }
        else if (type == Encounter::SuperRod)
        {
            pokemon[0].setSpecie(getValue(data, 190, 2));
            pokemon[1].setSpecie(getValue(data, 190, 2));
        }
    }
}

void Encounters4::modifySwarmDPPt(QVector<Slot> &pokemon, const QByteArray &data) const
{
    if (profile.getSwarm())
    {
        pokemon[0].setSpecie(getValue(data, 39, 2));
        pokemon[1].setSpecie(getValue(data, 41, 2));
    }
}

u16 Encounters4::getValue(const QByteArray &data, int offset, int length) const
{
    return data.mid(offset, length).toHex().toUShort(nullptr, 16);
}
