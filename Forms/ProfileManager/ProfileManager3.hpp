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

#ifndef PROFILEMANAGER3_HPP
#define PROFILEMANAGER3_HPP

#include <QMainWindow>
#include <Forms/ProfileManager/ProfileManager3NewEdit.hpp>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QObject>
#include <libPokeFinder/Gen3/Profile3.hpp>

using namespace std;
typedef uint32_t u32;

namespace Ui {
class ProfileManager3;
}

class ProfileManager3 : public QMainWindow
{
    Q_OBJECT

signals:
    void updateProfiles();

public:
    explicit ProfileManager3(QWidget *parent = 0);
    ~ProfileManager3();

private slots:
    void on_pushButtonNew_clicked();
    void on_pushButtonOk_clicked();
    void registerProfile(QString profileName, int version, int language, u32 tid, u32 sid, bool deadBattery);
    void on_pushButtonEdit_clicked();
    void on_pushButtonDelete_clicked();

private:
    Ui::ProfileManager3 *ui;

    void updateTable(vector<QList<QStandardItem *>> rows);

};

#endif // PROFILEMANAGER3_HPP
