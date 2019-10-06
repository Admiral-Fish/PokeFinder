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
#include "ProfileEditor3.hpp"
#include "ui_ProfileEditor3.h"

namespace PokeFinderForms
{
    ProfileEditor3::ProfileEditor3(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ProfileEditor3)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);

        setupModels();
    }

    ProfileEditor3::ProfileEditor3(const PokeFinderCore::Profile3 &profile, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ProfileEditor3)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);

        setupModels();

        ui->lineEditProfile->setText(profile.getProfileName());
        ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(profile.getVersion()));
        ui->comboBoxLanguage->setCurrentIndex(ui->comboBoxLanguage->findData(profile.getLanguage()));
        ui->textBoxTID->setText(QString::number(profile.getTID()));
        ui->textBoxSID->setText(QString::number(profile.getSID()));
        ui->checkBoxDeadBattery->setChecked(profile.getDeadBattery());

        isEditing = true;
        original = profile;
    }

    ProfileEditor3::~ProfileEditor3()
    {
        QSettings setting;
        setting.setValue("profileEditor3/geometry", this->saveGeometry());

        delete ui;
    }

    PokeFinderCore::Profile3 ProfileEditor3::getNewProfile()
    {
        return fresh;
    }

    PokeFinderCore::Profile3 ProfileEditor3::getOriginal()
    {
        return original;
    }

    void ProfileEditor3::setupModels()
    {
        ui->textBoxTID->setValues(InputType::TIDSID);
        ui->textBoxSID->setValues(InputType::TIDSID);

        ui->comboBoxVersion->setItemData(0, PokeFinderCore::Game::Ruby);
        ui->comboBoxVersion->setItemData(1, PokeFinderCore::Game::Sapphire);
        ui->comboBoxVersion->setItemData(2, PokeFinderCore::Game::FireRed);
        ui->comboBoxVersion->setItemData(3, PokeFinderCore::Game::LeafGreen);
        ui->comboBoxVersion->setItemData(4, PokeFinderCore::Game::Emerald);
        ui->comboBoxVersion->setItemData(5, PokeFinderCore::Game::Gales);
        ui->comboBoxVersion->setItemData(6, PokeFinderCore::Game::Colosseum);

        ui->comboBoxLanguage->setItemData(0, PokeFinderCore::Language::English);
        ui->comboBoxLanguage->setItemData(1, PokeFinderCore::Language::Spanish);
        ui->comboBoxLanguage->setItemData(2, PokeFinderCore::Language::French);
        ui->comboBoxLanguage->setItemData(3, PokeFinderCore::Language::Italian);
        ui->comboBoxLanguage->setItemData(4, PokeFinderCore::Language::German);
        ui->comboBoxLanguage->setItemData(5, PokeFinderCore::Language::Japanese);

        QSettings setting;
        if (setting.contains("profileEditor3/geometry")) this->restoreGeometry(setting.value("profileEditor3/geometry").toByteArray());
    }

    void ProfileEditor3::on_pushButtonAccept_clicked()
    {
        QString input = ui->lineEditProfile->text().trimmed();
        if (input.isEmpty())
        {
            QMessageBox error;
            error.setText(tr("Enter a Profile Name."));
            error.exec();
            return;
        }

        fresh = PokeFinderCore::Profile3(ui->lineEditProfile->text(), static_cast<PokeFinderCore::Game>(ui->comboBoxVersion->currentData().toInt()), ui->textBoxTID->getUShort(),
                                         ui->textBoxSID->getUShort(), static_cast<PokeFinderCore::Language>(ui->comboBoxLanguage->currentData().toInt()), ui->checkBoxDeadBattery->isChecked());

        done(QDialog::Accepted);
    }

    void ProfileEditor3::on_comboBoxVersion_currentIndexChanged(int /*index*/)
    {
        bool flag = ui->comboBoxVersion->currentData().toInt() & PokeFinderCore::Game::RS;
        ui->checkBoxDeadBattery->setVisible(flag);
        if (!flag)
        {
            ui->checkBoxDeadBattery->setChecked(false);
        }
    }
}
