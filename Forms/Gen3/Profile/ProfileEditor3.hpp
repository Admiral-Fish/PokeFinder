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

#ifndef PROFILEEDITOR3_HPP
#define PROFILEEDITOR3_HPP

#include <Core/Gen3/Profile3.hpp>
#include <QDialog>

namespace Ui
{
    class ProfileEditor3;
}

class ProfileEditor3 : public QDialog
{
    Q_OBJECT
signals:
    void newProfile(Profile3);
    void editProfile(Profile3, Profile3);

public:
    explicit ProfileEditor3(QWidget *parent = nullptr);
    explicit ProfileEditor3(const Profile3 &profile, QWidget *parent = nullptr);
    ~ProfileEditor3() override;
    Profile3 getNewProfile();
    Profile3 getOriginal();

private:
    Ui::ProfileEditor3 *ui;
    bool isEditing = false;
    Profile3 original;
    Profile3 fresh;

    void setupModels();

private slots:
    void okay();
    void versionIndexChanged(int index);
};

#endif // PROFILEEDITOR3_HPP
