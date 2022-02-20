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

#include "ProfileEditor5.hpp"
#include "ui_ProfileEditor5.h"
#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Game.hpp>
#include <Forms/Gen5/Profile/ProfileCalibrator5.hpp>
#include <QMessageBox>

ProfileEditor5::ProfileEditor5(QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
    versionIndexChanged(ui->comboBoxVersion->currentIndex());
}

ProfileEditor5::ProfileEditor5(const Profile5 &profile, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileEditor5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();

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
    auto keypress = profile.getKeypresses();
    ui->comboBoxKeypresses->setChecks(std::vector<bool>(keypress.begin(), keypress.end()));

    ui->checkBoxSkipLR->setChecked(profile.getSkipLR());
    ui->checkBoxSoftReset->setChecked(profile.getSoftReset());
    ui->checkBoxMemoryLink->setChecked(profile.getMemoryLink());
    ui->checkBoxShinyCharm->setChecked(profile.getShinyCharm());

    isEditing = true;
    original = profile;

    versionIndexChanged(ui->comboBoxVersion->currentIndex());
}

ProfileEditor5::ProfileEditor5(Game version, Language language, DSType dsType, u64 mac, u8 vcount, u16 timer0, u8 gxstat, u8 vframe,
                               QWidget *parent) :
    QDialog(parent), ui(new Ui::ProfileEditor5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();

    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(toInt(version)));
    ui->comboBoxLanguage->setCurrentIndex(ui->comboBoxLanguage->findData(toInt(language)));
    ui->comboBoxDSType->setCurrentIndex(ui->comboBoxDSType->findData(toInt(dsType)));
    ui->textBoxMAC->setText(QString::number(mac, 16));
    ui->textBoxVCount->setText(QString::number(vcount, 16));
    ui->textBoxTimer0Min->setText(QString::number(timer0, 16));
    ui->textBoxTimer0Max->setText(QString::number(timer0, 16));
    ui->textBoxGxStat->setText(QString::number(gxstat, 16));
    ui->textBoxVFrame->setText(QString::number(vframe, 16));

    versionIndexChanged(ui->comboBoxVersion->currentIndex());
}

ProfileEditor5::~ProfileEditor5()
{
    delete ui;
}

Profile5 ProfileEditor5::getNewProfile()
{
    return fresh;
}

Profile5 ProfileEditor5::getOriginal()
{
    return original;
}

void ProfileEditor5::setupModels()
{
    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);
    ui->textBoxMAC->setValues(0, 0xFFFFFFFFFFFF, 12, 16);
    ui->textBoxVCount->setValues(0, 0xFF, 2, 16);
    ui->textBoxGxStat->setValues(0, 99, 2, 16);
    ui->textBoxVFrame->setValues(0, 99, 2, 16);
    ui->textBoxTimer0Min->setValues(InputType::Seed16Bit);
    ui->textBoxTimer0Max->setValues(InputType::Seed16Bit);

    ui->comboBoxVersion->setup({ toInt(Game::Black), toInt(Game::White), toInt(Game::Black2), toInt(Game::White2) });

    ui->comboBoxDSType->setup({ toInt(DSType::DS), toInt(DSType::DSi), 2, toInt(DSType::DS3) });

    ui->comboBoxLanguage->setup({ toInt(Language::English), toInt(Language::Spanish), toInt(Language::French), toInt(Language::Italian),
                                  toInt(Language::German), toInt(Language::Japanese), toInt(Language::Korean) });

    ui->comboBoxKeypresses->setup();

    connect(ui->pushButtonAccept, &QPushButton::clicked, this, &ProfileEditor5::okay);
    connect(ui->pushButtonFindParameters, &QPushButton::clicked, this, &ProfileEditor5::findParameters);
    connect(ui->comboBoxVersion, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProfileEditor5::versionIndexChanged);
}

void ProfileEditor5::okay()
{
    QString input = ui->lineEditProfile->text().trimmed();
    if (input.isEmpty())
    {
        QMessageBox error;
        error.setText(tr("Enter a profile name"));
        error.exec();
        return;
    }

    fresh = Profile5(ui->lineEditProfile->text().toStdString(), static_cast<Game>(ui->comboBoxVersion->currentData().toUInt()),
                     ui->textBoxTID->getUShort(), ui->textBoxSID->getUShort(), ui->textBoxMAC->getULong(),
                     ui->comboBoxKeypresses->getChecked(), ui->textBoxVCount->getUChar(), ui->textBoxGxStat->getUChar(),
                     ui->textBoxVFrame->getUChar(), ui->checkBoxSkipLR->isChecked(), ui->textBoxTimer0Min->getUShort(),
                     ui->textBoxTimer0Max->getUShort(), ui->checkBoxSoftReset->isChecked(), ui->checkBoxMemoryLink->isChecked(),
                     ui->checkBoxShinyCharm->isChecked(), static_cast<DSType>(ui->comboBoxDSType->currentData().toInt()),
                     static_cast<Language>(ui->comboBoxLanguage->currentData().toInt()));

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
