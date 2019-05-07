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

#include <QMessageBox>
#include <QSettings>
#include "ProfileEditor4.hpp"
#include "ui_ProfileEditor4.h"

ProfileEditor4::ProfileEditor4(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileEditor4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->labelRadio->setVisible(false);
    ui->comboBoxRadio->setVisible(false);

    setupModels();
}

ProfileEditor4::ProfileEditor4(const Profile4 &profile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileEditor4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();

    ui->lineEditProfile->setText(profile.getProfileName());
    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(profile.getVersion()));
    ui->comboBoxLanguage->setCurrentIndex(ui->comboBoxLanguage->findData(profile.getLanguage()));
    ui->textBoxTID->setText(QString::number(profile.getTID()));
    ui->textBoxSID->setText(QString::number(profile.getSID()));
    ui->comboBoxDualSlot->setCurrentIndex(ui->comboBoxDualSlot->findData(profile.getDualSlot()));
    ui->comboBoxRadio->setCurrentIndex(profile.getRadio());
    ui->checkBoxRadar->setChecked(profile.getRadar());
    ui->checkBoxSwarm->setChecked(profile.getSwarm());

    isEditing = true;
    original = profile;
}

ProfileEditor4::~ProfileEditor4()
{
    QSettings setting;
    setting.setValue("profileEditor4/size", this->size());

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

    ui->comboBoxVersion->setItemData(0, Game::Diamond);
    ui->comboBoxVersion->setItemData(1, Game::Pearl);
    ui->comboBoxVersion->setItemData(2, Game::Platinum);
    ui->comboBoxVersion->setItemData(3, Game::HeartGold);
    ui->comboBoxVersion->setItemData(4, Game::SoulSilver);

    ui->comboBoxDualSlot->setItemData(0, Game::Blank);
    ui->comboBoxDualSlot->setItemData(1, Game::Ruby);
    ui->comboBoxDualSlot->setItemData(2, Game::Sapphire);
    ui->comboBoxDualSlot->setItemData(3, Game::FireRed);
    ui->comboBoxDualSlot->setItemData(4, Game::LeafGreen);
    ui->comboBoxDualSlot->setItemData(5, Game::Emerald);

    ui->comboBoxLanguage->setItemData(0, Language::English);
    ui->comboBoxLanguage->setItemData(1, Language::Spanish);
    ui->comboBoxLanguage->setItemData(2, Language::French);
    ui->comboBoxLanguage->setItemData(3, Language::Italian);
    ui->comboBoxLanguage->setItemData(4, Language::German);
    ui->comboBoxLanguage->setItemData(5, Language::Japanese);
    ui->comboBoxLanguage->setItemData(6, Language::Korean);

    QSettings setting;
    if (setting.contains("profileEditor4/size")) this->resize(setting.value("profileEditor4/size").toSize());
}

void ProfileEditor4::on_pushButtonAccept_clicked()
{
    QString input = ui->lineEditProfile->text().trimmed();
    if (input.isEmpty())
    {
        QMessageBox error;
        error.setText(tr("Enter a profile name"));
        error.exec();
        return;
    }

    fresh = Profile4(ui->lineEditProfile->text(), static_cast<Game>(ui->comboBoxVersion->currentData().toInt()), ui->textBoxTID->getUShort(),
                     ui->textBoxSID->getUShort(), static_cast<Game>(ui->comboBoxDualSlot->currentData().toInt()), ui->comboBoxRadio->currentIndex(),
                     static_cast<Language>(ui->comboBoxLanguage->currentData().toInt()), ui->checkBoxRadar->isChecked(), ui->checkBoxSwarm->isChecked());

    done(QDialog::Accepted);
}

void ProfileEditor4::on_comboBoxVersion_currentIndexChanged(int index)
{
    (void)index;

    Game game = static_cast<Game>(ui->comboBoxVersion->currentData().toInt());
    bool flag = game & Game::HGSS;

    ui->labelRadio->setVisible(flag);
    ui->comboBoxRadio->setVisible(flag);

    ui->labelRadar->setVisible(!flag);
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
