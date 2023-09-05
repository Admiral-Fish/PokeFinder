/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "ProfileEditor5.hpp"
#include "ui_ProfileEditor5.h"
#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Language.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/States/ProfileSearcherState5.hpp>
#include <Form/Gen5/Profile/ProfileCalibrator5.hpp>
#include <QMessageBox>

ProfileEditor5::ProfileEditor5(QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);
    ui->textBoxMAC->setValues(0, 0xFFFFFFFFFFFF, 12, 16);
    ui->textBoxVCount->setValues(0, 0xFF, 2, 16);
    ui->textBoxGxStat->setValues(0, 99, 2, 16);
    ui->textBoxVFrame->setValues(0, 99, 2, 16);
    ui->textBoxTimer0Min->setValues(InputType::Seed16Bit);
    ui->textBoxTimer0Max->setValues(InputType::Seed16Bit);

    ui->comboBoxVersion->setup({ toInt(Game::Black), toInt(Game::White), toInt(Game::Black2), toInt(Game::White2) });

    ui->comboBoxDSType->setup({ toInt(DSType::DS), toInt(DSType::DSi), toInt(DSType::DS3) });

    ui->comboBoxLanguage->setup({ toInt(Language::English), toInt(Language::Spanish), toInt(Language::French), toInt(Language::Italian),
                                  toInt(Language::German), toInt(Language::Japanese), toInt(Language::Korean) });

    ui->comboBoxKeypresses->setup();

    connect(ui->pushButtonAccept, &QPushButton::clicked, this, &ProfileEditor5::okay);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &ProfileEditor5::reject);
    connect(ui->pushButtonFindParameters, &QPushButton::clicked, this, &ProfileEditor5::findParameters);
    connect(ui->comboBoxVersion, &QComboBox::currentIndexChanged, this, &ProfileEditor5::versionIndexChanged);

    versionIndexChanged(ui->comboBoxVersion->currentIndex());
}

ProfileEditor5::ProfileEditor5(const Profile5 &profile, QWidget *parent) : ProfileEditor5(parent)
{
    ui->lineEditProfile->setText(QString::fromStdString(profile.getName()));
    ui->textBoxTID->setText(QString::number(profile.getTID()));
    ui->textBoxSID->setText(QString::number(profile.getSID()));
    ui->textBoxMAC->setText(QString::number(profile.getMac(), 16));
    ui->textBoxVCount->setText(QString::number(profile.getVCount(), 16));
    ui->textBoxGxStat->setText(QString::number(profile.getGxStat(), 16));
    ui->textBoxVFrame->setText(QString::number(profile.getVFrame(), 16));
    ui->textBoxTimer0Min->setText(QString::number(profile.getTimer0Min(), 16));
    ui->textBoxTimer0Max->setText(QString::number(profile.getTimer0Max(), 16));

    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(toInt(profile.getVersion())));
    ui->comboBoxLanguage->setCurrentIndex(ui->comboBoxLanguage->findData(toInt(profile.getLanguage())));
    ui->comboBoxDSType->setCurrentIndex(ui->comboBoxDSType->findData(toInt(profile.getDSType())));

    ui->comboBoxKeypresses->setChecks(profile.getKeypresses());

    ui->checkBoxSkipLR->setChecked(profile.getSkipLR());
    ui->checkBoxSoftReset->setChecked(profile.getSoftReset());
    ui->checkBoxMemoryLink->setChecked(profile.getMemoryLink());
    ui->checkBoxShinyCharm->setChecked(profile.getShinyCharm());
}

ProfileEditor5::ProfileEditor5(Game version, Language language, DSType dsType, u64 mac, const ProfileSearcherState5 &calibration,
                               QWidget *parent) :
    ProfileEditor5(parent)
{
    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(toInt(version)));
    ui->comboBoxLanguage->setCurrentIndex(ui->comboBoxLanguage->findData(toInt(language)));
    ui->comboBoxDSType->setCurrentIndex(ui->comboBoxDSType->findData(toInt(dsType)));
    ui->textBoxMAC->setText(QString::number(mac, 16));
    ui->textBoxVCount->setText(QString::number(calibration.getVCount(), 16));
    ui->textBoxTimer0Min->setText(QString::number(calibration.getTimer0(), 16));
    ui->textBoxTimer0Max->setText(QString::number(calibration.getTimer0(), 16));
    ui->textBoxGxStat->setText(QString::number(calibration.getGxStat(), 16));
    ui->textBoxVFrame->setText(QString::number(calibration.getVFrame(), 16));
}

ProfileEditor5::~ProfileEditor5()
{
    delete ui;
}

Profile5 ProfileEditor5::getProfile()
{
    return Profile5(ui->lineEditProfile->text().toStdString(), ui->comboBoxVersion->getEnum<Game>(), ui->textBoxTID->getUShort(),
                    ui->textBoxSID->getUShort(), ui->textBoxMAC->getULong(), ui->comboBoxKeypresses->getCheckedArray<9>(),
                    ui->textBoxVCount->getUChar(), ui->textBoxGxStat->getUChar(), ui->textBoxVFrame->getUChar(),
                    ui->checkBoxSkipLR->isChecked(), ui->textBoxTimer0Min->getUShort(), ui->textBoxTimer0Max->getUShort(),
                    ui->checkBoxSoftReset->isChecked(), ui->checkBoxMemoryLink->isChecked(), ui->checkBoxShinyCharm->isChecked(),
                    ui->comboBoxDSType->getEnum<DSType>(), ui->comboBoxLanguage->getEnum<Language>());
}

void ProfileEditor5::okay()
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

void ProfileEditor5::findParameters()
{
    auto *calibrator = new ProfileCalibrator5();
    calibrator->show();

    done(QDialog::Rejected);
}

void ProfileEditor5::versionIndexChanged(int index)
{
    if (index >= 0)
    {
        Game version = static_cast<Game>(ui->comboBoxVersion->currentData().toUInt());
        if ((version & Game::BW2) != Game::None)
        {
            ui->checkBoxMemoryLink->setVisible(true);
            ui->checkBoxShinyCharm->setVisible(true);
        }
        else
        {
            ui->checkBoxMemoryLink->setVisible(false);
            ui->checkBoxMemoryLink->setChecked(false);
            ui->checkBoxShinyCharm->setVisible(false);
            ui->checkBoxShinyCharm->setChecked(false);
        }
    }
}
