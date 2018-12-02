/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    checkProfileJson();
    setupLanguage();
    checkUpdates();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete langGroup;
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
    QString currLang = setting.value("locale", "en").toString();

    QStringList locales = { "de", "en", "es", "fr", "it", "ja", "ko", "zh_Hans_CN" };
    for (int i = 0; i < locales.size(); i++)
    {
        QString lang = locales[i];

        auto *action = ui->menuLanguage->actions()[i];
        action->setData(lang);

        if (currLang == lang)
            action->setChecked(true);

        langGroup->addAction(action);
    }
    switchTranslator(translator, QString(":/translations/PokeFinder_%1.qm").arg(currLang));
    ui->retranslateUi(this);
}

void MainWindow::slotLanguageChanged(QAction *action)
{
    if (action != nullptr)
        loadLanguage(action->data().toString());
}

void MainWindow::updateProfiles(int num)
{
    if (num == 3)
    {
        if (stationary3) stationary3->updateProfiles();
        if (wild3) wild3->updateProfiles();
        if (egg3) egg3->updateProfiles();
    }
    else if (num == 4)
    {
        if (stationary4) stationary4->updateProfiles();
        if (wild4) wild4->updateProfiles();
        if (egg4) egg4->updateProfiles();
    }
}

void MainWindow::loadLanguage(const QString &lang)
{
    QSettings setting;
    if (setting.value("locale", "en") != lang)
    {
        setting.setValue("locale", lang);

        QMessageBox message;
        message.setText(tr("Language updated. Please restart for changes to take effect."));
        message.exec();
    }
}

void MainWindow::switchTranslator(QTranslator &translator, const QString &filename)
{
    QApplication::removeTranslator(&translator);
    if (translator.load(filename))
        QApplication::installTranslator(&translator);
}

void MainWindow::checkProfileJson()
{
    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::NewOnly | QIODevice::Text))
    {
        file.close();
    }
}

void MainWindow::checkUpdates()
{
    // Setup date program was last opened and today's date
    QSettings setting;
    QDate today = QDate::currentDate();
    QDate lastOpened = setting.value("lastOpened", today).toDate();

    // Only check for update once a day
    if (lastOpened.daysTo(today) > 0)
    {
        // Access current version number from github
        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("https://raw.githubusercontent.com/Admiral-Fish/PokeFinder/master/version.txt"));
        QNetworkReply *reply = manager.get(request);

        QEventLoop loop;
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
        loop.exec();

        // Check if online version does not match current version
        QString webVersion(reply->readAll());
        if (webVersion != VERSION && !webVersion.isEmpty())
        {
            QMessageBox info;
            info.setWindowTitle(tr("Update Check"));
            info.setText(tr("An update is available. Would you like to download the newest version?"));
            info.setStandardButtons(QMessageBox::Yes);
            info.addButton(QMessageBox::No);
            info.setDefaultButton(QMessageBox::No);
            // If user wants the latest version open the page in browser
            if (info.exec() == QMessageBox::Yes)
                QDesktopServices::openUrl(QUrl("https://github.com/Admiral-Fish/PokeFinder/releases/tag/v" + webVersion));
        }
        delete reply;
    }

    setting.setValue("lastOpened", today);
}

void MainWindow::on_actionResearcher_triggered()
{
    auto *r = new Researcher();
    r->show();
    r->raise();
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

void MainWindow::on_action16BitSeedtoTime_triggered()
{
    auto *seedToTime = new SeedToTime3();
    seedToTime->show();
    seedToTime->raise();
}

void MainWindow::on_actionJirachiPattern_triggered()
{
    auto *jirachi = new JirachiPattern();
    jirachi->show();
    jirachi->raise();
}

void MainWindow::on_actionPokeSpot_triggered()
{
    auto *pokeSpot = new PokeSpot();
    pokeSpot->show();
    pokeSpot->raise();
}

void MainWindow::on_actionIVtoPID_triggered()
{
    auto *ivToPID = new IVtoPID();
    ivToPID->show();
    ivToPID->raise();
}

void MainWindow::on_actionGameCubeRTC_triggered()
{
    auto *rtc = new GameCubeRTC();
    rtc->show();
    rtc->raise();
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

void MainWindow::on_actionSeed_to_Time_triggered()
{
    auto *seedToTime = new SeedtoTime4();
    seedToTime->show();
    seedToTime->raise();
}

void MainWindow::on_actionIV_to_PID_triggered()
{
    auto *ivToPID = new IVtoPID();
    ivToPID->show();
    ivToPID->raise();
}
