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

#ifndef ENCOUNTERS4_HPP
#define ENCOUNTERS4_HPP

#include <QByteArrayList>
#include <Core/Parents/Pokemon.hpp>
#include <Core/Gen4/Profile4.hpp>

namespace PokeFinderCore
{
    class EncounterArea4;
    class Slot;
    enum Encounter : u8;

    class Encounters4
    {
    public:
        Encounters4(Encounter type, int time, const Profile4 &profile);
        QVector<EncounterArea4> getEncounters() const;

    private:
        Profile4 profile;
        Encounter type;
        int time; // 0: Morning, 1: Day, 2: Night
        // Dual is used in DPPt
        // Radio 0: None, 1: Hoenn, 2: Sinnoh, For HGSS
        QVector<Pokemon> pokemon;

        QByteArrayList getData() const;
        QVector<EncounterArea4> getHGSS(const QByteArray &data) const;
        QVector<EncounterArea4> getDPPt(const QByteArray &data) const;
        void modifyRadio(QVector<Slot> &mons, const QByteArray &data) const;
        void modifyTime(QVector<Slot> &mons, const QByteArray &data) const;
        void modifyDual(QVector<Slot> &mons, const QByteArray &data) const;
        void modifyRadar(QVector<Slot> &mons, const QByteArray &data) const;
        void modifySwarmHGSS(QVector<Slot> &mons, const QByteArray &data) const;
        void modifySwarmDPPt(QVector<Slot> &mons, const QByteArray &data) const;
        u16 getValue(const QByteArray &data, int offset, int length) const;
    };
}

#endif // ENCOUNTERS4_HPP
