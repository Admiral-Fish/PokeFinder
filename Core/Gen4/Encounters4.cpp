/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Slot.hpp>
#include <QFile>

namespace Encounters4
{
    namespace
    {
        QByteArrayList getData(Game game)
        {
            QString path;
            int size;
            switch (game)
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

        u16 getValue(const QByteArray &data, int offset, int length)
        {
            return data.mid(offset, length).toHex().toUShort(nullptr, 16);
        }

        void modifyRadio(QVector<Slot> &mons, const QByteArray &data, const QVector<PersonalInfo> &info, int radio)
        {
            u16 specie1;
            u16 specie2;
            if (radio == 1)
            {
                specie1 = getValue(data, 88, 2);
                specie2 = getValue(data, 90, 2);
            }
            else if (radio == 2)
            {
                specie1 = getValue(data, 92, 2);
                specie2 = getValue(data, 94, 2);
            }
            else
            {
                return;
            }

            mons[2].setSpecie(specie1, info.at(specie1));
            mons[3].setSpecie(specie1, info.at(specie1));
            mons[4].setSpecie(specie2, info.at(specie2));
            mons[5].setSpecie(specie2, info.at(specie2));
        }

        void modifyTime(QVector<Slot> &mons, const QByteArray &data, const QVector<PersonalInfo> &info, int time)
        {
            u16 specie1;
            u16 specie2;
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

            mons[2].setSpecie(specie1, info.at(specie1));
            mons[3].setSpecie(specie2, info.at(specie2));
        }

        void modifyDual(QVector<Slot> &mons, const QByteArray &data, const QVector<PersonalInfo> &info, Game dual)
        {
            u16 specie1;
            u16 specie2;
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

            mons[8].setSpecie(specie1, info.at(specie1));
            mons[9].setSpecie(specie2, info.at(specie2));
        }

        void modifyRadar(QVector<Slot> &mons, const QByteArray &data, const QVector<PersonalInfo> &info, bool radar)
        {
            if (radar)
            {
                QVector<u16> species = { getValue(data, 51, 2), getValue(data, 53, 2), getValue(data, 55, 2), getValue(data, 57, 2) };
                mons[4].setSpecie(species.at(0), info.at(species.at(0)));
                mons[5].setSpecie(species.at(1), info.at(species.at(1)));
                mons[10].setSpecie(species.at(2), info.at(species.at(2)));
                mons[11].setSpecie(species.at(3), info.at(species.at(3)));
            }
        }

        void modifySwarmHGSS(QVector<Slot> &mons, const QByteArray &data, const QVector<PersonalInfo> &info, Encounter encounter,
                             bool swarm)
        {
            if (swarm)
            {
                u16 specie;
                if (encounter == Encounter::Grass)
                {
                    specie = getValue(data, 184, 2);
                }
                else if (encounter == Encounter::Surfing)
                {
                    specie = getValue(data, 186, 2);
                }
                else if (encounter == Encounter::GoodRod)
                {
                    specie = getValue(data, 188, 2);
                }
                else if (encounter == Encounter::SuperRod)
                {
                    specie = getValue(data, 190, 2);
                }
                else
                {
                    return;
                }

                mons[0].setSpecie(specie, info.at(specie));
                mons[1].setSpecie(specie, info.at(specie));
            }
        }

        void modifySwarmDPPt(QVector<Slot> &mons, const QByteArray &data, const QVector<PersonalInfo> &info, bool swarm)
        {
            if (swarm)
            {
                QVector<u16> species = { getValue(data, 39, 2), getValue(data, 41, 2) };
                mons[0].setSpecie(species.at(0), info.at(species.at(0)));
                mons[1].setSpecie(species.at(1), info.at(species.at(1)));
            }
        }

        QVector<EncounterArea4> getHGSS(const QByteArray &data, const Profile4 &profile, const QVector<PersonalInfo> &info,
                                        Encounter encounter, int time)
        {
            QVector<EncounterArea4> encounters;
            u8 location = static_cast<u8>(getValue(data, 0, 1));

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
                    u8 level = static_cast<u8>(getValue(data, 4 + i * 7, 1));
                    u16 specie = getValue(data, 4 + t + i * 7, 2);
                    grass.append(Slot(specie, level, info.at(specie)));
                }

                modifyRadio(grass, data, info, profile.getRadio());
                modifySwarmHGSS(grass, data, info, encounter, profile.getSwarm());

                encounters.append(EncounterArea4(location, Encounter::Grass, grass));
            }
            if (getValue(data, 2, 1) == 1)
            {
                QVector<Slot> rock;
                for (int i = 0; i < 2; i++)
                {
                    u8 min = static_cast<u8>(getValue(data, 96 + i * 4, 1));
                    u8 max = static_cast<u8>(getValue(data, 97 + i * 4, 1));
                    u16 specie = getValue(data, 98 + i * 4, 2);
                    rock.append(Slot(specie, min, max, info.at(specie)));
                }
                encounters.append(EncounterArea4(location, Encounter::RockSmash, rock));
            }
            if (getValue(data, 3, 1) == 1)
            {
                QVector<Slot> surf;
                for (int i = 0; i < 5; i++)
                {
                    u8 min = static_cast<u8>(getValue(data, 104 + i * 4, 1));
                    u8 max = static_cast<u8>(getValue(data, 105 + i * 4, 1));
                    u16 specie = getValue(data, 106 + i * 4, 2);
                    surf.append(Slot(specie, min, max, info.at(specie)));
                }
                modifySwarmHGSS(surf, data, info, encounter, profile.getSwarm());
                encounters.append(EncounterArea4(location, Encounter::Surfing, surf));

                QVector<Slot> old;
                for (int i = 0; i < 5; i++)
                {
                    u8 min = static_cast<u8>(getValue(data, 124 + i * 4, 1));
                    u8 max = static_cast<u8>(getValue(data, 125 + i * 4, 1));
                    u16 specie = getValue(data, 126 + i * 4, 2);
                    old.append(Slot(specie, min, max, info.at(specie)));
                }
                encounters.append(EncounterArea4(location, Encounter::OldRod, old));

                QVector<Slot> good;
                for (int i = 0; i < 5; i++)
                {
                    u8 min = static_cast<u8>(getValue(data, 144 + i * 4, 1));
                    u8 max = static_cast<u8>(getValue(data, 145 + i * 4, 1));
                    u16 specie = getValue(data, 146 + i * 4, 2);
                    good.append(Slot(specie, min, max, info.at(specie)));
                }
                modifySwarmHGSS(good, data, info, encounter, profile.getSwarm());
                encounters.append(EncounterArea4(location, Encounter::GoodRod, good));

                QVector<Slot> super;
                for (int i = 0; i < 5; i++)
                {
                    u8 min = static_cast<u8>(getValue(data, 164 + i * 4, 1));
                    u8 max = static_cast<u8>(getValue(data, 165 + i * 4, 1));
                    u16 specie = getValue(data, 166 + i * 4, 2);
                    super.append(Slot(specie, min, max, info.at(specie)));
                }
                modifySwarmHGSS(super, data, info, encounter, profile.getSwarm());
                encounters.append(EncounterArea4(location, Encounter::SuperRod, super));
            }
            return encounters;
        }

