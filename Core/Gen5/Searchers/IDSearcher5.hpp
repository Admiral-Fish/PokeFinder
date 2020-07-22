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

#ifndef IDSEARCHER5_HPP
#define IDSEARCHER5_HPP

#include <Core/Gen5/Generators/IDGenerator5.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Util/Global.hpp>
#include <QDate>
#include <mutex>

class IDSearcher5
{
public:
    IDSearcher5() = default;
    explicit IDSearcher5(const IDGenerator5 &idGenerator, const Profile5 &profile, u32 pid, bool checkPID, bool save);
    void startSearch(int threads, QDate start, const QDate &end);
    void cancelSearch();
    QVector<IDState5> getResults();
    int getProgress() const;

private:
    IDGenerator5 idGenerator;
    Profile5 profile;
    u32 pid;
    bool checkPID;
    bool save;

    bool searching;
    int progress;
    QVector<IDState5> results;
    std::mutex resultMutex;
    std::mutex progressMutex;

    void search(const QDate &start, const QDate &end);
};

#endif // IDSEARCHER5_HPP
