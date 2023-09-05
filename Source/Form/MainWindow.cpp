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

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <Core/Util/Translator.hpp>
#include <Form/Gen3/Eggs3.hpp>
#include <Form/Gen3/GameCube.hpp>
#include <Form/Gen3/IDs3.hpp>
#include <Form/Gen3/Profile/ProfileManager3.hpp>
#include <Form/Gen3/Static3.hpp>
#include <Form/Gen3/Tools/GameCubeSeedFinder.hpp>
#include <Form/Gen3/Tools/JirachiAdvancer.hpp>
#include <Form/Gen3/Tools/PIDToIV.hpp>
#include <Form/Gen3/Tools/PokeSpot.hpp>
#include <Form/Gen3/Tools/SeedToTime3.hpp>
#include <Form/Gen3/Tools/SpindaPainter.hpp>
#include <Form/Gen3/Wild3.hpp>
#include <Form/Gen4/Eggs4.hpp>
#include <Form/Gen4/Event4.hpp>
#include <Form/Gen4/IDs4.hpp>
#include <Form/Gen4/Profile/ProfileManager4.hpp>
#include <Form/Gen4/Static4.hpp>
#include <Form/Gen4/Tools/ChainedSID.hpp>
#include <Form/Gen4/Tools/SeedToTime4.hpp>
#include <Form/Gen4/Wild4.hpp>
#include <Form/Gen5/DreamRadar.hpp>
#include <Form/Gen5/Eggs5.hpp>
#include <Form/Gen5/Event5.hpp>
#include <Form/Gen5/HiddenGrotto.hpp>
#include <Form/Gen5/IDs5.hpp>
#include <Form/Gen5/Profile/ProfileCalibrator5.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Form/Gen5/Static5.hpp>
#include <Form/Gen5/Wild5.hpp>
#include <Form/Gen8/Eggs8.hpp>
#include <Form/Gen8/Event8.hpp>
#include <Form/Gen8/IDs8.hpp>
#include <Form/Gen8/Profile/ProfileManager8.hpp>
#include <Form/Gen8/Raids.hpp>
#include <Form/Gen8/Static8.hpp>
#include <Form/Gen8/Tools/DenMap.hpp>
#include <Form/Gen8/Underground.hpp>
#include <Form/Gen8/Wild8.hpp>
#include <Form/Util/EncounterLookup.hpp>
#include <Form/Util/IVCalculator.hpp>
#include <Form/Util/IVToPID.hpp>
#include <Form/Util/Researcher.hpp>
#include <Form/Util/Settings.hpp>
#include <QClipboard>
#include <QDate>
#include <QDesktopServices>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <QtNetwork>
#include <version.h>

