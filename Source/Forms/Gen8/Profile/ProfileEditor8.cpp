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

#include "ProfileEditor8.hpp"
#include "ui_ProfileEditor8.h"
#include <Core/Enum/Game.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileEditor8::ProfileEditor8(QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
}

ProfileEditor8::ProfileEditor8(const Profile8 &profile, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();

    ui->lineEditProfile->setText(QString::fromStdString(profile.getName()));
    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(toInt(profile.getVersion())));
    ui->textBoxTID->setText(QString::number(profile.getTID()));
    ui->textBoxSID->setText(QString::number(profile.getSID()));
    ui->checkBoxShinyCharm->setChecked(profile.getShinyCharm());
    ui->checkBoxOvalCharm->setChecked(profile.getOvalCharm());
    ui->checkBoxRadar->setChecked(profile.getRadar());
    ui->checkBoxSwarm->setChecked(profile.getSwarm());

    isEditing = true;
    original = profile;
}

ProfileEditor8::~ProfileEditor8()
{
    QSettings setting;
    setting.setValue("profileEditor8/geometry", this->saveGeometry());

    delete ui;
}

Profile8 ProfileEditor8::getNewProfile()
{
    return fresh;
}

Profile8 ProfileEditor8::getOriginal()
{
    return original;
}

void ProfileEditor8::setupModels()
{
    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);

    ui->comboBoxVersion->setup({ toInt(Game::Sword), toInt(Game::Shield), toInt(Game::BD), toInt(Game::SP), toInt(Game::PLA) });

    connect(ui->pushButtonOkay, &QPushButton::clicked, this, &ProfileEditor8::okay);
    connect(ui->comboBoxVersion, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfileEditor8::versionIndexChanged);

    QSettings setting;
    if (setting.contains("profileEditor8/geometry"))
    {
        this->restoreGeometry(setting.value("profileEditor8/geometry").toByteArray());
    }
}

void ProfileEditor8::okay()
{
    QString input = ui->lineEditProfile->text().trimmed();
    if (input.isEmpty())
    {
        QMessageBox error;
        error.setText(tr("Enter a Profile Name."));
        error.exec();
        return;
    }

    fresh = Profile8(ui->lineEditProfile->text().toStdString(), static_cast<Game>(ui->comboBoxVersion->currentData().toUInt()),
                     ui->textBoxTID->getUShort(), ui->textBoxSID->getUShort(), ui->checkBoxShinyCharm->isChecked(),
                     ui->checkBoxOvalCharm->isChecked(), ui->checkBoxRadar->isChecked(), ui->checkBoxSwarm->isChecked());

    done(QDialog::Accepted);
}

void ProfileEditor8::versionIndexChanged(int index)
{
    if (index >= 0)
    {
        auto game = static_cast<Game>(ui->comboBoxVersion->currentData().toUInt());
        bool flag = (game & Game::BDSP) != Game::None;

        ui->checkBoxRadar->setVisible(flag);
        ui->checkBoxSwarm->setVisible(flag);

        flag = game != Game::PLA;

        ui->checkBoxOvalCharm->setVisible(flag);
    }
}
