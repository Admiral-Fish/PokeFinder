/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "ProfileEditor4.hpp"
#include "ui_ProfileEditor4.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileEditor4::ProfileEditor4(QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->checkBoxNationalDex->setVisible(false);
    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);

    ui->comboBoxVersion->setup(
        { toInt(Game::Diamond), toInt(Game::Pearl), toInt(Game::Platinum), toInt(Game::HeartGold), toInt(Game::SoulSilver) });

    connect(ui->pushButtonOkay, &QPushButton::clicked, this, &ProfileEditor4::okay);
    connect(ui->comboBoxVersion, &QComboBox::currentIndexChanged, this, &ProfileEditor4::versionIndexChanged);

    QSettings setting;
    if (setting.contains("profileEditor4/geometry"))
    {
        this->restoreGeometry(setting.value("profileEditor4/geometry").toByteArray());
    }
}

ProfileEditor4::ProfileEditor4(const Profile4 &profile, QWidget *parent) : ProfileEditor4(parent)
{
    ui->lineEditProfile->setText(QString::fromStdString(profile.getName()));
    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(toInt(profile.getVersion())));
    ui->textBoxTID->setText(QString::number(profile.getTID()));
    ui->textBoxSID->setText(QString::number(profile.getSID()));
    ui->checkBoxNationalDex->setChecked(profile.getNationalDex());
}

ProfileEditor4::~ProfileEditor4()
{
    QSettings setting;
    setting.setValue("profileEditor4/geometry", this->saveGeometry());

    delete ui;
}

Profile4 ProfileEditor4::getProfile()
{
    return Profile4(ui->lineEditProfile->text().toStdString(), ui->comboBoxVersion->getEnum<Game>(), ui->textBoxTID->getUShort(),
                    ui->textBoxSID->getUShort(), ui->checkBoxNationalDex->isChecked());
}

void ProfileEditor4::okay()
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

void ProfileEditor4::versionIndexChanged(int index)
{
    if (index >= 0)
    {
        bool flag = (ui->comboBoxVersion->getEnum<Game>() & Game::HGSS) != Game::None;
        ui->checkBoxNationalDex->setVisible(flag);
        if (!flag)
        {
            ui->checkBoxNationalDex->setChecked(false);
        }
    }
}
