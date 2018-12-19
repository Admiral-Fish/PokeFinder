/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Translator.hpp"

QStringList Translator::getSpecies(const QVector<u16> &nums)
{
    QStringList species;

    QSettings setting;
    QFile file(QString(":/text/species_%1.txt").arg(setting.value("locale", "en").toString()));

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");

        QStringList input;
        while (!ts.atEnd())
        {
            input << ts.readLine();
        }

        for (const u16 &x : nums)
        {
            species.append(input.at(x - 1));
        }

        file.close();
    }

    return species;
}

QStringList Translator::getLocationsGen3(const QVector<u8> &nums)
{
    QStringList locations;

    QSettings setting;
    QFile file(QString(":/text/rsefrlg_%1.txt").arg(setting.value("locale", "en").toString()));

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");

        QStringList input;
        while (!ts.atEnd())
        {
            input << ts.readLine();
        }

        for (const u8 &x : nums)
        {
            locations.append(input.at(x));
        }

        file.close();
    }

    return locations;
}

QStringList Translator::getLocationsGen4(const QVector<u8> &nums, Game game)
{
    QStringList locations;

    QString version;
    switch (game)
    {
        case Game::Diamond:
        case Game::Pearl:
        case Game::Platinum:
            version = "dppt";
            break;
        case Game::HeartGold:
        case Game::SoulSilver:
        default:
            version = "hgss";
            break;
    }

    QSettings setting;
    QFile file(QString(":/text/%1_%2.txt").arg(version, setting.value("locale", "en").toString()));

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");

        QStringList input;
        while (!ts.atEnd())
        {
            input << ts.readLine();
        }

        for (const u8 &x : nums)
        {
            locations.append(input.at(x));
        }

        file.close();
    }

    return locations;
}
