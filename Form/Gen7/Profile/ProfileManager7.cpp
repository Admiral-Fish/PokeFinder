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

#include "ProfileManager7.hpp"
#include "ui_ProfileManager7.h"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Form/Gen7/Profile/ProfileEditor7.hpp>
#include <Model/Gen7/ProfileModel7.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileManager7::ProfileManager7(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileManager7)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new ProfileModel7(ui->tableView);
    model->addItems(ProfileLoader7::getProfiles(Game::Gen7));
    ui->tableView->setModel(model);

    ui->tableView->setAcceptDrops(true);
    ui->tableView->setDefaultDropAction(Qt::MoveAction);
    ui->tableView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->tableView->setDragDropOverwriteMode(false);
    ui->tableView->setDragEnabled(true);
    ui->tableView->setDropIndicatorShown(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->pushButtonNew, &QPushButton::clicked, this, &ProfileManager7::create);
    connect(ui->pushButtonEdit, &QPushButton::clicked, this, &ProfileManager7::edit);
    connect(ui->pushButtonDuplicate, &QPushButton::clicked, this, &ProfileManager7::duplicate);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &ProfileManager7::remove);
    connect(ui->pushButtonOk, &QPushButton::clicked, this, &ProfileManager7::close);
    connect(model, &ProfileModel7::rowsMoved, this, [this] {
        ProfileLoader7::setProfiles(model->getModel());
        emit profilesChanged(7);
    });

    QSettings setting;
    if (setting.contains("profileManager7/geometry"))
    {
        this->restoreGeometry(setting.value("profileManager7/geometry").toByteArray());
    }
}

ProfileManager7::~ProfileManager7()
{
    QSettings setting;
    setting.setValue("profileManager7/geometry", this->saveGeometry());

    delete ui;
}

void ProfileManager7::create()
{
    std::unique_ptr<ProfileEditor7> dialog(new ProfileEditor7);
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile7 profile = dialog->getProfile();
        ProfileLoader7::addProfile(profile);
        model->addItem(profile);
        emit profilesChanged(7);
    }
}

void ProfileManager7::duplicate()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profile selected"), tr("Please select a profile"));
        msg.exec();
        return;
    }

    const Profile7 &profile = model->getItem(row);
    ProfileLoader7::addProfile(profile);
    model->addItem(profile);
    emit profilesChanged(7);
}

void ProfileManager7::edit()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("No profile selected"), tr("Please select a profile"));
        msg.exec();
        return;
    }

    Profile7 original = model->getItem(row);
    std::unique_ptr<ProfileEditor7> dialog(new ProfileEditor7(original));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile7 update = dialog->getProfile();
        ProfileLoader7::updateProfile(update, original);
        model->updateItem(update, row);
        emit profilesChanged(7);
    }
}

void ProfileManager7::remove()
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
        Profile7 profile = model->getItem(row);
        ProfileLoader7::removeProfile(profile);
        model->removeItem(row);
        emit profilesChanged(7);
    }
}
