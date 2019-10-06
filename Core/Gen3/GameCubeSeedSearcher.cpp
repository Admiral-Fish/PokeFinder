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

#include <QFile>
#include <QtConcurrent>
#include "GameCubeSeedSearcher.hpp"
#include <Core/RNG/LCRNG.hpp>

constexpr u16 galesHPStat[10][2] =
{
    { 322, 340 }, { 310, 290 }, { 210, 620 }, { 320, 230 }, { 310, 310 },
    { 290, 310 }, { 290, 270 }, { 290, 250 }, { 320, 270 }, { 270, 230 }
};

constexpr u8 coloNatures[8][6] =
{
    { 0x16, 0x15, 0x0f, 0x13, 0x04, 0x04 },
    { 0x0b, 0x08, 0x01, 0x10, 0x10, 0x0C },
    { 0x02, 0x10, 0x0f, 0x12, 0x0f, 0x03 },
    { 0x10, 0x13, 0x03, 0x16, 0x03, 0x18 },
    { 0x11, 0x10, 0x0f, 0x13, 0x05, 0x04 },
    { 0x0f, 0x11, 0x01, 0x03, 0x13, 0x03 },
    { 0x01, 0x08, 0x03, 0x01, 0x03, 0x03 },
    { 0x03, 0x0a, 0x0f, 0x03, 0x0f, 0x03 }
};

constexpr u8 coloGenders[8][6] =
{
    { 0, 1, 1, 0, 0, 1 },
    { 2, 1, 0, 0, 1, 0 },
    { 0, 1, 0, 1, 0, 1 },
    { 2, 1, 1, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 1 },
    { 2, 1, 2, 0, 2, 1 },
    { 2, 0, 0, 1, 1, 0 },
    { 1, 0, 1, 0, 1, 0 }
};

constexpr u8 coloGenderRatios [8][6] =
{
    { 0x1f, 0x7f, 0x7f, 0x7f, 0xbf, 0x7f },
    { 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f },
    { 0x1f, 0x3f, 0x7f, 0x7f, 0x7f, 0x7f },
    { 0xff, 0xbf, 0x7f, 0x7f, 0x1f, 0x7f },
    { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x7f },
    { 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f },
    { 0xff, 0x1f, 0x3f, 0x7f, 0x7f, 0x3f },
    { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f }
};

namespace PokeFinderCore
{
    GameCubeSeedSearcher::GameCubeSeedSearcher(Game version, const QVector<u32> &criteria)
    {
        this->version = version;
        this->criteria = criteria;
        searching = false;
        cancel = false;
        progress = 0;

        connect(this, &GameCubeSeedSearcher::finished, this, [ = ]
        {
            searching = false;
            QTimer::singleShot(1000, this, &GameCubeSeedSearcher::deleteLater);
        });
    }

    QVector<u32> GameCubeSeedSearcher::getInitialSeeds(u8 num1, u8 num2)
    {
        QVector<u32> seeds;

        QString path;
        u8 nbr, index;
        if (version & Game::Gales)
        {
            path = "xd.precalc";
            nbr = 25;
            index = num1 * 5 + num2;
        }
        else
        {
            path = "colo.precalc";
            nbr = 24;
            index = num1 + 8 * num2;
        }

        QFile file(path);
        if (file.open(QIODevice::ReadOnly))
        {
            auto *seedSizes = new u32[nbr];
            file.read(reinterpret_cast<char *>(seedSizes), sizeof(u32) * nbr);

            u32 offset = 0;
            for (u8 i = 0; i < index; i++)
            {
                offset += seedSizes[i];
            }

            seeds.resize(seedSizes[index]);
            file.seek(sizeof(u32) * offset + sizeof(u32) * nbr);
            file.read(reinterpret_cast<char *>(seeds.data()), sizeof(u32) * seedSizes[index]);

            delete[] seedSizes;
            file.close();
        }

        emit finished();
        return seeds;
    }

