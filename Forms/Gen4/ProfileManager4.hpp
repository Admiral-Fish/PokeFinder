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

#ifndef PROFILEMANAGER4_HPP
#define PROFILEMANAGER4_HPP

#include <QMainWindow>
#include <Forms/Gen4/ProfileManager4NewEdit.hpp>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QObject>
#include <PokeFinderCore/Gen4/Profile4.hpp>
#include <Models/Gen4/Profile4Model.hpp>

using std::vector;
typedef uint32_t u32;

namespace Ui
{
    class ProfileManager4;
}

class ProfileManager4 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *);

signals:
    void updateProfiles();

private:
    Ui::ProfileManager4 *ui;
    Profile4Model *model = new Profile4Model(this);

    void setupModels();

private slots:
    void on_pushButtonNew_clicked();
    void on_pushButtonOk_clicked();
    void registerProfile(Profile4 profile);
    void editProfile(Profile4 profile, Profile4 original);
    void on_pushButtonEdit_clicked();
    void on_pushButtonDelete_clicked();

public:
    explicit ProfileManager4(QWidget *parent = 0);
    ~ProfileManager4();

};

#endif // PROFILEMANAGER4_HPP
