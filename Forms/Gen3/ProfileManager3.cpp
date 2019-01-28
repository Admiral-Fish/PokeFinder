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

#include "ProfileManager3.hpp"
#include "ui_ProfileManager3.h"

ProfileManager3::ProfileManager3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileManager3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

ProfileManager3::~ProfileManager3()
{
    delete ui;
}

void ProfileManager3::setupModels()
{
    model = new Profile3Model(ui->tableView);
    model->setModel(Profile3::loadProfileList());
    ui->tableView->setModel(model);
}

void ProfileManager3::on_pushButtonNew_clicked()
{
    auto *dialog = new ProfileManager3NewEdit();
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile3 profile = dialog->getNewProfile();
        profile.saveProfile();
        model->addItem(profile);
        emit updateProfiles();
    }
    delete dialog;
}

void ProfileManager3::on_pushButtonEdit_clicked()
{
    int row = ui->tableView->currentIndex().row();

    if (row < 0)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    auto *dialog = new ProfileManager3NewEdit(model->getProfile(row));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile3 profile = dialog->getNewProfile();
        profile.updateProfile(dialog->getOriginal());
        model->updateProfile(profile, row);
        emit updateProfiles();
    }
    delete dialog;
}

void ProfileManager3::on_pushButtonDelete_clicked()
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

        Profile3 profile = model->getProfile(row);
        profile.deleteProfile();
        model->removeProfile(row);

        emit updateProfiles();
    }
}
