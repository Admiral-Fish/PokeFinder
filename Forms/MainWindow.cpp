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

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <Forms/Gen3/Eggs3.hpp>
#include <Forms/Gen3/GameCube.hpp>
#include <Forms/Gen3/IDs3.hpp>
#include <Forms/Gen3/Stationary3.hpp>
#include <Forms/Gen3/Tools/GameCubeRTC.hpp>
#include <Forms/Gen3/Tools/GameCubeSeedFinder.hpp>
#include <Forms/Gen3/Tools/JirachiPattern.hpp>
#include <Forms/Gen3/Tools/PIDIV.hpp>
#include <Forms/Gen3/Tools/PokeSpot.hpp>
#include <Forms/Gen3/Tools/SeedTime3.hpp>
#include <Forms/Gen3/Tools/SpindaPainter.hpp>
#include <Forms/Gen3/Wild3.hpp>
#include <Forms/Gen4/Eggs4.hpp>
#include <Forms/Gen4/IDs4.hpp>
#include <Forms/Gen4/Stationary4.hpp>
#include <Forms/Gen4/Tools/ChainedSID.hpp>
#include <Forms/Gen4/Tools/SeedtoTime4.hpp>
#include <Forms/Gen4/Wild4.hpp>
//#include <Forms/Gen5/Stationary5.hpp>
#include <Forms/Util/EncounterLookup.hpp>
#include <Forms/Util/IVCalculator.hpp>
#include <Forms/Util/IVtoPID.hpp>
#include <Forms/Util/Researcher.hpp>
#include <QDate>
#include <QDesktopServices>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <QtNetwork>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("Pok\303\251Finder %1").arg(VERSION));

    setupModels();
    QTimer::singleShot(1000, this, &MainWindow::checkUpdates);
}

MainWindow::~MainWindow()
{
    QSettings setting;
    setting.beginGroup("settings");
    setting.setValue("locale", currentLanguage);
    setting.setValue("style", currentStyle);
    setting.endGroup();

    setting.setValue("mainWindow/geometry", this->saveGeometry());

    delete ui;
    delete stationary3;
    delete wild3;
    delete egg3;
    delete gamecube;
    delete ids3;
    delete stationary4;
    delete wild4;
    delete egg4;
    delete ids4;
    // delete stationary5;
}

void MainWindow::setupModels()
{
    QSettings setting;

    langGroup = new QActionGroup(ui->menuLanguage);
    langGroup->setExclusive(true);
    connect(langGroup, &QActionGroup::triggered, this, &MainWindow::slotLanguageChanged);
    currentLanguage = setting.value("settings/locale", "en").toString();
    QStringList locales = { "de", "en", "es", "fr", "it", "ja", "ko", "zh" };
    for (int i = 0; i < locales.size(); i++)
    {
        const QString &lang = locales.at(i);

        auto *action = ui->menuLanguage->actions()[i];
        action->setData(lang);

        if (currentLanguage == lang)
        {
            action->setChecked(true);
        }

        langGroup->addAction(action);
    }

    styleGroup = new QActionGroup(ui->menuStyle);
    styleGroup->setExclusive(true);
    connect(styleGroup, &QActionGroup::triggered, this, &MainWindow::slotStyleChanged);
    currentStyle = setting.value("settings/style", "dark").toString();
    QStringList styles = { "dark", "light" };
    for (int i = 0; i < styles.size(); i++)
    {
        const QString &style = styles.at(i);

        auto *action = ui->menuStyle->actions()[i];
        action->setData(style);

        if (currentStyle == style)
        {
            action->setChecked(true);
        }

        styleGroup->addAction(action);
    }

    connect(ui->pushButtonStationary3, &QPushButton::clicked, this, &MainWindow::openStationary3);
    connect(ui->pushButtonWild3, &QPushButton::clicked, this, &MainWindow::openWild3);
    connect(ui->pushButtonGameCube, &QPushButton::clicked, this, &MainWindow::openGameCube);
    connect(ui->pushButtonEgg3, &QPushButton::clicked, this, &MainWindow::openEgg3);
    connect(ui->pushButtonIDs3, &QPushButton::clicked, this, &MainWindow::openIDs3);
    connect(ui->actionGameCubeRTC, &QAction::triggered, this, &MainWindow::openGameCubeRTC);
    connect(ui->actionGameCubeSeedFinder, &QAction::triggered, this, &MainWindow::openGameCubeSeedFinder);
    connect(ui->actionIVtoPID3, &QAction::triggered, this, &MainWindow::openIVtoPID);
    connect(ui->actionJirachiPattern, &QAction::triggered, this, &MainWindow::openJirachiPattern);
    connect(ui->actionPIDtoIV, &QAction::triggered, this, &MainWindow::openPIDtoIV);
    connect(ui->actionPokeSpot, &QAction::triggered, this, &MainWindow::openPokeSpot);
    connect(ui->actionSeedtoTime3, &QAction::triggered, this, &MainWindow::openSeedtoTime3);
    connect(ui->actionSpindaPainter, &QAction::triggered, this, &MainWindow::openSpindaPainter);
    connect(ui->pushButtonStationary4, &QPushButton::clicked, this, &MainWindow::openStationary4);
    connect(ui->pushButtonWild4, &QPushButton::clicked, this, &MainWindow::openWild4);
    connect(ui->pushButtonEgg4, &QPushButton::clicked, this, &MainWindow::openEgg4);
    connect(ui->pushButtonIDs4, &QPushButton::clicked, this, &MainWindow::openIDs4);
    connect(ui->actionIVtoPID4, &QAction::triggered, this, &MainWindow::openIVtoPID);
    connect(ui->actionSeedtoTime4, &QAction::triggered, this, &MainWindow::openSeedtoTime4);
    connect(ui->actionSIDfromChainedShiny, &QAction::triggered, this, &MainWindow::openSIDFromChainedShiny);
    // connect(ui->pushButtonStationary5, &QPushButton::clicked, this, &MainWindow::openStationary5);
    connect(ui->actionEncounterLookup, &QAction::triggered, this, &MainWindow::openEncounterLookup);
    connect(ui->actionIVCalculator, &QAction::triggered, this, &MainWindow::openIVCalculator);
    connect(ui->actionResearcher, &QAction::triggered, this, &MainWindow::openResearcher);

    if (setting.contains("mainWindow/geometry"))
    {
        this->restoreGeometry(setting.value("mainWindow/geometry").toByteArray());
    }
}

