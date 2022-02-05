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

#include "ProfileEditor4.hpp"
#include "ui_ProfileEditor4.h"
#include <QMessageBox>
#include <QSettings>

ProfileEditor4::ProfileEditor4(QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->labelRadio->setVisible(false);
    ui->comboBoxRadio->setVisible(false);

    setupModels();
}

ProfileEditor4::ProfileEditor4(const Profile4 &profile, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();

    ui->lineEditProfile->setText(QString::fromStdString(profile.getName()));
    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(toInt(profile.getVersion())));
    ui->textBoxTID->setText(QString::number(profile.getTID()));
    ui->textBoxSID->setText(QString::number(profile.getSID()));
    ui->comboBoxDualSlot->setCurrentIndex(ui->comboBoxDualSlot->findData(toInt(profile.getDualSlot())));
    ui->comboBoxRadio->setCurrentIndex(profile.getRadio());
    ui->checkBoxRadar->setChecked(profile.getRadar());
    ui->checkBoxSwarm->setChecked(profile.getSwarm());

    isEditing = true;
    original = profile;
}

ProfileEditor4::~ProfileEditor4()
{
    QSettings setting;
    setting.setValue("profileEditor4/geometry", this->saveGeometry());

    delete ui;
}

Profile4 ProfileEditor4::getNewProfile()
{
    return fresh;
}

Profile4 ProfileEditor4::getOriginal()
{
    return original;
}

void ProfileEditor4::setupModels()
{
    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);

    ui->comboBoxVersion->setup(
        { toInt(Game::Diamond), toInt(Game::Pearl), toInt(Game::Platinum), toInt(Game::HeartGold), toInt(Game::SoulSilver) });

    ui->comboBoxDualSlot->setup({ toInt(Game::None), toInt(Game::Ruby), toInt(Game::Sapphire), toInt(Game::FireRed), toInt(Game::LeafGreen),
                                  toInt(Game::Emerald) });

    connect(ui->pushButtonOkay, &QPushButton::clicked, this, &ProfileEditor4::okay);
    connect(ui->comboBoxVersion, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfileEditor4::versionIndexChanged);

    QSettings setting;
    if (setting.contains("profileEditor4/geometry"))
    {
        this->restoreGeometry(setting.value("profileEditor4/geometry").toByteArray());
    }
}

void ProfileEditor4::okay()
{
    QString input = ui->lineEditProfile->text().trimmed();
    if (input.isEmpty())
    {
        QMessageBox error;
        error.setText(tr("Enter a profile name"));
        error.exec();
        return;
    }

    fresh = Profile4(ui->lineEditProfile->text().toStdString(), static_cast<Game>(ui->comboBoxVersion->getCurrentUInt()),
                     ui->textBoxTID->getUShort(), ui->textBoxSID->getUShort(), static_cast<Game>(ui->comboBoxDualSlot->getCurrentUInt()),
                     ui->comboBoxRadio->currentIndex(), ui->checkBoxRadar->isChecked(), ui->checkBoxSwarm->isChecked());

    done(QDialog::Accepted);
}

void ProfileEditor4::versionIndexChanged(int index)
{
    if (index >= 0)
    {
        auto game = static_cast<Game>(ui->comboBoxVersion->currentData().toUInt());
        bool flag = (game & Game::HGSS) != Game::None;

        ui->labelRadio->setVisible(flag);
        ui->comboBoxRadio->setVisible(flag);

        ui->checkBoxRadar->setVisible(!flag);

        ui->labelDualSlot->setVisible(!flag);
        ui->comboBoxDualSlot->setVisible(!flag);

        if (flag)
        {
            ui->comboBoxDualSlot->setCurrentIndex(0);
            ui->checkBoxRadar->setChecked(false);
        }
        else
        {
            ui->comboBoxRadio->setCurrentIndex(0);
        }
    }
}