MainWindow::MainWindow(bool profile, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("Pok\303\251Finder %1").arg(POKEFINDER_VERSION));

    connect(ui->pushButtonEgg3, &QPushButton::clicked, this, &MainWindow::openEgg3);
    connect(ui->pushButtonGameCube, &QPushButton::clicked, this, &MainWindow::openGameCube);
    connect(ui->pushButtonIDs3, &QPushButton::clicked, this, &MainWindow::openIDs3);
    connect(ui->pushButtonStatic3, &QPushButton::clicked, this, &MainWindow::openStatic3);
    connect(ui->pushButtonWild3, &QPushButton::clicked, this, &MainWindow::openWild3);
    connect(ui->actionGameCubeSeedFinder, &QAction::triggered, this, &MainWindow::openGameCubeSeedFinder);
    connect(ui->actionIVstoPID3, &QAction::triggered, this, &MainWindow::openIVToPID);
    connect(ui->actionJirachiAdvancer, &QAction::triggered, this, &MainWindow::openJirachiAdvancer);
    connect(ui->actionPIDtoIVs, &QAction::triggered, this, &MainWindow::openPIDtoIV);
    connect(ui->actionPokeSpot, &QAction::triggered, this, &MainWindow::openPokeSpot);
    connect(ui->actionProfileManager3, &QAction::triggered, this, &MainWindow::openProfileManager3);
    connect(ui->actionSeedToTime3, &QAction::triggered, this, &MainWindow::openSeedToTime3);
    connect(ui->actionSpindaPainter, &QAction::triggered, this, &MainWindow::openSpindaPainter);

    connect(ui->pushButtonEgg4, &QPushButton::clicked, this, &MainWindow::openEgg4);
    connect(ui->pushButtonEvent4, &QPushButton::clicked, this, &MainWindow::openEvent4);
    connect(ui->pushButtonIDs4, &QPushButton::clicked, this, &MainWindow::openIDs4);
    connect(ui->pushButtonStatic4, &QPushButton::clicked, this, &MainWindow::openStatic4);
    connect(ui->pushButtonWild4, &QPushButton::clicked, this, &MainWindow::openWild4);
    connect(ui->actionIVstoPID4, &QAction::triggered, this, &MainWindow::openIVToPID);
    connect(ui->actionProfileManager4, &QAction::triggered, this, &MainWindow::openProfileManager4);
    connect(ui->actionSeedToTime4, &QAction::triggered, this, &MainWindow::openSeedToTime4);
    connect(ui->actionSIDfromChainedShiny, &QAction::triggered, this, &MainWindow::openSIDFromChainedShiny);

    connect(ui->pushButtonDreamRadar, &QPushButton::clicked, this, &MainWindow::openDreamRadar);
    connect(ui->pushButtonEgg5, &QPushButton::clicked, this, &MainWindow::openEgg5);
    connect(ui->pushButtonEvent5, &QPushButton::clicked, this, &MainWindow::openEvent5);
    connect(ui->pushButtonHiddenGrotto, &QPushButton::clicked, this, &MainWindow::openHiddenGrotto);
    connect(ui->pushButtonIDs5, &QPushButton::clicked, this, &MainWindow::openIDs5);
    connect(ui->pushButtonStatic5, &QPushButton::clicked, this, &MainWindow::openStatic5);
    connect(ui->pushButtonWild5, &QPushButton::clicked, this, &MainWindow::openWild5);
    connect(ui->actionProfileCalibrator, &QAction::triggered, this, &MainWindow::openProfileCalibrator);
    connect(ui->actionProfileManager5, &QAction::triggered, this, &MainWindow::openProfileManager5);

    connect(ui->pushButtonEgg8, &QPushButton::clicked, this, &MainWindow::openEgg8);
    connect(ui->pushButtonEvent8, &QPushButton::clicked, this, &MainWindow::openEvent8);
    connect(ui->pushButtonIDs8, &QPushButton::clicked, this, &MainWindow::openIDs8);
    connect(ui->pushButtonStatic8, &QPushButton::clicked, this, &MainWindow::openStatic8);
    connect(ui->pushButtonWild8, &QPushButton::clicked, this, &MainWindow::openWild8);
    connect(ui->pushButtonUnderground, &QPushButton::clicked, this, &MainWindow::openUnderground);
    connect(ui->pushButtonRaid, &QPushButton::clicked, this, &MainWindow::openRaids);
    connect(ui->actionDenMap, &QAction::triggered, this, &MainWindow::openDenMap);
    connect(ui->actionProfileManager8, &QAction::triggered, this, &MainWindow::openProfileManager8);

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::openAbout);
    connect(ui->actionEncounterLookup, &QAction::triggered, this, &MainWindow::openEncounterLookup);
    connect(ui->actionIVCalculator, &QAction::triggered, this, &MainWindow::openIVCalculator);
    connect(ui->actionResearcher, &QAction::triggered, this, &MainWindow::openResearcher);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::openSettings);

    QSettings setting;
    if (setting.contains("mainWindow/geometry"))
    {
        this->restoreGeometry(setting.value("mainWindow/geometry").toByteArray());
    }

    QTimer::singleShot(1000, this, [this, profile] {
        if (!profile)
        {
            QMessageBox msg(QMessageBox::Warning, tr("Unable to locate profiles"),
                            tr("Please update path to your profiles file to restore existing profiles."));
            msg.exec();
        }

        checkUpdates();
    });
}

MainWindow::~MainWindow()
{
    QSettings setting;
    setting.setValue("mainWindow/geometry", this->saveGeometry());

    delete ui;

    delete egg3;
    delete gamecube;
    delete ids3;
    delete static3;
    delete wild3;

    delete egg4;
    delete event4;
    delete ids4;
    delete static4;
    delete wild4;

    delete dreamRadar;
    delete egg5;
    delete event5;
    delete hiddenGrotto;
    delete ids5;
    delete static5;
    delete wild5;

    delete egg8;
    delete event8;
    delete ids8;
    delete raids;
    delete static8;
    delete wild8;
    delete underground;
}

