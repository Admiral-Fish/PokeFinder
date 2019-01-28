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

#include "ProfileManager4.hpp"
#include "ui_ProfileManager4.h"

ProfileManager4::ProfileManager4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileManager4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

ProfileManager4::~ProfileManager4()
{
    delete ui;
}

void ProfileManager4::setupModels()
{
    model = new Profile4Model(ui->tableView);
    model->setModel(Profile4::loadProfileList());
    ui->tableView->setModel(model);
}

void ProfileManager4::on_pushButtonNew_clicked()
{
    auto *dialog = new ProfileManager4NewEdit();
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile4 profile = dialog->getNewProfile();
        profile.saveProfile();
        model->addItem(profile);
        emit updateProfiles();
    }
    delete dialog;
}

void ProfileManager4::on_pushButtonEdit_clicked()
{
    int row = ui->tableView->currentIndex().row();

    if (row < 0)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    auto *dialog = new ProfileManager4NewEdit(model->getProfile(row));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile4 profile = dialog->getNewProfile();
        profile.updateProfile(dialog->getOriginal());
        model->updateProfile(profile, row);
        emit updateProfiles();
    }
    delete dialog;
}

void ProfileManager4::on_pushButtonDelete_clicked()
{
    int row = ui->tableView->currentIndex().row();

    if (row < 0)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    QMessageBox message(QMessageBox::Question, tr("Delete profile"), tr("Are you sure you wish to delete this profile?"), QMessageBox::Yes | QMessageBox::No);
    if (message.exec() == QMessageBox::Yes)
    {
        Profile4 profile = model->getProfile(row);
        profile.deleteProfile();
        model->removeProfile(row);

        emit updateProfiles();
    }
}
