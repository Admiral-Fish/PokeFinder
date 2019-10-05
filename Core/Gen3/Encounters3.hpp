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

#ifndef ENCOUNTERS3_HPP
#define ENCOUNTERS3_HPP

#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Profile3.hpp>

namespace PokeFinderCore
{

    class Encounters3
    {

    public:
        Encounters3(Encounter type, const Profile3 &profile);
        QVector<EncounterArea3> getEncounters() const;

    private:
        QVector<Pokemon> pokemon;
        Encounter type;
        Profile3 profile;

        QByteArrayList getData() const;
        QVector<EncounterArea3> getArea(const QByteArray &data) const;
        u16 getValue(const QByteArray &data, int offset, int length) const;

    };

}

#endif // ENCOUNTERS3_HPP
