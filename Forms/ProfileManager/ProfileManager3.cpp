/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

    model = new Profile3Model(this);
    model->setModel(Profile3::loadProfileList());
    ui->tableView->setModel(model);
}

ProfileManager3::~ProfileManager3()
{
    delete ui;
}

void ProfileManager3::on_pushButtonNew_clicked()
{
    ProfileManager3NewEdit* dialog = new ProfileManager3NewEdit();
    connect(dialog, SIGNAL (newProfile(Profile3)), this, SLOT (registerProfile(Profile3)));
    dialog->exec();
}

void ProfileManager3::registerProfile(Profile3 profile)
{
    profile.saveProfile();
    model->addItem(profile);
    ui->tableView->viewport()->update();
    emit updateProfiles();
}

void ProfileManager3::editProfile(Profile3 profile, Profile3 original)
{
    profile.updateProfile(original);
    int r = ui->tableView->selectionModel()->selectedRows().at(0).row();
    model->updateProfile(profile, r);
    ui->tableView->viewport()->update();
    emit updateProfiles();
}

void ProfileManager3::on_pushButtonOk_clicked()
{
    this->close();
}

void ProfileManager3::on_pushButtonEdit_clicked()
{
    QItemSelectionModel *selections = ui->tableView->selectionModel();
    if(selections->selectedRows().count() != 1)
    {
        QMessageBox error;
        error.setText("Please select one row.");
        error.exec();
        return;
    }

    int r = selections->selectedRows().at(0).row();
    ProfileManager3NewEdit* dialog = new ProfileManager3NewEdit(model->getProfile(r));
    connect(dialog, SIGNAL (editProfile(Profile3, Profile3)), this, SLOT (editProfile(Profile3, Profile3)));
    dialog->exec();
}

void ProfileManager3::on_pushButtonDelete_clicked()
{
    QMessageBox error;
    QItemSelectionModel *selections = ui->tableView->selectionModel();
    if(selections->selectedRows().count() != 1)
    {
        error.setText(tr("Please select one row."));
        error.exec();
        return;
    }

    int r = selections->selectedRows().at(0).row();
    Profile3 profile = model->getProfile(r);
    profile.deleteProfile();

    model->removeProfile(r);
}


