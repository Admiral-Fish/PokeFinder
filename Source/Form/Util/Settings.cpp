/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Settings.hpp"
#include "ui_Settings.h"
#include <Core/Parents/ProfileLoader.hpp>
#include <QApplication>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QThread>

Settings::Settings(QWidget *parent) : QWidget(parent), ui(new Ui::Settings)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    QSettings setting;
    setting.beginGroup("settings");

    // Language
    QString language = setting.value("locale").toString();
    QStringList languages = { "zh", "en", "fr", "de", "it", "ja", "ko", "es" };
    for (int i = 0; i < languages.size(); i++)
    {
        const QString &lang = languages[i];
        ui->comboBoxLanguage->setItemData(i, lang);
        if (language == lang)
        {
            ui->comboBoxLanguage->setCurrentIndex(i);
        }
    }

    // Style
    QString style = setting.value("style").toString();
    QStringList styles = { "dark", "light" };
    for (int i = 0; i < styles.size(); i++)
    {
        const QString &sty = styles[i];
        ui->comboBoxStyle->setItemData(i, sty);
        if (style == sty)
        {
            ui->comboBoxStyle->setCurrentIndex(i);
        }
    }

    // Threads
    int threads = setting.value("threads").toInt();
    for (int i = 1; i <= QThread::idealThreadCount(); i++)
    {
        ui->comboBoxThreads->addItem(QString::number(i), i);
        if (i == threads)
        {
            ui->comboBoxThreads->setCurrentIndex(i - 1);
        }
    }

    // Profiles
    QString profile = setting.value("profiles").toString();
    ui->lineEditProfiles->setText(profile);

    // IV Bin
    ui->lineEditIVBin->setText(setting.value("ivs", "").toString());

    setting.endGroup();

    connect(ui->comboBoxLanguage, &QComboBox::currentIndexChanged, this, &Settings::languageIndexChanged);
    connect(ui->comboBoxStyle, &QComboBox::currentIndexChanged, this, &Settings::styleIndexChanged);
    connect(ui->comboBoxThreads, &QComboBox::currentIndexChanged, this, &Settings::threadsIndexChanged);
    connect(ui->pushButtonProfile, &QPushButton::clicked, this, &Settings::changeProfiles);
    connect(ui->pushButtonIVBin, &QPushButton::clicked, this, &Settings::changeIVs);

    if (setting.contains("settingsForm/geometry"))
    {
        this->restoreGeometry(setting.value("settingsForm/geometry").toByteArray());
    }
}

Settings::~Settings()
{
    QSettings setting;
    setting.setValue("settingsForm/geometry", this->saveGeometry());

    delete ui;
}

void Settings::changeIVs()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select the IV bin file", QDir::currentPath(), "bin (*.bin)");
    if (!fileName.isEmpty())
    {
        QFile f(fileName);
        if (f.open(QIODevice::ReadOnly))
        {
            QByteArray hash = QCryptographicHash::hash(f.readAll(), QCryptographicHash::Sha256).toHex();
            if (hash != "c49210a31edb77ae54ddce028f8e4081947b0b5315c70e6eefda8ff88d3d70c1")
            {
                QMessageBox msg(QMessageBox::Information, tr("IV bin"), tr("Invalid IV bin file was provided"));
                msg.exec();
                return;
            }

            ui->lineEditIVBin->setText(fileName);

            QSettings setting;
            setting.setValue("settings/ivs", fileName);
        }
        else
        {
            QMessageBox msg(QMessageBox::Information, tr("IV bin"), tr("There was a problem opening the file"));
            msg.exec();
        }
    }
}

void Settings::changeProfiles()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Select Profile json", QDir::currentPath(), "json (*.json)");
    if (!fileName.isEmpty())
    {
        if (!QFile::exists(fileName))
        {
            QFile f(fileName);
            if (!f.open(QIODevice::WriteOnly))
            {
                QMessageBox msg(QMessageBox::Information, tr("Profile File"), tr("There was a problem creating the file"));
                msg.exec();
                return;
            }
        }

        QSettings setting;
        setting.setValue("settings/profiles", fileName);

        ProfileLoader::init(fileName.toStdString());

        ui->lineEditProfiles->setText(fileName);
    }
}

void Settings::languageIndexChanged(int index)
{
    if (index >= 0)
    {
        QSettings setting;
        QString currentLanguage = setting.value("settings/locale").toString();
        QString language = ui->comboBoxLanguage->currentData().toString();

        if (currentLanguage != language)
        {
            setting.setValue("settings/locale", language);

            QMessageBox msg(QMessageBox::Question, tr("Language update"), tr("Restart for changes to take effect. Restart now?"),
                            QMessageBox::Yes | QMessageBox::No);
            if (msg.exec() == QMessageBox::Yes)
            {
                QProcess::startDetached(QApplication::applicationFilePath());
                QApplication::quit();
            }
        }
    }
}

void Settings::styleIndexChanged(int index)
{
    if (index >= 0)
    {
        QSettings setting;
        QString currentStyle = setting.value("settings/style").toString();
        QString style = ui->comboBoxStyle->currentData().toString();
        if (currentStyle != style)
        {
            setting.setValue("settings/style", style);

            QMessageBox msg(QMessageBox::Question, tr("Style change"), tr("Restart for changes to take effect. Restart now?"),
                            QMessageBox::Yes | QMessageBox::No);
            if (msg.exec() == QMessageBox::Yes)
            {
                QProcess::startDetached(QApplication::applicationFilePath());
                QApplication::quit();
            }
        }
    }
}

void Settings::threadsIndexChanged(int index)
{
    if (index >= 0)
    {
        QSettings setting;
        setting.setValue("settings/threads", ui->comboBoxThreads->currentData().toInt());
    }
}
