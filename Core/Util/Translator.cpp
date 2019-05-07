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
#include <QSettings>
#include <QTextStream>
#include "Translator.hpp"

QStringList Translator::getCharacteristic()
{
    QStringList names;
    QSettings setting;
    QFile file(QString(":/text/characteristic_%1.txt").arg(setting.value("settings/locale", "en").toString()));
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");

        while (!ts.atEnd())
        {
            names.append(ts.readLine());
        }
        file.close();
    }
    return names;
}

QStringList Translator::getSpecies(const QVector<u16> &nums)
{
    QStringList species;

    QSettings setting;
    QFile file(QString(":/text/species_%1.txt").arg(setting.value("settings/locale", "en").toString()));

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");

        QStringList input;
        while (!ts.atEnd())
        {
            input << ts.readLine();
        }
        file.close();

        for (const u16 &x : nums)
        {
            species.append(input.at(x - 1));
        }
    }

    return species;
}

QStringList Translator::getLocationsGen3(const QVector<u8> &nums, Game game)
{
    QStringList locations;

    QString version;
    if (game & Game::FRLG)
    {
        version = "frlg";
    }
    else
    {
        version = "rse";
    }

    QSettings setting;
    QFile file(QString(":/text/%1_%2.txt").arg(version, setting.value("settings/locale", "en").toString()));

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");

        QMap<u8, QString> input;
        while (!ts.atEnd())
        {
            QStringList entry = ts.readLine().split(',');
            input[entry.at(0).toInt()] = entry.at(1);
        }
        file.close();

        for (const u8 &x : nums)
        {
            locations.append(input[x]);
        }
    }

    return locations;
}

QStringList Translator::getLocationsGen4(const QVector<u8> &nums, Game game)
{
    QStringList locations;

    QString version;
    if (game & Game::DPPt)
    {
        version = "dppt";
    }
    else
    {
        version = "hgss";
    }

    QSettings setting;
    QFile file(QString(":/text/%1_%2.txt").arg(version, setting.value("settings/locale", "en").toString()));

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");

        QMap<u8, QString> input;
        while (!ts.atEnd())
        {
            QStringList entry = ts.readLine().split(',');
            input[entry.at(0).toInt()] = entry.at(1);
        }
        file.close();

        for (const u8 &x : nums)
        {
            locations.append(input[x]);
        }
    }

    return locations;
}
