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

#include "ProfileManager8.hpp"
#include "ui_ProfileManager8.h"
#include <Core/Parents/ProfileLoader.hpp>
#include <Form/Gen8/Profile/ProfileEditor8.hpp>
#include <Model/Gen8/ProfileModel8.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileManager8::ProfileManager8(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileManager8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new ProfileModel8(ui->tableView);
    model->addItems(ProfileLoader8::getProfiles());
    ui->tableView->setModel(model);

    connect(ui->pushButtonNew, &QPushButton::clicked, this, &ProfileManager8::create);
    connect(ui->pushButtonEdit, &QPushButton::clicked, this, &ProfileManager8::edit);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &ProfileManager8::remove);

    QSettings setting;
    if (setting.contains("profileManager8/geometry"))
    {
        this->restoreGeometry(setting.value("profileManager8/geometry").toByteArray());
    }
}

ProfileManager8::~ProfileManager8()
{
    QSettings setting;
    setting.setValue("profileManager8/geometry", this->saveGeometry());

    delete ui;
}

void ProfileManager8::create()
{
    std::unique_ptr<ProfileEditor8> dialog(new ProfileEditor8);
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile8 profile = dialog->getProfile();
        ProfileLoader8::addProfile(profile);
        model->addItem(profile);
        emit profilesModified(8);
    }
}

void ProfileManager8::edit()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profile selected"), tr("Please select a profile"));
        msg.exec();
        return;
    }

    Profile8 original = model->getItem(row);
    std::unique_ptr<ProfileEditor8> dialog(new ProfileEditor8(original));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile8 update = dialog->getProfile();
        ProfileLoader8::updateProfile(update, original);
        model->updateItem(update, row);
        emit profilesModified(8);
    }
}

void ProfileManager8::remove()
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
        Profile8 profile = model->getItem(row);
        ProfileLoader8::removeProfile(profile);
        model->removeItem(row);
        emit profilesModified(8);
    }
}
