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
    QByteArrayList entries = getData();
    for (int i = 0; i < entries.size(); i++)
    {
        const QByteArray &data = entries.at(i);
        if (profile.getVersion() & Game::HGSS)
        {
            for (const auto &encounter : getHGSS(data, i))
            {
                if (encounter.getType() == type)
                {
                    encounters.append(encounter);
                }
            }
        }
        else
        {
            for (const auto &encounter : getDPPt(data, i))
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
            size = 158;
            break;
        case Game::Pearl:
            path = ":/encounters/pearl.bin";
            size = 158;
            break;
        case Game::Platinum:
            path = ":/encounters/platinum.bin";
            size = 158;
            break;
        case Game::HeartGold:
            path = ":/encounters/heartgold.bin";
            size = 191;
            break;
        case Game::SoulSilver:
        default:
            path = ":/encounters/soulsilver.bin";
            size = 191;
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

QVector<EncounterArea4> Encounters4::getHGSS(const QByteArray &data, int i) const
{
    QVector<EncounterArea4> encounters;
    if (getValue(data, 0, 1) == 1)
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
            int level = getValue(data, 3 + i * 7, 1);
            int specie = getValue(data, 3 + t + i * 7, 2);
            pokemon.append(Slot(specie, level));
        }

        modifyRadio(pokemon, data);
        modifySwarmHGSS(pokemon, data);

        encounters.append(EncounterArea4(i, Encounter::Grass, pokemon));
    }
    if (getValue(data, 1, 1) == 1)
    {
        QVector<Slot> pokemon;
        for (int i = 0; i < 2; i++)
        {
            int min = getValue(data, 95 + i * 4, 1);
            int max = getValue(data, 96 + i * 4, 1);
            int specie = getValue(data, 97 + i * 4, 2);
            pokemon.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(i, Encounter::RockSmash, pokemon));
    }
    if (getValue(data, 2, 1) == 1)
    {
        QVector<Slot> surf;
        for (int i = 0; i < 5; i++)
        {
            int min = getValue(data, 103 + i * 4, 1);
            int max = getValue(data, 103 + 1 + i * 4, 1);
            int specie = getValue(data, 103 + 2 + i * 4, 2);
            surf.append(Slot(specie, min, max));
        }
        modifySwarmHGSS(surf, data);
        encounters.append(EncounterArea4(i, Encounter::Surfing, surf));

        QVector<Slot> old;
        for (int i = 0; i < 5; i++)
        {
            int min = getValue(data, 123 + i * 4, 1);
            int max = getValue(data, 123 + 1 + i * 4, 1);
            int specie = getValue(data, 123 + 2 + i * 4, 2);
            old.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(i, Encounter::OldRod, old));

        QVector<Slot> good;
        for (int i = 0; i < 5; i++)
        {
            int min = getValue(data, 143 + i * 4, 1);
            int max = getValue(data, 143 + 1 + i * 4, 1);
            int specie = getValue(data, 143 + 2 + i * 4, 2);
            good.append(Slot(specie, min, max));
        }
        modifySwarmHGSS(good, data);
        encounters.append(EncounterArea4(i, Encounter::GoodRod, good));

        QVector<Slot> super;
        for (int i = 0; i < 5; i++)
        {
            int min = getValue(data, 163 + i * 4, 1);
            int max = getValue(data, 163 + 1 + i * 4, 1);
            int specie = getValue(data, 163 + 2 + i * 4, 2);
            super.append(Slot(specie, min, max));
        }
        modifySwarmHGSS(super, data);
        encounters.append(EncounterArea4(i, Encounter::SuperRod, super));
    }
    return encounters;
}

QVector<EncounterArea4> Encounters4::getDPPt(const QByteArray &data, int i) const
{
    QVector<EncounterArea4> encounters;
    if (getValue(data, 0, 1) == 1)
    {
        QVector<Slot> pokemon;
        for (int i = 0; i < 12; i++)
        {
            int level = getValue(data, 2 + i * 3, 1);
            int specie = getValue(data, 3 + i * 3, 2);
            pokemon.append(Slot(specie, level));
        }

        modifyTime(pokemon, data);
        modifyDual(pokemon, data);
        modifyRadar(pokemon, data);
        modifySwarmDPPt(pokemon, data);

        encounters.append(EncounterArea4(i, Encounter::Grass, pokemon));
    }
    if (getValue(data, 1, 1) == 1)
    {
        QVector<Slot> surf;
        for (int i = 0; i < 5; i++)
        {
            int min = getValue(data, 78 + i * 4, 1);
            int max = getValue(data, 78 + 1 + i * 4, 1);
            int specie = getValue(data, 78 + 2 + i * 4, 2);
            surf.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(i, Encounter::Surfing, surf));

        QVector<Slot> old;
        for (int i = 0; i < 5; i++)
        {
            int min = getValue(data, 98 + i * 4, 1);
            int max = getValue(data, 98 + 1 + i * 4, 1);
            int specie = getValue(data, 98 + 2 + i * 4, 2);
            old.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(i, Encounter::OldRod, old));

        QVector<Slot> good;
        for (int i = 0; i < 5; i++)
        {
            int min = getValue(data, 118 + i * 4, 1);
            int max = getValue(data, 118 + 1 + i * 4, 1);
            int specie = getValue(data, 118 + 2 + i * 4, 2);
            good.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(i, Encounter::GoodRod, good));

        QVector<Slot> super;
        for (int i = 0; i < 5; i++)
        {
            int min = getValue(data, 138 + i * 4, 1);
            int max = getValue(data, 138 + 1 + i * 4, 1);
            int specie = getValue(data, 138 + 2 + i * 4, 2);
            super.append(Slot(specie, min, max));
        }
        encounters.append(EncounterArea4(i, Encounter::SuperRod, super));
    }
    return encounters;
}

