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
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>
#include <QTranslator>
#include <Forms/MainWindow.hpp>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QFile file("log.txt");
    if (file.open(QIODevice::Append))
    {
        QString message;
        switch (type)
        {
            case QtDebugMsg:
                message = QString("Debug: %1 (%2:%3, %4)\n").arg(msg, QString(context.file), QString::number(context.line), QString(context.function));
                break;
            case QtInfoMsg:
                message = QString("Info: %1 (%2:%3, %4)\n").arg(msg, QString(context.file), QString::number(context.line), QString(context.function));
                break;
            case QtWarningMsg:
                message = QString("Warning: %1 (%2:%3, %4)\n").arg(msg, QString(context.file), QString::number(context.line), QString(context.function));
                break;
            case QtCriticalMsg:
                message = QString("Critical: %1 (%2:%3, %4)\n").arg(msg, QString(context.file), QString::number(context.line), QString(context.function));
                break;
            case QtFatalMsg:
                message = QString("Fatal: %1 (%2:%3, %4)\n").arg(msg, QString(context.file), QString::number(context.line), QString(context.function));
                break;
        }

        QTextStream ts(&file);
        ts << message;

        file.close();
    }

    if (type == QtFatalMsg)
    {
        QMessageBox error(QMessageBox::Critical, QObject::tr("Crash detected"), QObject::tr("A crash has occured. Please look at log.txt for more detailed information."));
        error.exec();
        QApplication::exit(1);
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QApplication a(argc, argv);
    a.setApplicationName("PokeFinder");
    a.setOrganizationName("PokeFinder Team");

    QSettings setting;
    if (setting.contains("style")) setting.clear();

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

    QTranslator translator;
    if (translator.load(QString(":/i18n/PokeFinder_%1.qm").arg(setting.value("settings/locale", "en").toString())))
    {
        QApplication::installTranslator(&translator);
    }

    MainWindow w;
    w.show();
    w.raise();

    return a.exec();
}
