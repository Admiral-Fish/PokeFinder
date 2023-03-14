/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Parents/ProfileLoader.hpp>
#include <Form/Gen4/Profile/ProfileEditor4.hpp>
#include <Model/Gen4/ProfileModel4.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileManager4::ProfileManager4(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileManager4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

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

ProfileManager4::~ProfileManager4()
{
    QSettings setting;
    setting.setValue("profileManager4/geometry", this->saveGeometry());

    delete ui;
}

void ProfileManager4::create()
{
    std::unique_ptr<ProfileEditor4> dialog(new ProfileEditor4);
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile4 profile = dialog->getProfile();
        ProfileLoader4::addProfile(profile);
        model->addItem(profile);
        emit profilesModified(4);
    }
}

void ProfileManager4::edit()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profile selected"), tr("Please select a profile"));
        msg.exec();
        return;
    }

    const Profile4 &original = model->getItem(row);
    std::unique_ptr<ProfileEditor4> dialog(new ProfileEditor4(original));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile4 update = dialog->getProfile();
        ProfileLoader4::updateProfile(update, original);
        model->updateItem(update, row);
        emit profilesModified(4);
    }
}

void ProfileManager4::remove()
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
        const Profile4 &profile = model->getItem(row);
        ProfileLoader4::removeProfile(profile);
        model->removeItem(row);
        emit profilesModified(4);
    }
}
