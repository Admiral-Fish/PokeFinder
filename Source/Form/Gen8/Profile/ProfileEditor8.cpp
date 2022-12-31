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
#include <Core/Gen8/Profile8.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileEditor8::ProfileEditor8(QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);

    ui->comboBoxVersion->setup({ toInt(Game::Sword), toInt(Game::Shield), toInt(Game::BD), toInt(Game::SP) });

    connect(ui->pushButtonOkay, &QPushButton::clicked, this, &ProfileEditor8::okay);

    QSettings setting;
    if (setting.contains("profileEditor8/geometry"))
    {
        this->restoreGeometry(setting.value("profileEditor8/geometry").toByteArray());
    }
}

ProfileEditor8::ProfileEditor8(const Profile8 &profile, QWidget *parent) : ProfileEditor8(parent)
{
    ui->lineEditProfile->setText(QString::fromStdString(profile.getName()));
    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(toInt(profile.getVersion())));
    ui->textBoxTID->setText(QString::number(profile.getTID()));
    ui->textBoxSID->setText(QString::number(profile.getSID()));
    ui->checkBoxNationalDex->setChecked(profile.getNationalDex());
    ui->checkBoxShinyCharm->setChecked(profile.getShinyCharm());
    ui->checkBoxOvalCharm->setChecked(profile.getOvalCharm());
}

ProfileEditor8::~ProfileEditor8()
{
    QSettings setting;
    setting.setValue("profileEditor8/geometry", this->saveGeometry());

    delete ui;
}

Profile8 ProfileEditor8::getProfile()
{
    return Profile8(ui->lineEditProfile->text().toStdString(), ui->comboBoxVersion->getEnum<Game>(), ui->textBoxTID->getUShort(),
                    ui->textBoxSID->getUShort(), ui->checkBoxNationalDex->isChecked(), ui->checkBoxShinyCharm->isChecked(),
                    ui->checkBoxOvalCharm->isChecked());
}

void ProfileEditor8::okay()
{
    QString input = ui->lineEditProfile->text().trimmed();
    if (input.isEmpty())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing name"), tr("Enter a profile name"));
        msg.exec();
        return;
    }

    done(QDialog::Accepted);
}
