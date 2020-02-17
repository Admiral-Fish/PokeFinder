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

#include <Core/Util/Translator.hpp>
#include <Forms/MainWindow.hpp>
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("PokeFinder");
    a.setOrganizationName("PokeFinder Team");

    QSettings setting;

    QString style = setting.value("settings/style", "dark").toString();
    if (style == "dark")
    {
        QFile file(":/qdarkstyle/style.qss");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream ts(&file);
            a.setStyleSheet(ts.readAll());
            file.close();
        }
    }

    QString locale = setting.value("settings/locale", "en").toString();
    if (locale == "zh_Hans_CN") // TODO: remove in a future version
    {
        locale = "zh";
        setting.setValue("settings/locale", locale);
    }
    Translator::init(locale);

    QTranslator translator;
    if (translator.load(QString(":/i18n/PokeFinder_%1.qm").arg(locale)))
    {
        QApplication::installTranslator(&translator);
    }

    MainWindow w;
    w.show();
    w.raise();

    return a.exec();
}
