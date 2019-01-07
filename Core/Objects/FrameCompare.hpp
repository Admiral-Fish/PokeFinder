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

#ifndef FRAMECOMPARE_HPP
#define FRAMECOMPARE_HPP

#include <QVector>
#include <Core/Objects/Frame.hpp>
#include <Core/Objects/Nature.hpp>

class FrameCompare
{

public:
    FrameCompare() = default;
    FrameCompare(const QVector<u8> &min, const QVector<u8> &max, int genderIndex, int genderRatioIndex, int abilityIndex,
                 const QVector<bool> &nature, const QVector<bool> &power, bool onlyShiny, bool skipCompare);
    FrameCompare(const QVector<u8> &min, const QVector<u8> &max, int genderIndex, int genderRatioIndex, int abilityIndex,
                 const QVector<bool> &nature, const QVector<bool> &power, bool onlyShiny, bool skipCompare, const QVector<bool> &encounter);
    FrameCompare(int genderIndex, int genderRatioIndex, int abilityIndex, QVector<bool> nature, bool onlyShiny);
    FrameCompare(const QVector<u8> &min, const QVector<u8> &max, const QVector<bool> &power);
    bool comparePID(const Frame &frame) const;
    bool compareIVs(const Frame &frame) const;
    bool compareNature(const Frame &frame) const;
    bool compareHiddenPower(const Frame &frame) const;
    bool compareSlot(const Frame &frame) const;
    bool compareGender(const Frame &frame) const;
    bool compareFrame(const Frame &frame) const;
    u8 getGenderRatio() const;

private:
    QVector<u8> min;
    QVector<u8> max;
    u16 gender;
    u8 genderRatio;
    u8 ability;
    QVector<bool> natures;
    QVector<bool> powers;
    QVector<bool> encounterSlots;
    bool shiny;
    bool skip;

};

#endif // FRAMECOMPARE_HPP
