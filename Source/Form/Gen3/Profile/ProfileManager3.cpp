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

#include "ProfileManager3.hpp"
#include "ui_ProfileManager3.h"
#include <Core/Parents/ProfileLoader.hpp>
#include <Form/Gen3/Profile/ProfileEditor3.hpp>
#include <Model/Gen3/ProfileModel3.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileManager3::ProfileManager3(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileManager3), model(new ProfileModel3(this))
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model->addItems(ProfileLoader3::getProfiles());
    ui->tableView->setModel(model);

    connect(ui->pushButtonNew, &QPushButton::clicked, this, &ProfileManager3::create);
    connect(ui->pushButtonEdit, &QPushButton::clicked, this, &ProfileManager3::edit);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &ProfileManager3::remove);

    QSettings setting;
    if (setting.contains("profileManager3/geometry"))
    {
        this->restoreGeometry(setting.value("profileManager3/geometry").toByteArray());
    }
}

ProfileManager3::~ProfileManager3()
{
    QSettings setting;
    setting.setValue("profileManager3/geometry", this->saveGeometry());

    delete ui;
}

void ProfileManager3::create()
{
    std::unique_ptr<ProfileEditor3> dialog(new ProfileEditor3);
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile3 profile = dialog->getProfile();
        ProfileLoader3::addProfile(profile);
        model->addItem(profile);
        emit updateProfiles(3);
    }
}

void ProfileManager3::edit()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profile selected"), tr("Please select a profile"));
        msg.exec();
        return;
    }

    Profile3 original = model->getItem(row);
    std::unique_ptr<ProfileEditor3> dialog(new ProfileEditor3(original));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile3 update = dialog->getProfile();
        ProfileLoader3::updateProfile(update, original);
        model->updateItem(update, row);
        emit updateProfiles(3);
    }
}

void ProfileManager3::remove()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profile selected"), tr("Please select a profile"));
        msg.exec();
        return;
    }

    QMessageBox message(QMessageBox::Question, tr("Delete profile"), tr("Are you sure you wish to delete this profile?"),
                        QMessageBox::Yes | QMessageBox::No);
    if (message.exec() == QMessageBox::Yes)
    {
        Profile3 profile = model->getItem(row);
        ProfileLoader3::removeProfile(profile);
        model->removeItem(row);
        emit updateProfiles(3);
    }
}