void MainWindow::checkUpdates() const
{
    QSettings setting;
    QDate today = QDate::currentDate();
    QDate lastOpened = setting.value("settings/lastOpened", today).toDate();

    if (lastOpened.daysTo(today) > 0)
    {
        auto json = QJsonDocument::fromJson(downloadFile("https://api.github.com/repos/Admiral-Fish/PokeFinder/releases/latest")).object();
        QString webVersion = json["tag_name"].toString().right(5);
        if (!webVersion.isEmpty() && POKEFINDER_VERSION != webVersion)
        {
            QMessageBox msg(QMessageBox::Question, tr("Update Check"),
                            tr("An update is available. Would you like to download the newest version?"),
                            QMessageBox::Yes | QMessageBox::No);
            if (msg.exec() == QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl("https://github.com/Admiral-Fish/PokeFinder/releases/latest"));
            }
        }
    }

    setting.setValue("settings/lastOpened", today);
}

QByteArray MainWindow::downloadFile(const QString &url) const
{
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    std::unique_ptr<QNetworkReply> reply(manager.get(request));

    QEventLoop loop;
    connect(reply.get(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    return reply->readAll();
}

void MainWindow::openEgg3()
{
    if (!egg3)
    {
        egg3 = new Eggs3();
        connect(egg3, &Eggs3::profilesModified, this, &MainWindow::updateProfiles);
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

void MainWindow::openGameCube()
{
    if (!gamecube)
    {
        gamecube = new GameCube();
        connect(gamecube, &GameCube::profilesModified, this, &MainWindow::updateProfiles);
    }
    gamecube->show();
    gamecube->raise();
}

void MainWindow::openGameCubeSeedFinder()
{
    auto *window = new GameCubeSeedFinder();
    window->show();
}

void MainWindow::openJirachiAdvancer() const
{
    auto *window = new JirachiAdvancer();
    window->show();
}

void MainWindow::openPIDtoIV() const
{
    auto *window = new PIDToIV();
    window->show();
}

void MainWindow::openPokeSpot()
{
    auto *window = new PokeSpot();
    window->show();
}

void MainWindow::openProfileManager3() const
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::profilesModified, this, &MainWindow::updateProfiles);
    manager->show();
}

void MainWindow::openSeedToTime3() const
{
    auto *window = new SeedToTime3();
    window->show();
}

void MainWindow::openSpindaPainter()
{
    auto *window = new SpindaPainter();
    window->show();
}

void MainWindow::openStatic3()
{
    if (!static3)
    {
        static3 = new Static3();
        connect(static3, &Static3::profilesModified, this, &MainWindow::updateProfiles);
    }
    static3->show();
    static3->raise();
}

void MainWindow::openWild3()
{
    if (!wild3)
    {
        wild3 = new Wild3();
        connect(wild3, &Wild3::profilesModified, this, &MainWindow::updateProfiles);
    }
    wild3->show();
    wild3->raise();
}

void MainWindow::openEgg4()
{
    if (!egg4)
    {
        egg4 = new Eggs4();
        connect(egg4, &Eggs4::profilesModified, this, &MainWindow::updateProfiles);
    }
    egg4->show();
}

void MainWindow::openEvent4()
{
    if (!event4)
    {
        event4 = new Event4();
        connect(event4, &Event4::profilesModified, this, &MainWindow::updateProfiles);
    }
    event4->show();
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

void MainWindow::openProfileManager4() const
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::profilesModified, this, &MainWindow::updateProfiles);
    manager->show();
}

void MainWindow::openStatic4()
{
    if (!static4)
    {
        static4 = new Static4();
        connect(static4, &Static4::profilesModified, this, &MainWindow::updateProfiles);
    }
    static4->show();
    static4->raise();
}

void MainWindow::openWild4()
{
    if (!wild4)
    {
        wild4 = new Wild4();
        connect(wild4, &Wild4::profilesModified, this, &MainWindow::updateProfiles);
    }
    wild4->show();
    wild4->raise();
}

void MainWindow::openSeedToTime4() const
{
    auto *window = new SeedToTime4();
    window->show();
}

void MainWindow::openSIDFromChainedShiny()
{
    auto *window = new ChainedSID();
    window->show();
}

void MainWindow::openDreamRadar()
{
    if (!dreamRadar)
    {
        dreamRadar = new DreamRadar();
        connect(dreamRadar, &DreamRadar::profilesModified, this, &MainWindow::updateProfiles);
    }

    if (!dreamRadar->hasProfiles())
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profiles found"),
                        tr("Please use the Profile Calibrator under Gen 5 Tools to create one."));
        msg.exec();
        dreamRadar->close();
    }
    else
    {
        dreamRadar->show();
        dreamRadar->raise();
    }
}

