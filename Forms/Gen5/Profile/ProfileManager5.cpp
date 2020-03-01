/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen5/ProfileLoader5.hpp>
#include <Forms/Gen5/Profile/ProfileEditor5.hpp>
#include <Models/Gen5/ProfileModel5.hpp>

ProfileManager5::ProfileManager5(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileManager5)
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
    model = new ProfileModel5(ui->tableView);
    model->addItems(ProfileLoader5::getProfiles());
    ui->tableView->setModel(model);

    connect(ui->pushButtonNew, &QPushButton::clicked, this, &ProfileManager5::create);
    connect(ui->pushButtonEdit, &QPushButton::clicked, this, &ProfileManager5::edit);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &ProfileManager5::remove);
}

void ProfileManager5::create()
{
    QScopedPointer<ProfileEditor5> dialog(new ProfileEditor5);
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile5 profile = dialog->getNewProfile();
        ProfileLoader5::addProfile(profile);
        model->addItem(profile);
        emit updateProfiles();
    }
}

void ProfileManager5::edit()
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
        ProfileLoader5::updateProfile(profile, dialog->getOriginal());
        model->updateItem(profile, row);
        emit updateProfiles();
    }
}

void ProfileManager5::remove()
{
    int row = ui->tableView->currentIndex().row();

    if (row < 0)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    QMessageBox message(QMessageBox::Question, tr("Delete profile"), tr("Are you sure you wish to delete this profile?"),
                        QMessageBox::Yes | QMessageBox::No);
    if (message.exec() == QMessageBox::Yes)
    {
        Profile5 profile = model->getItem(row);
        ProfileLoader5::removeProfile(profile);
        model->removeItem(row);

        emit updateProfiles();
    }
}
