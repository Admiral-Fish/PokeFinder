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

#include <QApplication>
#include <QFile>
#include <Forms/MainWindow.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("PokeFinder");
    a.setOrganizationName("PokeFinder Team");

    QString option = argc > 1 ? QString(argv[1]) : QString();

    QFile style(":/qdarkstyle/style.qss");
    if (style.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream ts(&style);
        if (option != "default")
        {
            a.setStyleSheet(ts.readAll());
        }
        style.close();
    }

    MainWindow w;
    w.show();
    w.raise();

    return a.exec();
}
