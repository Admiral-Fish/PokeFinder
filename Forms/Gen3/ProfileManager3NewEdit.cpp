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

#include "ProfileManager3NewEdit.hpp"
#include "ui_ProfileManager3NewEdit.h"

ProfileManager3NewEdit::ProfileManager3NewEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileManager3NewEdit)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

ProfileManager3NewEdit::ProfileManager3NewEdit(Profile3 profile, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileManager3NewEdit)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();

    ui->lineEditProfile->setText(profile.profileName);
    ui->comboBoxVersion->setCurrentIndex(ui->comboBoxVersion->findData(profile.version));
    ui->comboBoxLanguage->setCurrentIndex(profile.language);
    ui->textBoxTID->setText(QString::number(profile.tid));
    ui->textBoxSID->setText(QString::number(profile.sid));
    ui->checkBoxDeadBattery->setChecked(profile.deadBattery);
    isEditing = true;
    original = profile;
}

ProfileManager3NewEdit::~ProfileManager3NewEdit()
{
    delete ui;
}

Profile3 ProfileManager3NewEdit::getNewProfile()
{
    return fresh;
}

Profile3 ProfileManager3NewEdit::getOriginal()
{
    return original;
}

void ProfileManager3NewEdit::changeEvent(QEvent *event)
{
    if (event != NULL)
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

void ProfileManager3NewEdit::setupModels()
{
    ui->textBoxTID->setValues(0, 48, true);
    ui->textBoxSID->setValues(0, 48, true);

    ui->comboBoxVersion->setItemData(0, Ruby);
    ui->comboBoxVersion->setItemData(1, Sapphire);
    ui->comboBoxVersion->setItemData(2, FireRed);
    ui->comboBoxVersion->setItemData(3, LeafGreen);
    ui->comboBoxVersion->setItemData(4, Emerald);
    ui->comboBoxVersion->setItemData(5, Gales);
    ui->comboBoxVersion->setItemData(6, Colosseum);
}

void ProfileManager3NewEdit::on_pushButtonAccept_clicked()
{
    QString input = ui->lineEditProfile->text().trimmed();
    if (input == "")
    {
        QMessageBox error;
        error.setText(tr("Enter a Profile Name."));
        error.exec();
        return;
    }

    fresh = Profile3(ui->lineEditProfile->text(), (Game)ui->comboBoxVersion->currentData().toInt(), ui->textBoxTID->text().toUInt(NULL, 10),
                     ui->textBoxSID->text().toUInt(NULL, 10), ui->comboBoxLanguage->currentIndex(), ui->checkBoxDeadBattery->isChecked());

    done(QDialog::Accepted);
}


void ProfileManager3NewEdit::on_pushButtonCancel_clicked()
{
    done(QDialog::Rejected);
}

void ProfileManager3NewEdit::on_comboBoxVersion_currentIndexChanged(int index)
{
    if (index > 1)
    {
        ui->checkBoxDeadBattery->setEnabled(false);
        ui->checkBoxDeadBattery->setChecked(false);
    }
    else
    {
        ui->checkBoxDeadBattery->setEnabled(true);
    }
}
