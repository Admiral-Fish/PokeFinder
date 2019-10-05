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

#ifndef EGGSEARCHER4_HPP
#define EGGSEARCHER4_HPP

#include <QMutex>
#include <QObject>
#include <Core/Gen4/Egg4.hpp>

namespace PokeFinderCore
{

    class EggSearcher4 : public QObject
    {
        Q_OBJECT

    signals:
        void finished();
        void updateProgress(const QVector<Frame4> &, int);

    public:
        EggSearcher4(const Egg4 &generatorIV, const Egg4 &generatorPID, const FrameCompare &compare, u32 minDelay, u32 maxDelay, int type);
        void startSearch();

    public slots:
        void cancelSearch();

    private:
        Egg4 generatorIV;
        Egg4 generatorPID;
        FrameCompare compare;
        u32 minDelay, maxDelay;
        QMutex mutex;
        QVector<Frame4> results;
        bool searching, cancel;
        int progress, type;

        void search();
        QVector<Frame4> getResults();

    };

}

#endif // EGGSEARCHER4_HPP
