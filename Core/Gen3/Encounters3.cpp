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

#include "Encounters3.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/Slot.hpp>
#include <QFile>

namespace Encounters3
{
    namespace
    {
        QByteArrayList getData(Game game)
        {
            QString path;
            int size = 120;
            switch (game)
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

        u16 getValue(const QByteArray &data, int offset, int length)
        {
            return data.mid(offset, length).toHex().toUShort(nullptr, 16);
        }

        QVector<EncounterArea3> getArea(const QByteArray &data, const QVector<PersonalInfo> &info)
        {
            QVector<EncounterArea3> encounters;

            u8 location = static_cast<u8>(data.at(0));

            if (data.at(1) == 1 || data.at(1) == 2)
            {
                QVector<Slot> grass;
                for (u8 i = 0; i < 12; i++)
                {
                    u8 level = static_cast<u8>(data.at(4 + i * 3));
                    u16 specie = getValue(data, 5 + i * 3, 2);
                    grass.append(Slot(specie, level, info.at(specie)));
                }

                u8 val = static_cast<u8>(data.at(1));
                encounters.append(EncounterArea3(location, val == 1 ? Encounter::Grass : Encounter::SafariZone, grass));
            }
            if (data.at(2) == 1)
            {
                QVector<Slot> rock;
                for (u8 i = 0; i < 5; i++)
                {
                    u8 minLevel = static_cast<u8>(data.at(40 + i * 4));
                    u8 maxLevel = static_cast<u8>(data.at(41 + i * 4));
                    u16 specie = getValue(data, 42 + i * 4, 2);
                    rock.append(Slot(specie, minLevel, maxLevel, info.at(specie)));
                }
                encounters.append(EncounterArea3(location, Encounter::RockSmash, rock));
            }
            if (data.at(3) == 1)
            {
                QVector<Slot> surf;
                for (u8 i = 0; i < 5; i++)
                {
                    u8 minLevel = static_cast<u8>(data.at(60 + i * 4));
                    u8 maxLevel = static_cast<u8>(data.at(61 + i * 4));
                    u16 specie = getValue(data, 62 + i * 4, 2);
                    surf.append(Slot(specie, minLevel, maxLevel, info.at(specie)));
                }
                encounters.append(EncounterArea3(location, Encounter::Surfing, surf));

                QVector<Slot> old;
                for (u8 i = 0; i < 2; i++)
                {
                    u8 minLevel = static_cast<u8>(data.at(80 + i * 4));
                    u8 maxLevel = static_cast<u8>(data.at(81 + i * 4));
                    u16 specie = getValue(data, 82 + i * 4, 2);
                    old.append(Slot(specie, minLevel, maxLevel, info.at(specie)));
                }
                encounters.append(EncounterArea3(location, Encounter::OldRod, old));

                QVector<Slot> good;
                for (u8 i = 0; i < 3; i++)
                {
                    u8 minLevel = static_cast<u8>(data.at(88 + i * 4));
                    u8 maxLevel = static_cast<u8>(data.at(89 + i * 4));
                    u16 specie = getValue(data, 90 + i * 4, 2);
                    good.append(Slot(specie, minLevel, maxLevel, info.at(specie)));
                }
                encounters.append(EncounterArea3(location, Encounter::GoodRod, good));

                QVector<Slot> super;
                for (u8 i = 0; i < 5; i++)
                {
                    u8 minLevel = static_cast<u8>(data.at(100 + i * 4));
                    u8 maxLevel = static_cast<u8>(data.at(101 + i * 4));
                    u16 specie = getValue(data, 102 + i * 4, 2);
                    super.append(Slot(specie, minLevel, maxLevel, info.at(specie)));
                }
                encounters.append(EncounterArea3(location, Encounter::SuperRod, super));
            }
            return encounters;
        }
    }

    QVector<EncounterArea3> getEncounters(Encounter encounter, const Profile3 &profile)
    {
        QVector<EncounterArea3> encounterAreas;
        QVector<PersonalInfo> info = PersonalInfo::loadPersonal(3);

        for (const auto &data : getData(profile.getVersion()))
        {
            for (const auto &encounterArea : getArea(data, info))
            {
                if (encounterArea.getEncounter() == encounter)
                {
                    encounterAreas.append(encounterArea);
                }
            }
        }

        return encounterAreas;
    }
}
