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

#ifndef IVSEARCHER4_HPP
#define IVSEARCHER4_HPP

#include <Core/Gen4/Frame4.hpp>
#include <Core/Gen4/Searcher4.hpp>
#include <QMutex>
#include <QObject>

namespace PokeFinderCore
{
    class IVSearcher4 : public QObject
    {
        Q_OBJECT
    signals:
        void finished();
        void updateProgress(const QVector<PokeFinderCore::Frame4> &, int);

    public:
        IVSearcher4(const Searcher4 &searcher, const QVector<u8> &min, const QVector<u8> &max);
        void startSearch();

    public slots:
        void cancelSearch();

    private:
        Searcher4 searcher;
        QVector<u8> min, max;
        QMutex mutex;
        QVector<Frame4> results;
        bool searching, cancel;
        int progress;

        void search();
        QVector<Frame4> getResults();
    };
}

#endif // IVSEARCHER4_HPP
