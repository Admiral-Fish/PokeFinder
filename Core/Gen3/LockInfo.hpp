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

#ifndef LOCKINFO_HPP
#define LOCKINFO_HPP

#include <Core/Util/Global.hpp>
#include <QVector>

enum Method : u8;
enum ShadowType : u8;

class LockInfo
{
public:
    LockInfo() = default;
    LockInfo(u8 nature, u8 genderLower, u8 genderUpper);
    bool compare(u32 pid) const;

private:
    u8 genderUpper;
    u8 genderLower;
    u8 nature;
    bool free;
};

class ShadowTeam
{

public:
    ShadowTeam() = default;
    ShadowTeam(const QVector<LockInfo> &locks, ShadowType type);
    LockInfo getLock(u8 index) const;
    ShadowType getType() const;
    int getSize() const;
    static QVector<ShadowTeam> loadShadowTeams(Method version);

private:
    QVector<LockInfo> locks;
    ShadowType type;
};

#endif // LOCKINFO_HPP
