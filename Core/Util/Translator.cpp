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

#include "Translator.hpp"
#include <Core/Enum/Game.hpp>
#include <QFile>
#include <QTextStream>

namespace
{
    QString language;
    QStringList characteristics;
    QStringList natures;
    QStringList hiddenPowers;
    QStringList species;
    const QStringList genders = { "♂", "♀", "-" };

    QStringList readFile(const QString &name)
    {
        QFile file(name);

        QStringList input;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream ts(&file);
            ts.setCodec("UTF-8");

            while (!ts.atEnd())
            {
                input << ts.readLine();
            }
            file.close();
        }
        return input;
    }
}

namespace Translator
{
    void init(const QString &locale)
    {
        language = locale;
        characteristics = readFile(QString(":/text/characteristic_%1.txt").arg(language));
        natures = readFile(QString(":/text/natures_%1.txt").arg(language));
        hiddenPowers = readFile(QString(":/text/powers_%1.txt").arg(language));
        species = readFile(QString(":/text/species_%1.txt").arg(language));
    }

    QStringList getCharacteristic()
    {
        return characteristics;
    }

    QStringList getNatures()
    {
        return natures;
    }

    QString getNature(u8 nature)
    {
        return natures.at(nature);
    }

    QStringList getHiddenPowers()
    {
        return hiddenPowers;
    }

    QString getHiddenPower(u8 power)
    {
        return hiddenPowers.at(power);
    }

    QString getSpecies(u16 specie)
    {
        return species.at(specie - 1);
    }

    QStringList getSpecies(const QVector<u16> &nums)
    {
        QStringList s;
        for (u16 num : nums)
        {
            s.append(species.at(num - 1));
        }
        return s;
    }

    QString getGender(u8 gender)
    {
        return genders.at(gender);
    }

    QStringList getLocations(const QVector<u8> &nums, Game game)
    {
        QString version;
        if (game & Game::FRLG)
        {
            version = "frlg";
        }
        else if (game & Game::RSE)
        {
            version = "rse";
        }
        else if (game & Game::DPPt)
        {
            version = "dppt";
        }
        else
        {
            version = "hgss";
        }

        QStringList strings = readFile(QString(":/text/%1_%2.txt").arg(version, language));
        QMap<int, QString> map;
        for (const QString &string : strings)
        {
            QStringList entry = string.split(",");
            map.insert(entry.at(0).toInt(), entry.at(1));
        }

        QStringList locations;
        for (const u8 &num : nums)
        {
            locations.append(map[num]);
        }

        return locations;
    }
}
