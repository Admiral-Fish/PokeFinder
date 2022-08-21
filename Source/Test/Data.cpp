/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Data.hpp"
#include <QFile>

json readData(const std::string &file, const std::string &test, const std::string &category)
{
    json j;

    QFile f(QString(":/data/%1.json").arg(QString::fromStdString(file)));
    if (f.open(QIODevice::ReadOnly))
    {
        j = nlohmann::json::parse(f.readAll().toStdString(), nullptr, false);
        j = j[test][category];
    }

    return j;
}
