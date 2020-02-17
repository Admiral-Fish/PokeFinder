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

#ifndef IDFILTER_HPP
#define IDFILTER_HPP

#include <Core/Util/Global.hpp>
#include <QVector>

class IDFrame;

class IDFilter
{
public:
    IDFilter() = default;
    IDFilter(const QVector<u16> &tidFilter, const QVector<u16> &sidFilter, const QVector<u16> &tsvFilter);
    bool compare(const IDFrame &frame) const;

private:
    QVector<u16> tidFilter;
    QVector<u16> sidFilter;
    QVector<u16> tsvFilter;
};

#endif // IDFILTER_HPP
