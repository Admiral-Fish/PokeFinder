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

#include "ProfileManager4.hpp"
#include "ui_ProfileManager4.h"
#include <Core/Gen4/ProfileLoader4.hpp>
#include <Forms/Gen4/Profile/ProfileEditor4.hpp>
#include <Models/Gen4/ProfileModel4.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileManager4::ProfileManager4(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileManager4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

ProfileManager4::~ProfileManager4()
{
    QSettings setting;
    setting.setValue("profileManager4/geometry", this->saveGeometry());

    delete ui;
}

void ProfileManager4::setupModels()
{
    model = new ProfileModel4(ui->tableView);
    model->addItems(ProfileLoader4::getProfiles());
    ui->tableView->setModel(model);

    connect(ui->pushButtonNew, &QPushButton::clicked, this, &ProfileManager4::create);
    connect(ui->pushButtonEdit, &QPushButton::clicked, this, &ProfileManager4::edit);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &ProfileManager4::remove);

    QSettings setting;
    if (setting.contains("profileManager4/geometry"))
    {
        this->restoreGeometry(setting.value("profileManager4/geometry").toByteArray());
    }
}

void ProfileManager4::create()
{
    QScopedPointer<ProfileEditor4> dialog(new ProfileEditor4);
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile4 profile = dialog->getNewProfile();
        ProfileLoader4::addProfile(profile);
        model->addItem(profile);
        emit updateProfiles();
    }
}

void ProfileManager4::edit()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    QScopedPointer<ProfileEditor4> dialog(new ProfileEditor4(model->getItem(row)));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile4 profile = dialog->getNewProfile();
        ProfileLoader4::updateProfile(profile, dialog->getOriginal());
        model->updateItem(profile, row);
        emit updateProfiles();
    }
}

void ProfileManager4::remove()
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
        Profile4 profile = model->getItem(row);
        ProfileLoader4::removeProfile(profile);
        model->removeItem(row);

        emit updateProfiles();
    }
}