void MainWindow::slotLanguageChanged(QAction *action)
{
    if (action)
    {
        QString lang = action->data().toString();
        if (currentLanguage != lang)
        {
            currentLanguage = lang;

            QMessageBox message(QMessageBox::Question, tr("Language update"), tr("Restart for changes to take effect. Restart now?"),
                                QMessageBox::Yes | QMessageBox::No);
            if (message.exec() == QMessageBox::Yes)
            {
                QProcess::startDetached(QApplication::applicationFilePath());
                QApplication::quit();
            }
        }
    }
}

void MainWindow::slotStyleChanged(QAction *action)
{
    if (action)
    {
        QString style = action->data().toString();
        if (currentStyle != style)
        {
            currentStyle = style;

            QMessageBox message(QMessageBox::Question, tr("Style change"), tr("Restart for changes to take effect. Restart now?"),
                                QMessageBox::Yes | QMessageBox::No);
            if (message.exec() == QMessageBox::Yes)
            {
                QProcess::startDetached(QApplication::applicationFilePath());
                QApplication::quit();
            }
        }
    }
}

void MainWindow::checkUpdates()
{
    QSettings setting;
    QDate today = QDate::currentDate();
    QDate lastOpened = setting.value("settings/lastOpened", today).toDate();

    if (lastOpened.daysTo(today) > 0)
    {
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("https://api.github.com/repos/Admiral-Fish/PokeFinder/releases/latest"));
        QScopedPointer<QNetworkReply> reply(manager.get(request));

        QEventLoop loop;
        connect(reply.data(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
        connect(reply.data(), QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), &loop, &QEventLoop::quit);
        loop.exec();

        auto json = QJsonDocument::fromJson(reply->readAll()).object();
        QString webVersion = json["tag_name"].toString().right(5);
        if (!webVersion.isEmpty() && VERSION != webVersion)
        {
            QMessageBox info(QMessageBox::Question, tr("Update Check"),
                             tr("An update is available. Would you like to download the newest version?"),
                             QMessageBox::Yes | QMessageBox::No);
            if (info.exec() == QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl("https://github.com/Admiral-Fish/PokeFinder/releases/latest"));
            }
        }
    }

    setting.setValue("settings/lastOpened", today);
}

void MainWindow::updateProfiles(int num)
{
    if (num == 3)
    {
        if (stationary3)
        {
            stationary3->updateProfiles();
        }
        if (wild3)
        {
            wild3->updateProfiles();
        }
        if (gamecube)
        {
            gamecube->updateProfiles();
        }
        if (egg3)
        {
            egg3->updateProfiles();
        }
    }
    else if (num == 4)
    {
        if (stationary4)
        {
            stationary4->updateProfiles();
        }
        if (wild4)
        {
            wild4->updateProfiles();
        }
        if (egg4)
        {
            egg4->updateProfiles();
        }
    }
    /*else if (num == 5)
    {
        if (stationary5)
        {
            stationary5->updateProfiles();
        }
    }*/
}

