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

#ifndef GAMECUBESEED_HPP
#define GAMECUBESEED_HPP

#include <QObject>
#include <QVector>
#include <Core/Util/Game.hpp>
#include <Core/Util/Global.hpp>

namespace PokeFinderCore
{

    class GameCubeSeedSearcher : public QObject
    {
        Q_OBJECT

    signals:
        void finished();
        void outputSeeds(QVector<u32> seeds);
        void updateProgress(int);

    public:
        GameCubeSeedSearcher(Game version, const QVector<u32> &criteria);
        QVector<u32> getInitialSeeds(u8 num1, u8 num2);
        void startSearch(const QVector<u32> &seeds);
        void addHPCriteria(const QVector<u32> &criteria);

    public slots:
        void cancelSearch();

    private:
        QVector<u32> seeds;
        QVector<u32> criteria;
        Game version;
        bool searching, cancel;
        int progress;

        void search();
        bool generateTeamGales(u32 &seed);
        bool generateTeamColo(u32 &seed);
        void generatePokemonGales(u32 &seed, u16 tsv);
        void generatePokemonColo(u32 &seed, u16 tsv, u32 dummyPID, u8 nature, u8 gender, u8 genderRatio);
        QVector<u8> generateEVs(u32 &seed);

    };

}

#endif // GAMECUBESEED_HPP
