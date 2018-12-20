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

ProfileManager3NewEdit::ProfileManager3NewEdit(const Profile3 &profile, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileManager3NewEdit)
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
    ui->checkBoxDeadBattery->setChecked(profile.getDeadBattery());

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

void ProfileManager3NewEdit::setupModels()
{
    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSID->setValues(InputType::TIDSID);

    ui->comboBoxVersion->setItemData(0, Game::Ruby);
    ui->comboBoxVersion->setItemData(1, Game::Sapphire);
    ui->comboBoxVersion->setItemData(2, Game::FireRed);
    ui->comboBoxVersion->setItemData(3, Game::LeafGreen);
    ui->comboBoxVersion->setItemData(4, Game::Emerald);
    ui->comboBoxVersion->setItemData(5, Game::Gales);
    ui->comboBoxVersion->setItemData(6, Game::Colosseum);
}

void ProfileManager3NewEdit::on_pushButtonAccept_clicked()
{
    QString input = ui->lineEditProfile->text().trimmed();
    if (input.isEmpty())
    {
        QMessageBox error;
        error.setText(tr("Enter a Profile Name."));
        error.exec();
        return;
    }

    fresh = Profile3(ui->lineEditProfile->text(), static_cast<Game>(ui->comboBoxVersion->currentData().toInt()), ui->textBoxTID->text().toUShort(),
                     ui->textBoxSID->text().toUShort(), ui->comboBoxLanguage->currentIndex(), ui->checkBoxDeadBattery->isChecked());

    done(QDialog::Accepted);
}

void ProfileManager3NewEdit::on_comboBoxVersion_currentIndexChanged(int index)
{
    (void) index;

    bool flag = ui->comboBoxVersion->currentData().toInt() & Game::RS;
    ui->labelDeadBattery->setVisible(true);
    ui->checkBoxDeadBattery->setVisible(true);
    if (!flag)
    {
        ui->checkBoxDeadBattery->setChecked(false);
    }

}
