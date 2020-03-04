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

#ifndef STATIONARYSEARCHER5_HPP
#define STATIONARYSEARCHER5_HPP

#include <Core/Gen5/Generators/StationaryGenerator5.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <QDate>
#include <mutex>

class StationarySearcher5
{
public:
    StationarySearcher5() = default;
    explicit StationarySearcher5(const StationaryGenerator5 &ivGenerator, const StationaryGenerator5 &pidGenerator, const Profile5 &profile,
                                 const QVector<QHash<u32, u32>> &ivMap, bool includePID);
    void startSearch(int threads, QDate start, const QDate &end);
    void cancelSearch();
    QVector<StationaryFrame5> getResults();
    int getProgress() const;

private:
    StationaryGenerator5 ivGenerator;
    StationaryGenerator5 pidGenerator;
    Profile5 profile;
    QVector<QHash<u32, u32>> ivMap;
    bool includePID;
    bool fastSearch;

    bool searching;
    int progress;
    QVector<StationaryFrame5> results;
    std::mutex resultMutex;
    std::mutex progressMutex;

    void search(const QDate &start, const QDate &end);
};

#endif // STATIONARYSEARCHER5_HPP
