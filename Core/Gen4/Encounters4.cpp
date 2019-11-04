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

#include "Encounters4.hpp"
#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Util/Encounter.hpp>
#include <QFile>

namespace PokeFinderCore
{
    Encounters4::Encounters4(Encounter type, int time, const Profile4 &profile)
        : pokemon(Pokemon::loadPersonal(4))
    {
        this->type = type;
        this->time = time;
        this->profile = profile;
    }

    QVector<EncounterArea4> Encounters4::getEncounters() const
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

    QVector<EncounterArea4> Encounters4::getHGSS(const QByteArray &data) const
    {
        QVector<EncounterArea4> encounters;
        u8 location = getValue(data, 0, 1);

        if (getValue(data, 1, 1) == 1)
        {
            int t;
            if (time == 0)
            {
                t = 1;
            }
            else if (time == 1)
            {
                t = 3;
            }
            else
            {
                t = 5;
            }

            QVector<Slot> grass;
            for (int i = 0; i < 12; i++)
            {
                u8 level = getValue(data, 4 + i * 7, 1);
                u16 specie = getValue(data, 4 + t + i * 7, 2);
                grass.append(Slot(specie, level, pokemon.at(specie)));
            }

            modifyRadio(grass, data);
            modifySwarmHGSS(grass, data);

            encounters.append(EncounterArea4(location, Encounter::Grass, grass));
        }
        if (getValue(data, 2, 1) == 1)
        {
            QVector<Slot> rock;
            for (int i = 0; i < 2; i++)
            {
                u8 min = getValue(data, 96 + i * 4, 1);
                u8 max = getValue(data, 97 + i * 4, 1);
                u16 specie = getValue(data, 98 + i * 4, 2);
                rock.append(Slot(specie, min, max, pokemon.at(specie)));
            }
            encounters.append(EncounterArea4(location, Encounter::RockSmash, rock));
        }
        if (getValue(data, 3, 1) == 1)
        {
            QVector<Slot> surf;
            for (int i = 0; i < 5; i++)
            {
                u8 min = getValue(data, 104 + i * 4, 1);
                u8 max = getValue(data, 105 + i * 4, 1);
                u16 specie = getValue(data, 106 + i * 4, 2);
                surf.append(Slot(specie, min, max, pokemon.at(specie)));
            }
            modifySwarmHGSS(surf, data);
            encounters.append(EncounterArea4(location, Encounter::Surfing, surf));

            QVector<Slot> old;
            for (int i = 0; i < 5; i++)
            {
                u8 min = getValue(data, 124 + i * 4, 1);
                u8 max = getValue(data, 125 + i * 4, 1);
                u16 specie = getValue(data, 126 + i * 4, 2);
                old.append(Slot(specie, min, max, pokemon.at(specie)));
            }
            encounters.append(EncounterArea4(location, Encounter::OldRod, old));

            QVector<Slot> good;
            for (int i = 0; i < 5; i++)
            {
                u8 min = getValue(data, 144 + i * 4, 1);
                u8 max = getValue(data, 145 + i * 4, 1);
                u16 specie = getValue(data, 146 + i * 4, 2);
                good.append(Slot(specie, min, max, pokemon.at(specie)));
            }
            modifySwarmHGSS(good, data);
            encounters.append(EncounterArea4(location, Encounter::GoodRod, good));

            QVector<Slot> super;
            for (int i = 0; i < 5; i++)
            {
                u8 min = getValue(data, 164 + i * 4, 1);
                u8 max = getValue(data, 165 + i * 4, 1);
                u16 specie = getValue(data, 166 + i * 4, 2);
                super.append(Slot(specie, min, max, pokemon.at(specie)));
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
            QVector<Slot> grass;
            for (int i = 0; i < 12; i++)
            {
                u8 level = getValue(data, 3 + i * 3, 1);
                u16 specie = getValue(data, 4 + i * 3, 2);
                grass.append(Slot(specie, level, pokemon.at(specie)));
            }

            modifyTime(grass, data);
            modifyDual(grass, data);
            modifyRadar(grass, data);
            modifySwarmDPPt(grass, data);

            encounters.append(EncounterArea4(location, Encounter::Grass, grass));
        }
        if (getValue(data, 2, 1) == 1)
        {
            QVector<Slot> surf;
            for (int i = 0; i < 5; i++)
            {
                u8 min = getValue(data, 79 + i * 4, 1);
                u8 max = getValue(data, 80 + i * 4, 1);
                u16 specie = getValue(data, 81 + i * 4, 2);
                surf.append(Slot(specie, min, max, pokemon.at(specie)));
            }
            encounters.append(EncounterArea4(location, Encounter::Surfing, surf));

            QVector<Slot> old;
            for (int i = 0; i < 5; i++)
            {
                u8 min = getValue(data, 99 + i * 4, 1);
                u8 max = getValue(data, 100 + i * 4, 1);
                u16 specie = getValue(data, 101 + i * 4, 2);
                old.append(Slot(specie, min, max, pokemon.at(specie)));
            }
            encounters.append(EncounterArea4(location, Encounter::OldRod, old));

            QVector<Slot> good;
            for (int i = 0; i < 5; i++)
            {
                u8 min = getValue(data, 119 + i * 4, 1);
                u8 max = getValue(data, 120 + i * 4, 1);
                u16 specie = getValue(data, 121 + i * 4, 2);
                good.append(Slot(specie, min, max, pokemon.at(specie)));
            }
            encounters.append(EncounterArea4(location, Encounter::GoodRod, good));

            QVector<Slot> super;
            for (int i = 0; i < 5; i++)
            {
                u8 min = getValue(data, 139 + i * 4, 1);
                u8 max = getValue(data, 140 + i * 4, 1);
                u16 specie = getValue(data, 141 + i * 4, 2);
                super.append(Slot(specie, min, max, pokemon.at(specie)));
            }
            encounters.append(EncounterArea4(location, Encounter::SuperRod, super));
        }
        return encounters;
    }

    void Encounters4::modifyRadio(QVector<Slot> &mons, const QByteArray &data) const
    {
        u16 specie1, specie2;
        if (profile.getRadio() == 1)
        {
            specie1 = getValue(data, 88, 2);
            specie2 = getValue(data, 90, 2);
        }
        else if (profile.getRadio() == 2)
        {
            specie1 = getValue(data, 92, 2);
            specie2 = getValue(data, 94, 2);
        }
        else
        {
            return;
        }

        mons[2].setSpecie(specie1, pokemon.at(specie1));
        mons[3].setSpecie(specie1, pokemon.at(specie1));
        mons[4].setSpecie(specie2, pokemon.at(specie2));
        mons[5].setSpecie(specie2, pokemon.at(specie2));
    }

    void Encounters4::modifyTime(QVector<Slot> &slot, const QByteArray &data) const
    {
        u16 specie1, specie2;
        if (time == 1)
        {
            specie1 = getValue(data, 43, 2);
            specie2 = getValue(data, 45, 2);
        }
        else if (time == 2)
        {
            specie1 = getValue(data, 47, 2);
            specie2 = getValue(data, 49, 2);
        }
        else
        {
            return;
        }

        slot[2].setSpecie(specie1, pokemon.at(specie1));
        slot[3].setSpecie(specie2, pokemon.at(specie2));
    }

    void Encounters4::modifyDual(QVector<Slot> &mons, const QByteArray &data) const
    {
        Game dual = profile.getDualSlot();
        u16 specie1, specie2;
        if (dual == Game::Ruby)
        {
            specie1 = getValue(data, 59, 2);
            specie2 = getValue(data, 61, 2);
        }
        else if (dual == Game::Sapphire)
        {
            specie1 = getValue(data, 63, 2);
            specie2 = getValue(data, 65, 2);
        }
        else if (dual == Game::Emerald)
        {
            specie1 = getValue(data, 67, 2);
            specie2 = getValue(data, 69, 2);
        }
        else if (dual == Game::FireRed)
        {
            specie1 = getValue(data, 71, 2);
            specie2 = getValue(data, 73, 2);
        }
        else if (dual == Game::LeafGreen)
        {
            specie1 = getValue(data, 75, 2);
            specie2 = getValue(data, 77, 2);
        }
        else
        {
            return;
        }

        mons[8].setSpecie(specie1, pokemon.at(specie1));
        mons[9].setSpecie(specie2, pokemon.at(specie2));
    }

    void Encounters4::modifyRadar(QVector<Slot> &mons, const QByteArray &data) const
    {
        if (profile.getRadar())
        {
            QVector<u16> species
                = { getValue(data, 51, 2), getValue(data, 53, 2), getValue(data, 55, 2), getValue(data, 57, 2) };
            mons[4].setSpecie(species.at(0), pokemon.at(species.at(0)));
            mons[5].setSpecie(species.at(1), pokemon.at(species.at(1)));
            mons[10].setSpecie(species.at(2), pokemon.at(species.at(2)));
            mons[11].setSpecie(species.at(3), pokemon.at(species.at(3)));
        }
    }

    void Encounters4::modifySwarmHGSS(QVector<Slot> &mons, const QByteArray &data) const
    {
        if (profile.getSwarm())
        {
            u16 specie;
            if (type == Encounter::Grass)
            {
                specie = getValue(data, 184, 2);
            }
            else if (type == Encounter::Surfing)
            {
                specie = getValue(data, 186, 2);
            }
            else if (type == Encounter::GoodRod)
            {
                specie = getValue(data, 188, 2);
            }
            else if (type == Encounter::SuperRod)
            {
                specie = getValue(data, 190, 2);
            }
            else
            {
                return;
            }

            mons[0].setSpecie(specie, pokemon.at(specie));
            mons[1].setSpecie(specie, pokemon.at(specie));
        }
    }

    void Encounters4::modifySwarmDPPt(QVector<Slot> &mons, const QByteArray &data) const
    {
        if (profile.getSwarm())
        {
            QVector<u16> species = { getValue(data, 39, 2), getValue(data, 41, 2) };
            mons[0].setSpecie(species.at(0), pokemon.at(species.at(0)));
            mons[1].setSpecie(species.at(1), pokemon.at(species.at(1)));
        }
    }

    u16 Encounters4::getValue(const QByteArray &data, int offset, int length) const
    {
        return data.mid(offset, length).toHex().toUShort(nullptr, 16);
    }
}
