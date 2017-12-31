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

#include "ProfileManagerGen3.hpp"
#include "ui_ProfileManagerGen3.h"

ProfileManagerGen3::ProfileManagerGen3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileManagerGen3)
{
    ui->setupUi(this);
    updateTable(ProfileGen3::loadProfiles());
}

ProfileManagerGen3::~ProfileManagerGen3()
{
    delete ui;
}

void ProfileManagerGen3::on_pushButtonNew_clicked()
{
    ProfileManagerGen3NewEdit* dialog = new ProfileManagerGen3NewEdit();
    QObject::connect(dialog, SIGNAL(newProfile(QString, int, int, u32, u32, bool)), this, SLOT(registerProfile(QString, int, int, u32, u32,bool)));
    dialog->exec();
}

void ProfileManagerGen3::updateTable(vector<QList<QStandardItem *>> rows)
{
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Profile Name"), tr("Version"), tr("Language"), tr("TID"), tr("SID"), tr("Dead Battery")});

    for(int i = 0; i < (int)rows.size(); i++)
    {
        model->appendRow(rows.at(i));
    }

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ProfileManagerGen3::registerProfile(QString profileName, int version, int language, u32 tid, u32 sid, bool deadBattery)
{
    bool exists = false;

    ProfileGen3 profile(profileName, version, tid, sid, language, deadBattery, true);
    profile.saveProfile();

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Profile Name"), tr("Version"), tr("Language"), tr("TID"), tr("SID"), tr("Dead Battery")});

    QAbstractItemModel* original = ui->tableView->model();

    for(int i = 0; i < original->rowCount(); i++)
    {
        QList<QStandardItem *> item;
        if(original->data(original->index(i, 0)).toString() == profileName)
        {
            exists = true;
            original->setData(original->index(i, 0), profileName);
            original->setData(original->index(i, 1), ProfileGen3::getVersion(version));
            original->setData(original->index(i, 2), ProfileGen3::getLanguage(language));
            original->setData(original->index(i, 3), QString::number(tid));
            original->setData(original->index(i, 4), QString::number(sid));
            if(version == 0 || version == 1)
            {
                original->setData(original->index(i, 5), (deadBattery ? "Yes" : "No"));
            }
            else
            {
                original->setData(original->index(i, 5), "N/A");
            }
        }
        for(int j = 0; j < original->columnCount(); j++)
        {
            QStandardItem *it = new QStandardItem(original->data(original->index(i, j)).toString());
            it->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
            item.append(it);
        }
        model->appendRow(item);
    }
    if(!exists)
    {
        QList<QStandardItem *> newItem;
        newItem.append(new QStandardItem(profileName));
        newItem.append(new QStandardItem(ProfileGen3::getVersion(version)));
        newItem.append(new QStandardItem(ProfileGen3::getLanguage(language)));
        newItem.append(new QStandardItem(QString::number(tid)));
        newItem.append(new QStandardItem(QString::number(sid)));
        if(version == 0 || version == 1)
        {
            newItem.append(new QStandardItem(deadBattery ? "Yes" : "No"));
        }
        else
        {
            newItem.append(new QStandardItem("N/A"));
        }

        for(int i = 0; i < newItem.count(); i++)
        {
            newItem.at(i)->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        }

        model->appendRow(newItem);
    }

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ProfileManagerGen3::on_pushButtonOk_clicked()
{
    emit updateProfiles();
    this->close();
}

void ProfileManagerGen3::on_pushButtonEdit_clicked()
{
    QMessageBox error;
    QItemSelectionModel *selections = ui->tableView->selectionModel();
    if(selections->selectedRows().count() != 1)
    {
        error.setText("Please select one row.");
        error.exec();
        return;
    }

    QAbstractItemModel* model = ui->tableView->model();

    int r = selections->selectedRows().at(0).row();
    ProfileManagerGen3NewEdit* dialog = new ProfileManagerGen3NewEdit(model->data(model->index(r, 0)).toString(), ProfileGen3::getVersionIndex(model->data(model->index(r, 1)).toString()), ProfileGen3::getLanguageIndex(model->data(model->index(r, 2)).toString()), model->data(model->index(r, 3)).toString().toUInt(), model->data(model->index(r, 4)).toString().toUInt(), model->data(model->index(r, 5)).toBool());
    QObject::connect(dialog, SIGNAL(newProfile(QString, int, int, u32, u32, bool)), this, SLOT(registerProfile(QString, int, int, u32, u32,bool)));
    dialog->exec();

}

void ProfileManagerGen3::on_pushButtonDelete_clicked()
{
    QMessageBox error;
    QItemSelectionModel *selections = ui->tableView->selectionModel();
    if(selections->selectedRows().count() != 1)
    {
        error.setText(tr("Please select one row."));
        error.exec();
        return;
    }

    QAbstractItemModel* model = ui->tableView->model();

    int r = selections->selectedRows().at(0).row();
    ProfileGen3 profile(model->data(model->index(r, 0)).toString(), ProfileGen3::getVersionIndex(model->data(model->index(r, 1)).toString()), ProfileGen3::getLanguageIndex(model->data(model->index(r, 2)).toString()), model->data(model->index(r, 3)).toString().toUInt(), model->data(model->index(r, 4)).toString().toUInt(), model->data(model->index(r, 5)).toBool());
    profile.deleteProfile();

    model->removeRow(r);

}
