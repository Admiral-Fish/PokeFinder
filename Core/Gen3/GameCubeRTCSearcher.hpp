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

#ifndef GAMECUBERTCSEARCHER_HPP
#define GAMECUBERTCSEARCHER_HPP

#include <QDateTime>
#include <QMutex>
#include <QObject>
#include <QStandardItem>
#include <Core/Util/Global.hpp>

namespace PokeFinderCore
{

    class GameCubeRTCSearcher : public QObject
    {
        Q_OBJECT

    signals:
        void finished();
        void result(QList<QStandardItem *>);

    public:
        GameCubeRTCSearcher(u32 initialSeed, u32 targetSeed, u32 minFrame, u32 maxFrame);
        void startSearch();

    public slots:
        void cancelSearch();

    private:
        const QDateTime date = QDateTime(QDate(2000, 1, 1), QTime(0, 0));
        u32 initialSeed, targetSeed;
        u32 minFrame, maxFrame;
        bool searching, cancel;

        void search();
    };

}

#endif // GAMECUBERTCSEARCHER_HPP
