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

#ifndef PROFILESEARCHER5_HPP
#define PROFILESEARCHER5_HPP

#include <Core/Util/Global.hpp>
#include <QDate>
#include <QStandardItem>
#include <QTime>
#include <QVector>
#include <mutex>

enum Buttons : u16;
enum Game : u16;
enum Language : u8;
enum DSType : u8;

class ProfileSearcher5
{
public:
    ProfileSearcher5() = default;
    explicit ProfileSearcher5(const QDate &date, const QTime &time, int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount,
                              u16 minTimer0, u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version, Language language,
                              DSType dsType, u64 mac, Buttons keypress);
    virtual ~ProfileSearcher5();
    void startSearch(int threads, u8 minVFrame, u8 maxVFrame);
    void cancelSearch();
    QVector<QList<QStandardItem *>> getResults();
    int getProgress() const;

private:
    QDate date;
    QTime time;
    int minSeconds;
    int maxSeconds;
    u8 minVCount;
    u8 maxVCount;
    u16 minTimer0;
    u16 maxTimer0;
    u8 minGxStat;
    u8 maxGxStat;
    bool softReset;
    Game version;
    Language language;
    DSType dsType;
    u64 mac;
    Buttons keypress;

    bool searching;
    int progress;
    QVector<QList<QStandardItem *>> results;
    std::mutex resultMutex;
    std::mutex progressMutex;

    void search(u8 vframeStart, u8 vframeEnd);

protected:
    virtual bool valid(u64 seed) = 0;
};

class ProfileIVSearcher5 : public ProfileSearcher5
{
public:
    ProfileIVSearcher5() = default;
    explicit ProfileIVSearcher5(const QVector<u8> &minIVs, const QVector<u8> &maxIVs, const QDate &date, const QTime &time, int minSeconds,
                                int maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0, u16 maxTimer0, u8 minGxStat, u8 maxGxStat,
                                bool softReset, Game version, Language language, DSType dsType, u64 mac, Buttons keypress);

private:
    QVector<u8> minIVs;
    QVector<u8> maxIVs;
    u8 offset;

    bool valid(u64 seed) override;
};

class ProfileNeedleSearcher5 : public ProfileSearcher5
{
public:
    ProfileNeedleSearcher5() = default;
    explicit ProfileNeedleSearcher5(const QVector<u8> &needles, bool unovaLink, bool memoryLink, const QDate &date, const QTime &time,
                                    int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0, u16 maxTimer0, u8 minGxStat,
                                    u8 maxGxStat, bool softReset, Game version, Language language, DSType dsType, u64 mac,
                                    Buttons keypress);

private:
    QVector<u8> needles;
    bool unovaLink;
    bool memoryLink;
    bool game;

    bool valid(u64 seed) override;
};

#endif // PROFILESEARCHER5_HPP
