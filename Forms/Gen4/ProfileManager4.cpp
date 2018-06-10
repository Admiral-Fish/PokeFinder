/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

ProfileManager4::ProfileManager4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileManager4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

ProfileManager4::~ProfileManager4()
{
    delete ui;
    delete model;
}

void ProfileManager4::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
            default:
                break;
        }
    }
}

void ProfileManager4::setupModels()
{
    model->setModel(Profile4::loadProfileList());
    ui->tableView->setModel(model);
}

void ProfileManager4::on_pushButtonNew_clicked()
{
    ProfileManager4NewEdit *dialog = new ProfileManager4NewEdit();
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile4 profile = dialog->getNewProfile();
        profile.saveProfile();
        model->addItem(profile);
        emit updateProfiles();
    }
    delete dialog;
}

void ProfileManager4::on_pushButtonOk_clicked()
{
    this->close();
}

void ProfileManager4::on_pushButtonEdit_clicked()
{
    int r = ui->tableView->currentIndex().row();

    if (r == -1)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    ProfileManager4NewEdit *dialog = new ProfileManager4NewEdit(model->getProfile(r));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile4 profile = dialog->getNewProfile();
        profile.updateProfile(dialog->getOriginal());
        int r = ui->tableView->currentIndex().row();
        model->updateProfile(profile, r);
        emit updateProfiles();
    }
    delete dialog;
}

void ProfileManager4::on_pushButtonDelete_clicked()
{
    int r = ui->tableView->currentIndex().row();

    if (r == -1)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    Profile4 profile = model->getProfile(r);
    profile.deleteProfile();

    model->removeProfile(r);

    emit updateProfiles();
}
