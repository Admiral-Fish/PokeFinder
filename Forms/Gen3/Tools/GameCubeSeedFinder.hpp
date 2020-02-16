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

#ifndef GAMECUBESEEDFINDER_HPP
#define GAMECUBESEEDFINDER_HPP

#include <Core/Util/Global.hpp>
#include <QWidget>

namespace Ui
{
    class GameCubeSeedFinder;
}

class GameCubeSeedFinder : public QWidget
{
    Q_OBJECT
public:
    explicit GameCubeSeedFinder(QWidget *parent = nullptr);
    ~GameCubeSeedFinder() override;

private:
    Ui::GameCubeSeedFinder *ui;
    QVector<u32> galeSeeds;
    QVector<u32> coloSeeds;
    u8 galesRound;
    u8 coloRound;

    void updateGales(const QVector<u32> &seeds);
    void updateGalesProgress(int progress);
    void updateColo(const QVector<u32> &seeds);
    void updateColoProgress(int progress);
    void updateChannel(const QVector<u32> &seeds);
    void updateChannelProgress(int progress);

private slots:
    void galesSearch();
    void galesReset();
    void coloSearch();
    void coloReset();
    void channelSearch();
    void channelAdd();
    void channelRemove();
};

#endif // GAMECUBESEEDFINDER_HPP
