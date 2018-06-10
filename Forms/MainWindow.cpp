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

    QFile file(QApplication::applicationDirPath() + "/profiles.xml");
    if (!file.exists())
        createProfileXml();

    setupLanguage();
    setupModels();
}

MainWindow::~MainWindow()
{
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

void MainWindow::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                setupModels();
                break;
            case QEvent::LocaleChange:
                {
                    QString locale = QLocale::system().name();
                    locale.truncate(locale.lastIndexOf('_'));
                    loadLanguage(locale);
                    break;
                }
            default:
                break;
        }
    }
}

void MainWindow::setupLanguage()
{
    QActionGroup *langGroup = new QActionGroup(ui->menuLanguage);
    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));
    langGroup->setExclusive(true);
    QStringList files = QDir(langPath).entryList(QStringList("PokeFinder_*.qm"));

    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

    for (int i = 0; i < files.size(); i++)
    {
        QString locale = files[i];
        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.indexOf('_') + 1);

        QAction *action = new QAction(this);

        action->setCheckable(true);
        action->setData(locale);

        if (defaultLocale == locale)
            action->setChecked(true);

        ui->menuLanguage->addAction(action);
        langGroup->addAction(action);
    }
    slotLanguageChanged(langGroup->checkedAction());
}

void MainWindow::setupModels()
{
    QList<QAction *> actions = ui->menuLanguage->actions();
    QStringList langs = QStringList() << tr("German") << tr("English") << tr("Spanish") << tr("French") << tr("Italian") << tr("Japanese") << tr("Korean") << tr("Chinese");
    QMap<QString, int> keys;
    keys["de"] = 0;
    keys["en"] = 1;
    keys["es"] = 2;
    keys["fr"] = 3;
    keys["it"] = 4;
    keys["ja"] = 5;
    keys["ko"] = 6;
    keys["zh_Hans_CN"] = 7;

    for (QAction *action : actions)
        action->setText(langs[keys[action->data().toString()]]);
}

void MainWindow::slotLanguageChanged(QAction *action)
{
    if (action != NULL)
        loadLanguage(action->data().toString());
}

void MainWindow::updateProfiles(int num)
{
    if (num == 3)
    {
        if (stationary3 != NULL) stationary3->updateProfiles();
        if (wild3 != NULL) wild3->updateProfiles();
        if (egg3 != NULL) egg3->updateProfiles();
    }
    else if (num == 4)
    {
        if (stationary4 != NULL) stationary4->updateProfiles();
        if (wild4 != NULL) wild4->updateProfiles();
        if (egg4 != NULL) egg4->updateProfiles();
    }
}

void MainWindow::loadLanguage(const QString &lang)
{
    if (currLang != lang)
    {
        currLang = lang;
        QLocale locale = QLocale(currLang);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(translator, QString("PokeFinder_%1.qm").arg(currLang));
    }
}

void MainWindow::switchTranslator(QTranslator &translator, const QString &filename)
{
    qApp->removeTranslator(&translator);
    if (translator.load(langPath + filename))
        qApp->installTranslator(&translator);
}

void MainWindow::createProfileXml()
{
    QFile file(QApplication::applicationDirPath() + "/profiles.xml");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QDomDocument doc;
        QDomElement profiles = doc.createElement(QString("Profiles"));
        doc.appendChild(profiles);
        QTextStream stream( &file );
        stream << doc.toString();
        file.close();
    }
}

void MainWindow::on_actionResearcher_triggered()
{
    Researcher *r = new Researcher();
    r->show();
    r->raise();
}

void MainWindow::on_pushButtonStationary3_clicked()
{
    if (stationary3 == NULL)
    {
        stationary3 = new Stationary3();
        connect(stationary3, SIGNAL (alertProfiles(int)), this, SLOT (updateProfiles(int)));
    }
    stationary3->show();
    stationary3->raise();
}

void MainWindow::on_pushButtonWild3_clicked()
{
    if (wild3 == NULL)
    {
        wild3 = new Wild3();
        connect(wild3, SIGNAL (alertProfiles(int)), this, SLOT (updateProfiles(int)));
    }
    wild3->show();
    wild3->raise();
}

void MainWindow::on_pushButtonEgg3_clicked()
{
    if (egg3 == NULL)
    {
        egg3 = new Eggs3();
        connect(egg3, SIGNAL (alertProfiles(int)), this, SLOT (updateProfiles(int)));
    }
    egg3->show();
    egg3->raise();
}

void MainWindow::on_pushButtonIDs3_clicked()
{
    if (ids3 == NULL)
    {
        ids3 = new IDs3();
    }
    ids3->show();
    ids3->raise();
}

void MainWindow::on_action16BitSeedtoTime_triggered()
{
    SeedToTime3 *seedToTime = new SeedToTime3();
    seedToTime->show();
    seedToTime->raise();
}

void MainWindow::on_actionJirachiPattern_triggered()
{
    JirachiPattern *jirachi = new JirachiPattern();
    jirachi->show();
    jirachi->raise();
}

void MainWindow::on_actionPokeSpot_triggered()
{
    PokeSpot *pokeSpot = new PokeSpot();
    pokeSpot->show();
    pokeSpot->raise();
}

void MainWindow::on_actionIVtoPID_triggered()
{
    IVtoPID *ivToPID = new IVtoPID();
    ivToPID->show();
    ivToPID->raise();
}

void MainWindow::on_actionGameCubeRTC_triggered()
{
    GameCubeRTC *rtc = new GameCubeRTC();
    rtc->show();
    rtc->raise();
}

void MainWindow::on_actionPIDtoIV_triggered()
{
    PIDtoIVs *pidToIV = new PIDtoIVs();
    if (stationary3 != NULL)
    {
        connect(pidToIV, &PIDtoIVs::moveResultsToStationary, stationary3, &Stationary3::moveResults);
    }
    pidToIV->show();
    pidToIV->raise();
}

void MainWindow::on_pushButtonStationary4_clicked()
{
    if (stationary4 == NULL)
    {
        stationary4 = new Stationary4();
        connect(stationary4, SIGNAL (alertProfiles(int)), this, SLOT (updateProfiles(int)));
    }
    stationary4->show();
    stationary4->raise();
}

void MainWindow::on_pushButtonWild4_clicked()
{
    if (wild4 == NULL)
    {
        wild4 = new Wild4();
        connect(wild4, SIGNAL (alertProfiles(int)), this, SLOT (updateProfiles(int)));
    }
    wild4->show();
    wild4->raise();
}

void MainWindow::on_pushButtonEgg4_clicked()
{
    if (egg4 == NULL)
    {
        egg4 = new Eggs4();
        connect(egg4, SIGNAL (alertProfiles(int)), this, SLOT (updateProfiles(int)));
    }
    egg4->show();
    egg4->raise();
}

void MainWindow::on_pushButtonIDs4_clicked()
{
    if (ids4 == NULL)
    {
        ids4 = new IDs4();
    }
    ids4->show();
    ids4->raise();
}

void MainWindow::on_actionSeed_to_Time_triggered()
{
    SeedtoTime4 *seedToTime = new SeedtoTime4();
    seedToTime->show();
    seedToTime->raise();
}

void MainWindow::on_actionIV_to_PID_triggered()
{
    IVtoPID *ivToPID = new IVtoPID();
    ivToPID->show();
    ivToPID->raise();
}
