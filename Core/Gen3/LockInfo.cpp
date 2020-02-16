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

#include "LockInfo.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Enum/ShadowType.hpp>
#include <QFile>

LockInfo::LockInfo(u8 nature, u8 genderLower, u8 genderUpper)
{
    this->nature = nature;
    this->genderLower = genderLower;
    this->genderUpper = genderUpper;
    free = nature == 255 && genderLower == 255 && genderUpper == 255;
}

bool LockInfo::compare(u32 pid) const
{
    if (free)
    {
        return true;
    }

    u8 gender = pid & 255;
    return gender >= genderLower && gender <= genderUpper && nature == (pid % 25);
}

ShadowTeam::ShadowTeam(const QVector<LockInfo> &locks, ShadowType type)
{
    this->locks = locks;
    this->type = type;
}

LockInfo ShadowTeam::getLock(u8 index) const
{
    return locks.at(index);
}

ShadowType ShadowTeam::getType() const
{
    return type;
}

int ShadowTeam::getSize() const
{
    return locks.size();
}

QVector<ShadowTeam> ShadowTeam::loadShadowTeams(Method version)
{
    QVector<ShadowTeam> shadowTeams;
    QString path = version == Method::XD ? ":/encounters/gales.bin" : ":/encounters/colo.bin";

    QByteArray data;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        data = file.readAll();
        file.close();
    }

    u16 offset = 0;
    while (offset < data.size())
    {
        auto type = static_cast<ShadowType>(data.at(offset + 1));
        u8 size = static_cast<u8>(data.at(offset));
        QVector<LockInfo> locks;
        for (u8 i = 0; i < size; i++)
        {
            u8 nature = static_cast<u8>(data.at(offset + 2 + i * 3));
            u8 genderLower = static_cast<u8>(data.at(offset + 3 + i * 3));
            u8 genderUpper = static_cast<u8>(data.at(offset + 4 + i * 3));
            locks.append(LockInfo(nature, genderLower, genderUpper));
        }

        shadowTeams.append(ShadowTeam(locks, type));
        offset += data[offset] * 3 + 2;
    }

    return shadowTeams;
}