void MainWindow::openEgg5()
{
    if (!egg5)
    {
        egg5 = new Eggs5();
        connect(egg5, &Eggs5::profilesModified, this, &MainWindow::updateProfiles);
    }

    if (!egg5->hasProfiles())
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profiles found"),
                        tr("Please use the Profile Calibrator under Gen 5 Tools to create one."));
        msg.exec();
        egg5->close();
    }
    else
    {
        egg5->show();
        egg5->raise();
    }
}

void MainWindow::openEvent5()
{
    if (!event5)
    {
        event5 = new Event5();
        connect(event5, &Event5::profilesModified, this, &MainWindow::updateProfiles);
    }

    if (!event5->hasProfiles())
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profiles found"),
                        tr("Please use the Profile Calibrator under Gen 5 Tools to create one."));
        msg.exec();
        event5->close();
    }
    else
    {
        event5->show();
        event5->raise();
    }
}

void MainWindow::openHiddenGrotto()
{
    if (!hiddenGrotto)
    {
        hiddenGrotto = new HiddenGrotto();
        connect(hiddenGrotto, &HiddenGrotto::profilesModified, this, &MainWindow::updateProfiles);
    }

    if (!hiddenGrotto->hasProfiles())
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profiles found"),
                        tr("Please use the Profile Calibrator under Gen 5 Tools to create one."));
        msg.exec();
        hiddenGrotto->close();
    }
    else
    {
        hiddenGrotto->show();
        hiddenGrotto->raise();
    }
}

void MainWindow::openIDs5()
{
    if (!ids5)
    {
        ids5 = new IDs5();
        connect(ids5, &IDs5::profilesModified, this, &MainWindow::updateProfiles);
    }

    if (!ids5->hasProfiles())
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profiles found"),
                        tr("Please use the Profile Calibrator under Gen 5 Tools to create one."));
        msg.exec();
        ids5->close();
    }
    else
    {
        ids5->show();
        ids5->raise();
    }
}

void MainWindow::openStatic5()
{
    if (!static5)
    {
        static5 = new Static5();
        connect(static5, &Static5::profilesModified, this, &MainWindow::updateProfiles);
    }

    if (!static5->hasProfiles())
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profiles found"),
                        tr("Please use the Profile Calibrator under Gen 5 Tools to create one."));
        msg.exec();
        static5->close();
    }
    else
    {
        static5->show();
        static5->raise();
    }
}

void MainWindow::openWild5()
{
    if (!wild5)
    {
        wild5 = new Wild5();
        connect(wild5, &Wild5::profilesModified, this, &MainWindow::updateProfiles);
    }

    if (!wild5->hasProfiles())
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profiles found"),
                        tr("Please use the Profile Calibrator under Gen 5 Tools to create one."));
        msg.exec();
        wild5->close();
    }
    else
    {
        wild5->show();
        wild5->raise();
    }
}

void MainWindow::openProfileCalibrator() const
{
    auto *calibrator = new ProfileCalibrator5();
    connect(calibrator, &ProfileCalibrator5::profilesModified, this, &MainWindow::updateProfiles);
    calibrator->show();
}

void MainWindow::openProfileManager5() const
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, &MainWindow::updateProfiles);
    manager->show();
}

void MainWindow::openDenMap()
{
    auto *window = new DenMap();
    window->show();
}

