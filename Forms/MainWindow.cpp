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

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <Forms/Gen3/Eggs3.hpp>
#include <Forms/Gen3/GameCube.hpp>
#include <Forms/Gen3/GameCubeRTC.hpp>
#include <Forms/Gen3/GameCubeSeedFinder.hpp>
#include <Forms/Gen3/IDs3.hpp>
#include <Forms/Gen3/JirachiPattern.hpp>
#include <Forms/Gen3/PIDtoIVs.hpp>
#include <Forms/Gen3/PokeSpot.hpp>
#include <Forms/Gen3/SeedToTime3.hpp>
#include <Forms/Gen3/SpindaPainter.hpp>
#include <Forms/Gen3/Stationary3.hpp>
#include <Forms/Gen3/Wild3.hpp>
#include <Forms/Gen4/ChainedSID.hpp>
#include <Forms/Gen4/Eggs4.hpp>
#include <Forms/Gen4/IDs4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>
#include <Forms/Gen4/Stationary4.hpp>
#include <Forms/Gen4/Wild4.hpp>
#include <Forms/Util/EncounterLookup.hpp>
#include <Forms/Util/IVCalculator.hpp>
#include <Forms/Util/IVtoPID.hpp>
#include <Forms/Util/Researcher.hpp>
#include <QDesktopServices>
#include <QtNetwork>

namespace PokeFinderForms
{
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        setWindowTitle(QString("Pok\303\251Finder %1").arg(VERSION));

        setupLanguage();
        setupStyle();
        QTimer::singleShot(1000, this, &MainWindow::checkUpdates);

