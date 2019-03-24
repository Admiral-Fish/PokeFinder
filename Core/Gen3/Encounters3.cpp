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

#include "Encounters3.hpp"

Encounters3::Encounters3(Encounter type, const Profile3 &profile)
{
    this->type = type;
    this->profile = profile;
}

QVector<EncounterArea3> Encounters3::getEncounters() const
{
    QVector<EncounterArea3> encounters;
    for (const auto &data : getData())
    {
        for (const auto &encounter : getArea(data))
        {
            if (encounter.getType() == type)
            {
                encounters.append(encounter);
            }
        }
    }

    return encounters;
}

QByteArrayList Encounters3::getData() const
{
    QString path;
    int size = 122;
    switch (profile.getVersion())
    {
        case Game::Emerald:
            path = ":/encounters/emerald.bin";
            break;
        case Game::Ruby:
            path = ":/encounters/ruby.bin";
            break;
        case Game::Sapphire:
            path = ":/encounters/sapphire.bin";
            break;
        case Game::FireRed:
            path = ":/encounters/firered.bin";
            break;
        case Game::LeafGreen:
        default:
            path = ":/encounters/leafgreen.bin";
            break;
    }

    QByteArray data;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
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

QVector<EncounterArea3> Encounters3::getArea(const QByteArray &data) const
{
    QVector<EncounterArea3> encounters;
    u8 location = getValue(data, 0, 1);
    u16 delay = getValue(data, 1, 2);

    if (getValue(data, 3, 1) == 1 || getValue(data, 3, 1) == 2)
    {
        QVector<Slot> pokemon;
        for (int i = 0; i < 12; i++)
        {
            u8 level = getValue(data, 6 + i * 3, 1);
            u16 specie = getValue(data, 7 + i * 3, 2);
            pokemon.append(Slot(specie, level));
        }

        u16 val = getValue(data, 3, 1);
        encounters.append(EncounterArea3(location, delay, val == 1 ? Encounter::Grass : Encounter::SafariZone, pokemon));
    }
    if (getValue(data, 4, 1) == 1)
    {
        QVector<Slot> pokemon;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 42 + i * 4, 1);
            u8 max = getValue(data, 43 + i * 4, 1);
            u16 specie = getValue(data, 44 + i * 4, 2);
            pokemon.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea3(location, delay, Encounter::RockSmash, pokemon));
    }
    if (getValue(data, 5, 1) == 1)
    {
        QVector<Slot> surf;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 62 + i * 4, 1);
            u8 max = getValue(data, 63 + i * 4, 1);
            u16 specie = getValue(data, 64 + i * 4, 2);
            surf.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea3(location, delay, Encounter::Surfing, surf));

        QVector<Slot> old;
        for (int i = 0; i < 2; i++)
        {
            u8 min = getValue(data, 82 + i * 4, 1);
            u8 max = getValue(data, 83 + i * 4, 1);
            u16 specie = getValue(data, 84 + i * 4, 2);
            old.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea3(location, delay, Encounter::OldRod, old));

        QVector<Slot> good;
        for (int i = 0; i < 3; i++)
        {
            u8 min = getValue(data, 90 + i * 4, 1);
            u8 max = getValue(data, 91 + i * 4, 1);
            u16 specie = getValue(data, 92 + i * 4, 2);
            good.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea3(location, delay, Encounter::GoodRod, good));

        QVector<Slot> super;
        for (int i = 0; i < 5; i++)
        {
            u8 min = getValue(data, 102 + i * 4, 1);
            u8 max = getValue(data, 103 + i * 4, 1);
            u16 specie = getValue(data, 104 + i * 4, 2);
            super.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea3(location, delay, Encounter::SuperRod, super));
    }
    return encounters;
}

u16 Encounters3::getValue(const QByteArray &data, int offset, int length) const
{
    return data.mid(offset, length).toHex().toUShort(nullptr, 16);
}
