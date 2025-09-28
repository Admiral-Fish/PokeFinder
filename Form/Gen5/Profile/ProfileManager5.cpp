/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Parents/ProfileLoader.hpp>
#include <Form/Gen5/Profile/ProfileEditor5.hpp>
#include <Model/Gen5/ProfileModel5.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileManager5::ProfileManager5(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileManager5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new ProfileModel5(ui->tableView);
    model->addItems(ProfileLoader5::getProfiles());
    ui->tableView->setModel(model);

    connect(ui->pushButtonNew, &QPushButton::clicked, this, &ProfileManager5::create);
    connect(ui->pushButtonEdit, &QPushButton::clicked, this, &ProfileManager5::edit);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &ProfileManager5::remove);
    connect(ui->pushButtonOk, &QPushButton::clicked, this, &ProfileManager5::close);

    QSettings setting;
    if (setting.contains("profileManager5/geometry"))
    {
        this->restoreGeometry(setting.value("profileManager5/geometry").toByteArray());
    }
}

ProfileManager5::~ProfileManager5()
{
    QSettings setting;
    setting.setValue("profileManager5/geometry", this->saveGeometry());

    delete ui;
}

void ProfileManager5::create()
{
    std::unique_ptr<ProfileEditor5> dialog(new ProfileEditor5);
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile5 profile = dialog->getProfile();
        ProfileLoader5::addProfile(profile);
        model->addItem(profile);
        emit profilesModified(5);
    }
}

void ProfileManager5::edit()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profile selected"), tr("Please select a profile"));
        msg.exec();
        return;
    }

    const Profile5 &original = model->getItem(row);
    std::unique_ptr<ProfileEditor5> dialog(new ProfileEditor5(original));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile5 update = dialog->getProfile();
        ProfileLoader5::updateProfile(update, original);
        model->updateItem(update, row);
        emit profilesModified(5);
    }
}

void ProfileManager5::remove()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profile selected"), tr("Please select a profile"));
        msg.exec();
        return;
    }

    QMessageBox msg(QMessageBox::Question, tr("Delete profile"), tr("Are you sure you wish to delete this profile?"),
                    QMessageBox::Yes | QMessageBox::No);
    if (msg.exec() == QMessageBox::Yes)
    {
        const Profile5 &profile = model->getItem(row);
        ProfileLoader5::removeProfile(profile);
        model->removeItem(row);
        emit profilesModified(5);
    }
}