    void GameCubeSeedSearcher::startSearch(const QVector<u32> &seeds)
    {
        if (!searching)
        {
            this->seeds = seeds;
            progress = 0;
            searching = true;
            cancel = false;

            auto *timer = new QTimer(this);
            connect(this, &GameCubeSeedSearcher::finished, timer, &QTimer::stop);
            connect(timer, &QTimer::timeout, this, [ = ] {  emit updateProgress(progress); });
            timer->start(1000);

            QtConcurrent::run([ = ] { search(); });
        }
    }

    void GameCubeSeedSearcher::addHPCriteria(const QVector<u32> &criteria)
    {
        this->criteria.append(criteria);
    }

    void GameCubeSeedSearcher::cancelSearch()
    {
        cancel = true;
    }

    void GameCubeSeedSearcher::search()
    {
        QVector<u32> newSeeds;
        for (u32 seed : seeds)
        {
            if (cancel)
            {
                emit finished();
                return;
            }

            if (version & Game::Gales)
            {
                if (generateTeamGales(seed))
                {
                    newSeeds.append(seed);
                }
            }
            else
            {
                if (generateTeamColo(seed))
                {
                    newSeeds.append(seed);
                }
            }
            progress++;
        }

        std::sort(newSeeds.begin(), newSeeds.end());
        auto unique = std::unique(newSeeds.begin(), newSeeds.end());
        newSeeds.erase(unique, newSeeds.end());

        emit updateProgress(progress);
        emit outputSeeds(newSeeds);
        emit finished();
    }

    bool GameCubeSeedSearcher::generateTeamGales(u32 &seed)
    {
        XDRNG rng(seed);
        rng.nextUInt();

        u8 playerIndex = rng.nextUShort() % 5;
        if (playerIndex != criteria.at(0))
        {
            return false;
        }

        u8 enemyIndex = rng.nextUShort() % 5;
        if (enemyIndex != criteria.at(1))
        {
            return false;
        }
        rng.nextUInt();

        u16 tid = rng.nextUShort();
        u16 sid = rng.nextUShort();
        u16 tsv = (tid ^ sid) >> 3;
        for (u8 i = 0; i < 2; i++)
        {
            u8 hpIV = rng.nextUShort(2) & 31;
            seed = rng.nextUInt(1);
            generatePokemonGales(seed, tsv);
            QVector<u8> evs = generateEVs(seed);
            u16 hp = evs.at(0) / 4 + hpIV + galesHPStat[enemyIndex + 5][i];
            if (hp != criteria.at(4 + i))
            {
                return false;
            }
            rng.setSeed(seed);
        }
        rng.nextUInt();

        tid = rng.nextUShort();
        sid = rng.nextUShort();
        tsv = (tid ^ sid) >> 3;
        for (u8 i = 0; i < 2; i++)
        {
            u8 hpIV = rng.nextUShort(2) & 31;
            seed = rng.nextUInt(1);
            generatePokemonGales(seed, tsv);
            QVector<u8> evs = generateEVs(seed);
            u16 hp = evs.at(0) / 4 + hpIV + galesHPStat[playerIndex][i];
            if (hp != criteria.at(2 + i))
            {
                return false;
            }
            rng.setSeed(seed);
        }

        return true;
    }

