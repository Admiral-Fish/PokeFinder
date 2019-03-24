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

#include <QMutex>
#include <QThread>
#include <Core/Gen4/Searcher4.hpp>

class IVSearcher4 : public QThread
{
    Q_OBJECT

public:
    IVSearcher4(const Searcher4 &searcher, const QVector<u8> &min, const QVector<u8> &max);
    void run() override;
    int currentProgress() const;
    QVector<Frame4> getResults();

public slots:
    void cancelSearch();

private:
    Searcher4 searcher;
    QVector<u8> min;
    QVector<u8> max;

    QMutex mutex;
    QVector<Frame4> results;
    bool cancel;
    int progress;

};

#endif // IVSEARCHER4_HPP
