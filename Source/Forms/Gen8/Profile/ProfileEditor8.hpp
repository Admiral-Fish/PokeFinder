/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef PROFILEEDITOR8_HPP
#define PROFILEEDITOR8_HPP

#include <Core/Parents/Profile.hpp>
#include <QDialog>

namespace Ui
{
    class ProfileEditor8;
}

class ProfileEditor8 : public QDialog
{
    Q_OBJECT
signals:
    void newProfile(Profile);
    void editProfile(Profile, Profile);

public:
    explicit ProfileEditor8(QWidget *parent = nullptr);
    explicit ProfileEditor8(const Profile &profile, QWidget *parent = nullptr);
    ~ProfileEditor8() override;
    Profile getNewProfile();
    Profile getOriginal();

private:
    Ui::ProfileEditor8 *ui;
    bool isEditing = false;
    Profile original;
    Profile fresh;

    void setupModels();

private slots:
    void okay();
};

#endif // PROFILEEDITOR8_HPP
