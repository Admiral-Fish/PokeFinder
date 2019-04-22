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

GameCubeSeedSearcher::GameCubeSeedSearcher(Game version)
{
    this->version = version;
}

QVector<u32> GameCubeSeedSearcher::getInitialSeeds(u8 num1, u8 num2)
{
    QVector<u32> seeds;

    QString path = version == Game::Gales ? "xd.precalc" : "colo.precalc";
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {

    }
    return seeds;
}

void GameCubeSeedSearcher::searchSeeds(const QVector<u32> &seeds)
{
    this->seeds = seeds;

    if (version == Game::Gales)
    {
        QtConcurrent::run([ = ] { searchGales(); });
    }
    else
    {
        QtConcurrent::run([ = ] { searchColo(); });
    }
}

void GameCubeSeedSearcher::searchGales()
{

}

void GameCubeSeedSearcher::searchColo()
{

}
