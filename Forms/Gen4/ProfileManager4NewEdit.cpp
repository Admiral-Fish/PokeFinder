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

#include "ProfileManager4NewEdit.hpp"
#include "ui_ProfileManager4NewEdit.h"

ProfileManager4NewEdit::ProfileManager4NewEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileManager4NewEdit)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

ProfileManager4NewEdit::ProfileManager4NewEdit(Profile4 profile, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileManager4NewEdit)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();

    ui->lineEditProfile->setText(profile.getProfileName());
    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(profile.getVersion()));
    ui->comboBoxLanguage->setCurrentIndex(profile.getLanguage());
    ui->textBoxTID->setText(QString::number(profile.getTID()));
    ui->textBoxSID->setText(QString::number(profile.getSID()));
    ui->comboBoxDualSlot->setCurrentIndex(ui->comboBoxDualSlot->findData(profile.getDualSlot()));
    ui->comboBoxRadio->setCurrentIndex(profile.getRadio());
    isEditing = true;
    original = profile;
}

ProfileManager4NewEdit::~ProfileManager4NewEdit()
{
    delete ui;
}

Profile4 ProfileManager4NewEdit::getNewProfile()
{
    return fresh;
}

Profile4 ProfileManager4NewEdit::getOriginal()
{
    return original;
}

void ProfileManager4NewEdit::changeEvent(QEvent *event)
{
    if (event)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
            default:
                break;
        }
    }
}

void ProfileManager4NewEdit::setupModels()
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
}

void ProfileManager4NewEdit::on_pushButtonAccept_clicked()
{
    QString input = ui->lineEditProfile->text().trimmed();
    if (input == "")
    {
        QMessageBox error;
        error.setText(tr("Enter a Profile Name."));
        error.exec();
        return;
    }

    fresh = Profile4(ui->lineEditProfile->text(), static_cast<Game>(ui->comboBoxVersion->currentData().toInt()), ui->textBoxTID->text().toUShort(),
                     ui->textBoxSID->text().toUShort(), static_cast<Game>(ui->comboBoxDualSlot->currentData().toInt()), ui->comboBoxRadio->currentIndex(),
                     ui->comboBoxLanguage->currentIndex());

    done(QDialog::Accepted);
}


void ProfileManager4NewEdit::on_pushButtonCancel_clicked()
{
    done(QDialog::Rejected);
}

void ProfileManager4NewEdit::on_comboBoxVersion_currentIndexChanged(int index)
{
    (void)index;

    Game game = static_cast<Game>(ui->comboBoxVersion->currentData().toInt());
    if (game == Game::HeartGold || game == Game::SoulSilver)
    {
        ui->comboBoxRadio->setEnabled(true);
    }
    else
    {
        ui->comboBoxRadio->setEnabled(false);
        ui->comboBoxRadio->setCurrentIndex(0);
    }
}