        QVector<EncounterArea4> getDPPt(const QByteArray &data, const Profile4 &profile, const QVector<PersonalInfo> &info, int time)
        {
            QVector<EncounterArea4> encounters;
            u8 location = static_cast<u8>(getValue(data, 0, 1));

            if (getValue(data, 1, 1) == 1)
            {
                QVector<Slot> grass;
                for (int i = 0; i < 12; i++)
                {
                    u8 level = static_cast<u8>(getValue(data, 3 + i * 3, 1));
                    u16 specie = getValue(data, 4 + i * 3, 2);
                    grass.append(Slot(specie, level, info.at(specie)));
                }

                modifyTime(grass, data, info, time);
                modifyDual(grass, data, info, profile.getDualSlot());
                modifyRadar(grass, data, info, profile.getRadar());
                modifySwarmDPPt(grass, data, info, profile.getSwarm());

                encounters.append(EncounterArea4(location, Encounter::Grass, grass));
            }
            if (getValue(data, 2, 1) == 1)
            {
                QVector<Slot> surf;
                for (int i = 0; i < 5; i++)
                {
                    u8 min = static_cast<u8>(getValue(data, 79 + i * 4, 1));
                    u8 max = static_cast<u8>(getValue(data, 80 + i * 4, 1));
                    u16 specie = getValue(data, 81 + i * 4, 2);
                    surf.append(Slot(specie, min, max, info.at(specie)));
                }
                encounters.append(EncounterArea4(location, Encounter::Surfing, surf));

                QVector<Slot> old;
                for (int i = 0; i < 5; i++)
                {
                    u8 min = static_cast<u8>(getValue(data, 99 + i * 4, 1));
                    u8 max = static_cast<u8>(getValue(data, 100 + i * 4, 1));
                    u16 specie = getValue(data, 101 + i * 4, 2);
                    old.append(Slot(specie, min, max, info.at(specie)));
                }
                encounters.append(EncounterArea4(location, Encounter::OldRod, old));

                QVector<Slot> good;
                for (int i = 0; i < 5; i++)
                {
                    u8 min = static_cast<u8>(getValue(data, 119 + i * 4, 1));
                    u8 max = static_cast<u8>(getValue(data, 120 + i * 4, 1));
                    u16 specie = getValue(data, 121 + i * 4, 2);
                    good.append(Slot(specie, min, max, info.at(specie)));
                }
                encounters.append(EncounterArea4(location, Encounter::GoodRod, good));

                QVector<Slot> super;
                for (int i = 0; i < 5; i++)
                {
                    u8 min = static_cast<u8>(getValue(data, 139 + i * 4, 1));
                    u8 max = static_cast<u8>(getValue(data, 140 + i * 4, 1));
                    u16 specie = getValue(data, 141 + i * 4, 2);
                    super.append(Slot(specie, min, max, info.at(specie)));
                }
                encounters.append(EncounterArea4(location, Encounter::SuperRod, super));
            }
            return encounters;
        }
    }

    QVector<EncounterArea4> getEncounters(Encounter encounter, int time, const Profile4 &profile)
    {
        QVector<EncounterArea4> encounters;
        QVector<PersonalInfo> info = PersonalInfo::loadPersonal(4);

        for (const auto &data : getData(profile.getVersion()))
        {
            auto areas
                = (profile.getVersion() & Game::HGSS) ? getHGSS(data, profile, info, encounter, time) : getDPPt(data, profile, info, time);
            for (const auto &area : areas)
            {
                if (area.getEncounter() == encounter)
                {
                    encounters.append(area);
                }
            }
        }

        return encounters;
    }
}
