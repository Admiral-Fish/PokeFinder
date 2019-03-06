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

#include "Utilities.hpp"

u16 Utilities::calcGen3Seed(const QDate &time, u32 h, u32 m)
{
    u32 d = QDate(time.year() == 2000 ? 1999 : 2000, 12, 31).daysTo(time);

    u32 seed = 1440 * d + 960 * (h / 10) + 60 * (h % 10) + 16 * (m / 10) + m % 10;
    return (seed >> 16) ^ (seed & 0xFFFF);
}

u32 Utilities::calcGen4Seed(const QDateTime &dateTime, u32 delay)
{
    QDate date = dateTime.date();
    QTime time = dateTime.time();

    u8 ab = date.month() * date.day() + time.minute() + time.second();
    u8 cd = time.hour();

    return ((ab << 24) | (cd << 16)) + delay;
}

bool Utilities::shiny(u32 pid, u16 tid, u16 sid)
{
    return ((pid & 0xFFFF) ^ (pid >> 16) ^ tid ^ sid) < 8;
}

QString Utilities::coinFlips(u32 seed, int flips)
{
    QStringList coins;

    MersenneTwister rng(seed);

    for (int i = 0; i < flips; i++)
    {
        coins.append((rng.nextUInt() & 1) == 0 ? "T" : "H");
    }

    return coins.join(", ");
}

QString Utilities::getCalls(u32 seed, int num, const HGSSRoamer &info)
{
    QString calls = "";

    int skips = info.getSkips();

    if (skips > 0)
    {
        calls += "(";
    }

    PokeRNG rng(seed);

    for (int i = 0; i < num + skips; i++)
    {
        u8 call = rng.nextUShort() % 3;

        calls += call == 0 ? "E" : call == 1 ? "K" : "P";

        if (i != (num + skips - 1))
        {
            if (skips != 0 && skips == i + 1)
            {
                calls += " skipped)  ";
            }
            else
            {
                calls += ", ";
            }
        }
    }
    return calls;
}

void Utilities::outputModelTXT(QAbstractTableModel *model)
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save Output to TXT"), QDir::currentPath(), QObject::tr("Text File (*.txt);;All Files (*)"));

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QString textData = "";
        int rows = model->rowCount();
        int columns = model->columnCount();

        for (int i = 0; i < columns; i++)
        {
            textData += model->headerData(i, Qt::Horizontal, 0).toString();
            if (i != columns - 1)
            {
                textData += "\t";
            }
        }
        textData += "\n";

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                QString entry = model->data(model->index(i, j)).toString();
                textData += (entry.isEmpty() ? "-" : entry);
                if (i != columns - 1)
                {
                    textData += "\t";
                }
            }
            textData += "\n";
        }

        QTextStream out(&file);
        out << textData;
        file.close();
    }
}

void Utilities::outputModelCSV(QAbstractTableModel *model)
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save Output to CSV"), QDir::currentPath(), QObject::tr("CSV File (*.csv);;All Files (*)"));

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QString textData = "";
        int rows = model->rowCount();
        int columns = model->columnCount();

        for (int i = 0; i < columns; i++)
        {
            textData += model->headerData(i, Qt::Horizontal, 0).toString();
            if (i != columns - 1)
            {
                textData += ",";
            }
        }

        textData += "\n";

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                QString entry = model->data(model->index(i, j)).toString();
                textData += (entry.isEmpty() ? "-" : entry);
                if (j != columns - 1)
                {
                    textData += ",";
                }
            }
            textData += "\n";
        }

        QTextStream out(&file);
        out << textData;
        file.close();
    }
}
