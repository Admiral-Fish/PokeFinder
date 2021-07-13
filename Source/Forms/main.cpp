/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Forms/MainWindow.hpp>
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QSettings>
#include <QStandardPaths>
#include <QTextStream>
#include <QThread>
#include <QTranslator>

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
    // TODO: remove this check in a later version
    else if (setting.value("settings/locale").toString() == "zh_Hans_CN")
    {
        setting.setValue("settings/locale", "zh");
    }

    if (!setting.contains("settings/threads") || (setting.value("settings/threads").toInt() > QThread::idealThreadCount()))
    {
        setting.setValue("settings/threads", QThread::idealThreadCount());
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("PokeFinder");
    a.setOrganizationName("PokeFinder Team");

    QSettings setting;
    validateSettings(setting);

    QString profilePath = setting.value("settings/profiles").toString();
    bool profile = ProfileLoader::init(profilePath.toStdString());

    // Transfer profiles to new setup
    // TODO: remove in a future version
    if (setting.contains("profiles"))
    {
        QByteArray data = setting.value("profiles").toByteArray();
        QJsonDocument profiles(QJsonDocument::fromJson(data));

        QFile f(profilePath);
        if (f.open(QIODevice::WriteOnly))
        {
            f.write(QJsonDocument(profiles).toJson());
            setting.remove("profiles");
        }
    }

    QString style = setting.value("settings/style").toString();
    if (style == "dark")
    {
        QFile file(":/qdarkstyle/dark/style.qss");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream ts(&file);
            a.setStyleSheet(ts.readAll());
        }
    }

    QString locale = setting.value("settings/locale").toString();
    Translator::init(locale.toStdString());

    QTranslator translator;
    if (translator.load(QString(":/i18n/PokeFinder_%1.qm").arg(locale)))
    {
        QApplication::installTranslator(&translator);
    }

    MainWindow w(profile);
    w.show();
    w.raise();

    return a.exec();
}