void MainWindow::openStationary3()
{
    if (!stationary3)
    {
        stationary3 = new Stationary3();
        connect(stationary3, &Stationary3::alertProfiles, this, &MainWindow::updateProfiles);
    }
    stationary3->show();
    stationary3->raise();
}

void MainWindow::openWild3()
{
    if (!wild3)
    {
        wild3 = new Wild3();
        connect(wild3, &Wild3::alertProfiles, this, &MainWindow::updateProfiles);
    }
    wild3->show();
    wild3->raise();
}

void MainWindow::openGameCube()
{
    if (!gamecube)
    {
        gamecube = new GameCube();
        connect(gamecube, &GameCube::alertProfiles, this, &MainWindow::updateProfiles);
    }
    gamecube->show();
    gamecube->raise();
}

void MainWindow::openEgg3()
{
    if (!egg3)
    {
        egg3 = new Eggs3();
        connect(egg3, &Eggs3::alertProfiles, this, &MainWindow::updateProfiles);
    }
    egg3->show();
    egg3->raise();
}

void MainWindow::openIDs3()
{
    if (!ids3)
    {
        ids3 = new IDs3();
    }
    ids3->show();
    ids3->raise();
}

void MainWindow::openGameCubeRTC()
{
    auto *rtc = new GameCubeRTC();
    rtc->show();
    rtc->raise();
}

void MainWindow::openGameCubeSeedFinder()
{
    auto *finder = new GameCubeSeedFinder();
    finder->show();
    finder->raise();
}

void MainWindow::openIVtoPID()
{
    auto *ivToPID = new IVtoPID();
    ivToPID->show();
    ivToPID->raise();
}

void MainWindow::openJirachiPattern()
{
    auto *jirachi = new JirachiPattern();
    jirachi->show();
    jirachi->raise();
}

void MainWindow::openPIDtoIV()
{
    auto *pidToIV = new PIDIV();
    pidToIV->show();
    pidToIV->raise();
}

void MainWindow::openPokeSpot()
{
    auto *pokeSpot = new PokeSpot();
    pokeSpot->show();
    pokeSpot->raise();
}

void MainWindow::openSeedtoTime3()
{
    auto *seedToTime = new SeedTime3();
    seedToTime->show();
    seedToTime->raise();
}

void MainWindow::openSpindaPainter()
{
    auto *spinda = new SpindaPainter();
    spinda->show();
    spinda->raise();
}

void MainWindow::openStationary4()
{
    if (!stationary4)
    {
        stationary4 = new Stationary4();
        connect(stationary4, &Stationary4::alertProfiles, this, &MainWindow::updateProfiles);
    }
    stationary4->show();
    stationary4->raise();
}

void MainWindow::openWild4()
{
    if (!wild4)
    {
        wild4 = new Wild4();
        connect(wild4, &Wild4::alertProfiles, this, &MainWindow::updateProfiles);
    }
    wild4->show();
    wild4->raise();
}

void MainWindow::openEgg4()
{
    if (!egg4)
    {
        egg4 = new Eggs4();
        connect(egg4, &Eggs4::alertProfiles, this, &MainWindow::updateProfiles);
    }
    egg4->show();
    egg4->raise();
}

void MainWindow::openIDs4()
{
    if (!ids4)
    {
        ids4 = new IDs4();
    }
    ids4->show();
    ids4->raise();
}

void MainWindow::openSeedtoTime4()
{
    auto *seedToTime = new SeedtoTime4();
    seedToTime->show();
    seedToTime->raise();
}

void MainWindow::openSIDFromChainedShiny()
{
    auto *chainedSID = new ChainedSID();
    chainedSID->show();
    chainedSID->raise();
}

/*void MainWindow::openStationary5()
{
    if (!stationary5)
    {
        stationary5 = new Stationary5();
        connect(stationary5, &Stationary5::alertProfiles, this, &MainWindow::updateProfiles);
    }
    stationary5->show();
    stationary5->raise();
}*/

void MainWindow::openEncounterLookup()
{
    auto *lookup = new EncounterLookup();
    lookup->show();
    lookup->raise();
}

void MainWindow::openIVCalculator()
{
    auto *iv = new IVCalculator();
    iv->show();
    iv->raise();
}

void MainWindow::openResearcher()
{
    auto *r = new Researcher();
    r->show();
    r->raise();
}
