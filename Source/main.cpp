/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/MainWindow.hpp>
#include <QApplication>
#include <QFile>
#include <QHeaderView>
#include <QSettings>
#include <QStandardPaths>
#include <QStyleHints>
#include <QThread>
#include <QTranslator>

/**
 * @brief Sets default settings for initial open or if settings get deleted
 *
 * @param setting Contains application settings
 */
void validateSettings(QSettings &setting)
{
    if (!setting.contains("profiles"))
    {
        QString profilePath = QString("%1/profiles.json").arg(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

        QFile f(profilePath);
        if (f.open(QIODevice::NewOnly | QIODevice::Text | QIODevice::WriteOnly))
        {
            f.write("{}");
        }

        setting.setValue("profiles", profilePath);
    }

    if (!setting.contains("style"))
    {
        setting.setValue("style", "auto");
    }

    if (!setting.contains("locale"))
    {
        setting.setValue("locale", "en");
    }

    if (!setting.contains("headerSize"))
    {
        setting.setValue("headerSize", QHeaderView::ResizeToContents);
    }

    if (!setting.contains("threads") || (setting.value("threads").toInt() > QThread::idealThreadCount()))
    {
        setting.setValue("threads", QThread::idealThreadCount());
    }
}

/**
 * @brief Setups theme, translation, etc. before opening application
 *
 * @param argc Number of arguments
 * @param argv Char array of arguments
 *
 * @return Exit code
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("PokeFinder");
    a.setOrganizationName("PokeFinder Team");

    Q_INIT_RESOURCE(i18n);
    Q_INIT_RESOURCE(resources);

    QSettings setting;
    setting.beginGroup("settings");
    validateSettings(setting);

    QString profilePath = setting.value("profiles").toString();
    bool profile = ProfileLoader::init(profilePath.toStdWString());

    a.setStyle("fusion");
    if (setting.value("style").toString() == "dark")
    {
        auto hints = a.styleHints();
        hints->setColorScheme(Qt::ColorScheme::Dark);
    }
    else if (setting.value("style").toString() == "light")
    {
        auto hints = a.styleHints();
        hints->setColorScheme(Qt::ColorScheme::Light);
    }

    QString locale = setting.value("locale").toString();
    Translator::init(locale.toStdString());

    QTranslator translator;
    if (translator.load(QString(":/i18n/PokeFinder_%1.qm").arg(locale)))
    {
        QApplication::installTranslator(&translator);
    }

    MainWindow w(profile);
    w.show();

    return a.exec();
}
