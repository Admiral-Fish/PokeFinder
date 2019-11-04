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

#ifndef IVSEARCHER3_HPP
#define IVSEARCHER3_HPP

#include <Core/Gen3/Searcher3.hpp>
#include <QMutex>
#include <QObject>

namespace PokeFinderCore
{
    class IVSearcher3 : public QObject
    {
        Q_OBJECT
    signals:
        void finished();
        void updateProgress(const QVector<Frame3> &, int progress);

    public:
        IVSearcher3(const Searcher3 &searcher, const QVector<u8> &min, const QVector<u8> &max);
        void startSearch();

    public slots:
        void cancelSearch();

    private:
        Searcher3 searcher;
        QVector<u8> min, max;
        QVector<Frame3> results;
        bool searching, cancel;
        int progress;
        QMutex mutex;

        void search();
        QVector<Frame3> getResults();
    };
}

#endif // IVSEARCHER3_HPP
