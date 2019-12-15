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

#include "Translator.hpp"
#include <QFile>
#include <QSettings>
#include <QTextStream>

static QStringList characteristics;
static QStringList natures;
static QStringList frameNatures;
static QStringList powers;
static QStringList species;

QStringList readFile(QString name)
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

namespace PokeFinderCore
{
    void Translator::init()
    {
        QSettings setting;
        QString locale = setting.value("settings/locale", "en").toString().left(2);

        characteristics = readFile(QString(":/text/characteristic_%1.txt").arg(locale));
        frameNatures = readFile(QString(":/text/natures_%1.txt").arg(locale));
        for (int i : { 3, 5, 2, 20, 23, 11, 8, 13, 1, 16, 15, 14, 4, 17, 19, 7, 22, 10, 21, 9, 18, 6, 0, 24, 12 })
        {
            natures.append(frameNatures.at(i));
        }
        powers = readFile(QString(":/text/powers_%1.txt").arg(locale));
        species = readFile(QString(":/text/species_%1.txt").arg(locale));
    }

    QStringList Translator::getCharacteristic() { return characteristics; }

    QStringList Translator::getNatures() { return natures; }

    QString Translator::getNature(u8 nature) { return frameNatures.at(nature); }

    QStringList Translator::getFrameNatures() { return frameNatures; }

    QStringList Translator::getPowers() { return powers; }

    QString Translator::getPower(u8 power) { return powers.at(power); }

    QString Translator::getSpecies(u16 specie) { return species.at(specie - 1); }

    QStringList Translator::getSpecies(const QVector<u16> &nums)
    {
        QStringList s;
        for (u16 num : nums)
        {
            s.append(species.at(num - 1));
        }
        return s;
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
}
