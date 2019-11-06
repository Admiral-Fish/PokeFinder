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
#include <Forms/Gen3/ProfileEditor3.hpp>
#include <Models/Gen3/Profile3Model.hpp>
#include <QMessageBox>
#include <QSettings>

namespace PokeFinderForms
{
    ProfileManager3::ProfileManager3(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::ProfileManager3)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose);

        setupModels();
    }

    ProfileManager3::~ProfileManager3()
    {
        QSettings setting;
        setting.setValue("profileManager3/geometry", this->saveGeometry());

        delete ui;
    }

    void ProfileManager3::setupModels()
    {
        model = new PokeFinderModels::Profile3Model(ui->tableView);
        model->addItems(PokeFinderCore::Profile3::loadProfileList());
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

    void ProfileManager3::create()
    {
        QScopedPointer<ProfileEditor3> dialog(new ProfileEditor3);
        if (dialog->exec() == QDialog::Accepted)
        {
            PokeFinderCore::Profile3 profile = dialog->getNewProfile();
            profile.saveProfile();
            model->addItem(profile);
            emit updateProfiles();
        }
    }

    void ProfileManager3::edit()
    {
        int row = ui->tableView->currentIndex().row();
        if (row < 0)
        {
            QMessageBox error;
            error.setText(tr("Please select a profile."));
            error.exec();
            return;
        }

        QScopedPointer<ProfileEditor3> dialog(new ProfileEditor3(model->getItem(row)));
        if (dialog->exec() == QDialog::Accepted)
        {
            PokeFinderCore::Profile3 profile = dialog->getNewProfile();
            profile.updateProfile(dialog->getOriginal());
            model->updateItem(profile, row);
            emit updateProfiles();
        }
    }

    void ProfileManager3::remove()
    {
        int row = ui->tableView->currentIndex().row();
        if (row < 0)
        {
            QMessageBox error;
            error.setText(tr("Please select a profile."));
            error.exec();
            return;
        }

        QMessageBox message(QMessageBox::Question, tr("Delete profile"),
            tr("Are you sure you wish to delete this profile?"), QMessageBox::Yes | QMessageBox::No);
        if (message.exec() == QMessageBox::Yes)
        {

            PokeFinderCore::Profile3 profile = model->getItem(row);
            profile.deleteProfile();
            model->removeItem(row);

            emit updateProfiles();
        }
    }
}
