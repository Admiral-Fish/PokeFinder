/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <Core/Gen8/DenLoader.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/MainWindow.hpp>
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <QThread>
#include <QTranslator>

/**
 * @brief Sets default settings for initial open or if settings get deleted
 *
 * @param setting Contains application settings
 */
void validateSettings(QSettings &setting)
{
    if (!setting.contains("settings/profiles"))
    {
        QString documentFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        setting.setValue("settings/profiles", QString("%1/profiles.json").arg(documentFolder));
    }

    if (!setting.contains("settings/style"))
    {
        setting.setValue("settings/style", "dark");
    }

    if (!setting.contains("settings/locale"))
    {
        setting.setValue("settings/locale", "en");
    }

    if (!setting.contains("settings/threads") || (setting.value("settings/threads").toInt() > QThread::idealThreadCount()))
    {
        setting.setValue("settings/threads", QThread::idealThreadCount());
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

    Q_INIT_RESOURCE(resources);

    QSettings setting;
    validateSettings(setting);

    QString profilePath = setting.value("settings/profiles").toString();
    bool profile = ProfileLoader::init(profilePath.toStdString());

    QFile file(QString(":/qdarkstyle/%1/%1style.qss").arg(setting.value("settings/style").toString()));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream ts(&file);
        a.setStyleSheet(ts.readAll());
    }

    QString locale = setting.value("settings/locale").toString();
    Translator::init(locale.toStdString());

    QTranslator translator;
    if (translator.load(QString(":/i18n/PokeFinder_%1.qm").arg(locale)))
    {
        QApplication::installTranslator(&translator);
    }

    DenLoader::init(QApplication::applicationDirPath().toStdString());

    MainWindow w(profile);
    w.show();

    return a.exec();
}