void Encounters4::modifyRadio(QVector<Slot> &pokemon, const QByteArray &data) const
{
    if (profile.getRadio() == 1)
    {
        pokemon[2].setSpecie(getValue(data, 87, 2));
        pokemon[3].setSpecie(getValue(data, 87, 2));
        pokemon[4].setSpecie(getValue(data, 89, 2));
        pokemon[5].setSpecie(getValue(data, 89, 2));
    }
    else if (profile.getRadio() == 2)
    {
        pokemon[2].setSpecie(getValue(data, 91, 2));
        pokemon[3].setSpecie(getValue(data, 91, 2));
        pokemon[4].setSpecie(getValue(data, 93, 2));
        pokemon[5].setSpecie(getValue(data, 93, 2));
    }
}

void Encounters4::modifyTime(QVector<Slot> &pokemon, const QByteArray &data) const
{
    if (time == 1)
    {
        pokemon[2].setSpecie(getValue(data, 42, 2));
        pokemon[3].setSpecie(getValue(data, 44, 2));
    }
    else if (time == 2)
    {
        pokemon[2].setSpecie(getValue(data, 46, 2));
        pokemon[3].setSpecie(getValue(data, 48, 2));
    }
}

void Encounters4::modifyDual(QVector<Slot> &pokemon, const QByteArray &data) const
{
    Game dual = profile.getDualSlot();
    if (dual == Game::Ruby)
    {
        pokemon[8].setSpecie(getValue(data, 58, 2));
        pokemon[9].setSpecie(getValue(data, 60, 2));
    }
    else if (dual == Game::Sapphire)
    {
        pokemon[8].setSpecie(getValue(data, 62, 2));
        pokemon[9].setSpecie(getValue(data, 64, 2));
    }
    else if (dual == Game::Emerald)
    {
        pokemon[8].setSpecie(getValue(data, 66, 2));
        pokemon[9].setSpecie(getValue(data, 68, 2));
    }
    else if (dual == Game::FireRed)
    {
        pokemon[8].setSpecie(getValue(data, 70, 2));
        pokemon[9].setSpecie(getValue(data, 72, 2));
    }
    else if (dual == Game::LeafGreen)
    {
        pokemon[8].setSpecie(getValue(data, 74, 2));
        pokemon[9].setSpecie(getValue(data, 76, 2));
    }
}

void Encounters4::modifyRadar(QVector<Slot> &pokemon, const QByteArray &data) const
{
    if (profile.getRadar())
    {
        pokemon[4].setSpecie(getValue(data, 50, 2));
        pokemon[5].setSpecie(getValue(data, 52, 2));
        pokemon[10].setSpecie(getValue(data, 54, 2));
        pokemon[11].setSpecie(getValue(data, 56, 2));
    }
}

void Encounters4::modifySwarmHGSS(QVector<Slot> &pokemon, const QByteArray &data) const
{
    if (profile.getSwarm())
    {
        if (type == Encounter::Grass)
        {
            pokemon[0].setSpecie(getValue(data, 183, 2));
            pokemon[1].setSpecie(getValue(data, 183, 2));
        }
        else if (type == Encounter::Surfing)
        {
            pokemon[0].setSpecie(getValue(data, 185, 2));
            pokemon[1].setSpecie(getValue(data, 185, 2));
        }
        else if (type == Encounter::GoodRod)
        {
            pokemon[0].setSpecie(getValue(data, 187, 2));
            pokemon[1].setSpecie(getValue(data, 187, 2));
        }
        else if (type == Encounter::SuperRod)
        {
            pokemon[0].setSpecie(getValue(data, 189, 2));
            pokemon[1].setSpecie(getValue(data, 189, 2));
        }
    }
}

void Encounters4::modifySwarmDPPt(QVector<Slot> &pokemon, const QByteArray &data) const
{
    if (profile.getSwarm())
    {
        pokemon[0].setSpecie(getValue(data, 38, 2));
        pokemon[1].setSpecie(getValue(data, 40, 2));
    }
}

u16 Encounters4::getValue(const QByteArray &data, int offset, int length) const
{
    return data.mid(offset, length).toHex().toUShort(nullptr, 16);
}