    bool GameCubeSeedSearcher::generateTeamColo(u32 &seed)
    {
        XDRNG rng(seed);

        u8 enemyIndex = rng.nextUShort() & 7;
        u8 playerIndex;
        do
        {
            playerIndex = rng.nextUShort() & 7;
        }
        while (enemyIndex == playerIndex);

        if (playerIndex != criteria.at(0))
        {
            return false;
        }

        u16 tid = rng.nextUShort();
        u16 sid = rng.nextUShort();
        u16 tsv = (tid ^ sid) >> 3;
        for (u8 i = 0; i < 6; i++)
        {
            u16 high = rng.nextUShort();
            u16 low = rng.nextUShort();
            u32 pid = (high << 16) | (low);

            seed = rng.nextUInt(2);
            generatePokemonColo(seed, tsv, pid, coloNatures[enemyIndex][i], coloGenders[enemyIndex][i], coloGenderRatios[enemyIndex][i]);
            rng.setSeed(seed);
        }

        u8 playerName = rng.nextUShort() % 3;
        if (playerName != criteria.at(1))
        {
            return false;
        }

        tid = rng.nextUShort();
        sid = rng.nextUShort();
        tsv = (tid ^ sid) >> 3;
        for (u8 i = 0; i < 6; i++)
        {
            u16 high = rng.nextUShort();
            u16 low = rng.nextUShort();
            u32 pid = (high << 16) | (low);

            seed = rng.nextUInt(2);
            generatePokemonColo(seed, tsv, pid, coloNatures[playerIndex][i], coloGenders[playerIndex][i], coloGenderRatios[playerIndex][i]);
            rng.setSeed(seed);
        }

        return true;
    }

    void GameCubeSeedSearcher::generatePokemonGales(u32 &seed, u16 tsv)
    {
        u32 attempts = 3145728;
        XDRNG rng(seed);

        while (attempts > 0)
        {
            u16 pid1 = rng.nextUShort();
            u16 pid2 = rng.nextUShort();
            attempts--;

            u16 psv = (pid1 ^ pid2) >> 3;
            if (psv != tsv)
            {
                break;
            }
        }
        seed = rng.getSeed();
    }

    void GameCubeSeedSearcher::generatePokemonColo(u32 &seed, u16 tsv, u32 dummyPID, u8 nature, u8 gender, u8 genderRatio)
    {
        bool flag = false;
        XDRNG rng(seed);

        while (!flag)
        {
            u16 high = rng.nextUShort();
            u16 low = rng.nextUShort();
            u32 pid = (high << 16) | low;

            if (genderRatio != 0xFF && genderRatio != 0xFE && genderRatio != 0)
            {
                if (gender == 2)
                {
                    u8 dummyGender = genderRatio > (dummyPID & 0xFF) ? 1 : 0;
                    u8 pidGender = genderRatio > (pid & 0xFF) ? 1 : 0;
                    if (dummyGender != pidGender)
                    {
                        continue;
                    }
                }
                else
                {
                    u8 pidGender = genderRatio > (pid & 0xFF) ? 1 : 0;
                    if (pidGender != gender)
                    {
                        continue;
                    }
                }
            }

            if (pid % 25 != nature)
            {
                continue;
            }

            u16 psv = (high ^ low) >> 3;
            if (psv != tsv)
            {
                flag = true;
            }
        }
        seed = rng.getSeed();
    }

    QVector<u8> GameCubeSeedSearcher::generateEVs(u32 &seed)
    {
        QVector<u8> evs(6);
        XDRNG rng(seed);
        u16 sum = 0;

        for (u8 i = 0; i < 101; i++)
        {
            sum = 0;
            for (u8 j = 0; j < 6; j++)
            {
                evs[j] += rng.nextUShort() & 0xff;
                sum += evs.at(j);
            }

            if (sum == 510)
            {
                break;
            }

            if (sum <= 490)
            {
                continue;
            }

            if (sum >= 530)
            {
                if (i >= 100)
                {
                    continue;
                }
                evs.fill(0);
                continue;
            }

            break;
        }

        while (sum != 510)
        {
            for (u8 i = 0; i < 6; i++)
            {
                if (sum < 510)
                {
                    if (evs.at(i) < 255)
                    {
                        evs[i]++;
                        sum++;
                    }
                }
                else if (sum > 510)
                {
                    if (evs.at(i) != 0)
                    {
                        evs[i]--;
                        sum--;
                    }
                }
            }
        }

        seed = rng.getSeed();
        return evs;
    }
}
