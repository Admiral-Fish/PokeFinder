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

#include "ProfileManager5.hpp"
#include "ui_ProfileManager5.h"

ProfileManager5::ProfileManager5(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileManager5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

ProfileManager5::~ProfileManager5()
{
    delete ui;
}

void ProfileManager5::setupModels()
{
    model = new Profile5Model(ui->tableView);
    model->addItems(Profile5::loadProfileList());
    ui->tableView->setModel(model);
}

void ProfileManager5::on_pushButtonNew_clicked()
{
    QScopedPointer<ProfileEditor5> dialog(new ProfileEditor5);
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile5 profile = dialog->getNewProfile();
        profile.saveProfile();
        model->addItem(profile);
        emit updateProfiles();
    }
}

void ProfileManager5::on_pushButtonEdit_clicked()
{
    int row = ui->tableView->currentIndex().row();

    if (row < 0)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    QScopedPointer<ProfileEditor5> dialog(new ProfileEditor5(model->getItem(row)));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile5 profile = dialog->getNewProfile();
        profile.updateProfile(dialog->getOriginal());
        model->updateItem(profile, row);
        emit updateProfiles();
    }
}

void ProfileManager5::on_pushButtonDelete_clicked()
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
        Profile5 profile = model->getItem(row);
        profile.deleteProfile();
        model->removeItem(row);

        emit updateProfiles();
    }
}
