/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef PROFILEEDITOR4_HPP
#define PROFILEEDITOR4_HPP

#include <Core/Gen4/Profile4.hpp>
#include <QDialog>

namespace Ui
{
    class ProfileEditor4;
}

class ProfileEditor4 : public QDialog
{
    Q_OBJECT
signals:
    void newProfile(Profile4);
    void editProfile(Profile4, Profile4);

public:
    explicit ProfileEditor4(QWidget *parent = nullptr);
    explicit ProfileEditor4(const Profile4 &profile, QWidget *parent = nullptr);
    ~ProfileEditor4() override;
    Profile4 getNewProfile();
    Profile4 getOriginal();

private:
    Ui::ProfileEditor4 *ui;
    bool isEditing = false;
    Profile4 original;
    Profile4 fresh;

    void setupModels();

private slots:
    void okay();
    void versionIndexChanged(int index);
};

#endif // PROFILEEDITOR4_HPP