void MainWindow::openEgg8()
{
    if (!egg8)
    {
        egg8 = new Eggs8();
        connect(egg8, &Eggs8::profilesModified, this, &MainWindow::updateProfiles);
    }
    egg8->show();
    egg8->raise();
}

void MainWindow::openEvent8()
{
    if (!event8)
    {
        event8 = new Event8();
        connect(event8, &Event8::profilesModified, this, &MainWindow::updateProfiles);
    }
    event8->show();
    event8->raise();
}

void MainWindow::openIDs8()
{
    if (!ids8)
    {
        ids8 = new IDs8();
    }
    ids8->show();
    ids8->raise();
}

void MainWindow::openProfileManager8() const
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::profilesModified, this, &MainWindow::updateProfiles);
    manager->show();
}

void MainWindow::openRaids()
{
    if (!raids)
    {
        raids = new Raids();
        connect(raids, &Raids::profilesModified, this, &MainWindow::updateProfiles);
    }
    raids->show();
    raids->show();
}

void MainWindow::openStatic8()
{
    if (!static8)
    {
        static8 = new Static8();
        connect(static8, &Static8::profilesModified, this, &MainWindow::updateProfiles);
    }
    static8->show();
    static8->show();
}

void MainWindow::openUnderground()
{
    if (!underground)
    {
        underground = new Underground();
        connect(underground, &Underground::profilesModified, this, &MainWindow::updateProfiles);
    }
    underground->show();
    underground->raise();
}

void MainWindow::openWild8()
{
    if (!wild8)
    {
        wild8 = new Wild8();
        connect(wild8, &Wild8::profilesModified, this, &MainWindow::updateProfiles);
    }
    wild8->show();
    wild8->raise();
}

void MainWindow::openAbout() const
{
    QStringList info = { tr("Version: %1").arg(POKEFINDER_VERSION), tr("Branch: %1").arg(GIT_BRANCH), tr("Commit: %1").arg(GIT_COMMIT) };

    QMessageBox msg(QMessageBox::Information, tr("About"), info.join('\n'), QMessageBox::Close);
    QAbstractButton *copy = msg.addButton(tr("Copy"), QMessageBox::NoRole);
    msg.exec();
    if (msg.clickedButton() == copy)
    {
        QApplication::clipboard()->setText(info.join('\n'));
    }
}

void MainWindow::openEncounterLookup() const
{
    auto *window = new EncounterLookup();
    window->show();
}

void MainWindow::openIVCalculator() const
{
    auto *window = new IVCalculator();
    window->show();
}

void MainWindow::openIVToPID() const
{
    auto *window = new IVToPID();
    window->show();
}

void MainWindow::openResearcher() const
{
    auto *window = new Researcher();
    window->show();
}

void MainWindow::openSettings() const
{
    auto *window = new Settings();
    window->show();
}

void MainWindow::updateProfiles(int num)
{
    if (num == 3)
    {
        if (egg3)
        {
            egg3->updateProfiles();
        }
        if (gamecube)
        {
            gamecube->updateProfiles();
        }
        if (static3)
        {
            static3->updateProfiles();
        }
        if (wild3)
        {
            wild3->updateProfiles();
        }
    }
    else if (num == 4)
    {
        if (egg4)
        {
            egg4->updateProfiles();
        }
        if (static4)
        {
            static4->updateProfiles();
        }
        if (wild4)
        {
            wild4->updateProfiles();
        }
    }
    else if (num == 5)
    {
        if (dreamRadar)
        {
            dreamRadar->updateProfiles();
        }
        if (egg5)
        {
            egg5->updateProfiles();
        }
        if (event5)
        {
            event5->updateProfiles();
        }
        if (hiddenGrotto)
        {
            hiddenGrotto->updateProfiles();
        }
        if (ids5)
        {
            ids5->updateProfiles();
        }
        if (static5)
        {
            static5->updateProfiles();
        }
        if (wild5)
        {
            wild5->updateProfiles();
        }
    }
    else if (num == 8)
    {
        if (egg8)
        {
            egg8->updateProfiles();
        }
        if (event8)
        {
            event8->updateProfiles();
        }
        if (raids)
        {
            raids->updateProfiles();
        }
        if (static8)
        {
            static8->updateProfiles();
        }
        if (wild8)
        {
            wild8->updateProfiles();
        }
    }
}