        QSettings setting;
        if (setting.contains("mainWindow/geometry"))
            this->restoreGeometry(setting.value("mainWindow/geometry").toByteArray());
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
        delete ids3;
        delete stationary4;
        delete wild4;
        delete egg4;
        delete ids4;
    }

    void MainWindow::setupLanguage()
    {
        langGroup = new QActionGroup(ui->menuLanguage);
        langGroup->setExclusive(true);
        connect(langGroup, &QActionGroup::triggered, this, &MainWindow::slotLanguageChanged);

        QSettings setting;
        currentLanguage = setting.value("settings/locale", "en").toString();

        QStringList locales = { "de", "en", "es", "fr", "it", "ja", "ko", "zh_Hans_CN" };
        for (u8 i = 0; i < locales.size(); i++)
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
    }

    void MainWindow::setupStyle()
    {
        styleGroup = new QActionGroup(ui->menuStyle);
        styleGroup->setExclusive(true);
        connect(styleGroup, &QActionGroup::triggered, this, &MainWindow::slotStyleChanged);

        QSettings setting;
        currentStyle = setting.value("settings/style", "dark").toString();

        QStringList styles = { "dark", "light" };
        for (u8 i = 0; i < styles.size(); i++)
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
    }

    void MainWindow::checkUpdates()
    {
        QSettings setting;
        QDate today = QDate::currentDate();
        QDate lastOpened = setting.value("settings/lastOpened", today).toDate();

        if (lastOpened.daysTo(today) >= 0)
        {
            QNetworkAccessManager manager;
            QNetworkRequest request(QUrl("https://api.github.com/repos/Admiral-Fish/PokeFinder/releases/latest"));
            QScopedPointer<QNetworkReply> reply(manager.get(request));

            QEventLoop loop;
            connect(reply.data(), SIGNAL(finished()), &loop, SLOT(quit()));
            connect(reply.data(), SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
            loop.exec();

            auto response = QJsonDocument::fromJson(reply->readAll());
            QString webVersion = response.object()["tag_name"].toString().right(5);
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

    void MainWindow::slotLanguageChanged(QAction *action)
    {
        if (action)
        {
            QString lang = action->data().toString();
            if (currentLanguage != lang)
            {
                currentLanguage = lang;

                QMessageBox message(QMessageBox::Question, tr("Language update"),
                    tr("Restart for changes to take effect. Restart now?"), QMessageBox::Yes | QMessageBox::No);
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

                QMessageBox message(QMessageBox::Question, tr("Style change"),
                    tr("Restart for changes to take effect. Restart now?"), QMessageBox::Yes | QMessageBox::No);
                if (message.exec() == QMessageBox::Yes)
                {
                    QProcess::startDetached(QApplication::applicationFilePath());
                    QApplication::quit();
                }
            }
        }
    }

    void MainWindow::updateProfiles(int num)
    {
        if (num == 3)
        {
            if (stationary3)
                stationary3->updateProfiles();
            if (wild3)
                wild3->updateProfiles();
            if (gamecube)
                gamecube->updateProfiles();
            if (egg3)
                egg3->updateProfiles();
        }
        else if (num == 4)
        {
            if (stationary4)
                stationary4->updateProfiles();
            if (wild4)
                wild4->updateProfiles();
            if (egg4)
                egg4->updateProfiles();
        }
    }

    void MainWindow::on_pushButtonStationary3_clicked()
    {
        if (!stationary3)
        {
            stationary3 = new Stationary3();
            connect(stationary3, &Stationary3::alertProfiles, this, &MainWindow::updateProfiles);
        }
        stationary3->show();
        stationary3->raise();
    }

    void MainWindow::on_pushButtonWild3_clicked()
    {
        if (!wild3)
        {
            wild3 = new Wild3();
            connect(wild3, &Wild3::alertProfiles, this, &MainWindow::updateProfiles);
        }
        wild3->show();
        wild3->raise();
    }

    void MainWindow::on_pushButtonGameCube_clicked()
    {
        if (!gamecube)
        {
            gamecube = new GameCube();
            connect(gamecube, &GameCube::alertProfiles, this, &MainWindow::updateProfiles);
        }
        gamecube->show();
        gamecube->raise();
    }

    void MainWindow::on_pushButtonEgg3_clicked()
    {
        if (!egg3)
        {
            egg3 = new Eggs3();
            connect(egg3, &Eggs3::alertProfiles, this, &MainWindow::updateProfiles);
        }
        egg3->show();
        egg3->raise();
    }

    void MainWindow::on_pushButtonIDs3_clicked()
    {
        if (!ids3)
        {
            ids3 = new IDs3();
        }
        ids3->show();
        ids3->raise();
    }

    void MainWindow::on_actionGameCubeRTC_triggered()
    {
        auto *rtc = new GameCubeRTC();
        rtc->show();
        rtc->raise();
    }

    void MainWindow::on_actionGameCube_Seed_Finder_triggered()
    {
        auto *finder = new GameCubeSeedFinder();
        finder->show();
        finder->raise();
    }

    void MainWindow::on_actionIVtoPID3_triggered()
    {
        auto *ivToPID = new IVtoPID();
        ivToPID->show();
        ivToPID->raise();
    }

    void MainWindow::on_actionJirachiPattern_triggered()
    {
        auto *jirachi = new JirachiPattern();
        jirachi->show();
        jirachi->raise();
    }

    void MainWindow::on_actionPIDtoIV_triggered()
    {
        auto *pidToIV = new PIDtoIVs();
        if (stationary3)
        {
            connect(pidToIV, &PIDtoIVs::moveResultsToStationary, stationary3, &Stationary3::moveResults);
        }
        pidToIV->show();
        pidToIV->raise();
    }

    void MainWindow::on_actionPokeSpot_triggered()
    {
        auto *pokeSpot = new PokeSpot();
        pokeSpot->show();
        pokeSpot->raise();
    }

    void MainWindow::on_actionSeedtoTime3_triggered()
    {
        auto *seedToTime = new SeedToTime3();
        seedToTime->show();
        seedToTime->raise();
    }

    void MainWindow::on_actionSpinda_Painter_triggered()
    {
        auto *spinda = new SpindaPainter();
        spinda->show();
        spinda->raise();
    }

    void MainWindow::on_pushButtonStationary4_clicked()
    {
        if (!stationary4)
        {
            stationary4 = new Stationary4();
            connect(stationary4, &Stationary4::alertProfiles, this, &MainWindow::updateProfiles);
        }
        stationary4->show();
        stationary4->raise();
    }

    void MainWindow::on_pushButtonWild4_clicked()
    {
        if (!wild4)
        {
            wild4 = new Wild4();
            connect(wild4, &Wild4::alertProfiles, this, &MainWindow::updateProfiles);
        }
        wild4->show();
        wild4->raise();
    }

    void MainWindow::on_pushButtonEgg4_clicked()
    {
        if (!egg4)
        {
            egg4 = new Eggs4();
            connect(egg4, &Eggs4::alertProfiles, this, &MainWindow::updateProfiles);
        }
        egg4->show();
        egg4->raise();
    }

    void MainWindow::on_pushButtonIDs4_clicked()
    {
        if (!ids4)
        {
            ids4 = new IDs4();
        }
        ids4->show();
        ids4->raise();
    }

    void MainWindow::on_actionIVtoPID4_triggered()
    {
        auto *ivToPID = new IVtoPID();
        ivToPID->show();
        ivToPID->raise();
    }

    void MainWindow::on_actionSeedtoTime4_triggered()
    {
        auto *seedToTime = new SeedtoTime4();
        seedToTime->show();
        seedToTime->raise();
    }

    void MainWindow::on_actionSID_from_Chained_Shiny_triggered()
    {
        auto *chainedSID = new ChainedSID();
        chainedSID->show();
        chainedSID->raise();
    }

    void MainWindow::on_actionEncounter_Lookup_triggered()
    {
        auto *lookup = new EncounterLookup();
        lookup->show();
        lookup->raise();
    }

    void MainWindow::on_actionIV_Calculator_triggered()
    {
        auto *iv = new IVCalculator();
        iv->show();
        iv->raise();
    }

    void MainWindow::on_actionResearcher_triggered()
    {
        auto *r = new Researcher();
        r->show();
        r->raise();
    }
}
