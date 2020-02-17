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

#ifndef CHAINEDSIDCALC_HPP
#define CHAINEDSIDCALC_HPP

#include <Core/Util/Global.hpp>
#include <QVector>

class ChainedSIDCalc
{
public:
    explicit ChainedSIDCalc(u16 tid);
    void addEntry(const QVector<u8> &ivs, u8 nature, u8 ability, u8 gender);
    QVector<u16> getSIDs() const;

private:
    QVector<u16> sids;
    u16 tid;

    bool matchGender(u8 gender, u8 val) const;
};

#endif // CHAINEDSIDCALC_HPP
