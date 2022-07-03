/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <QDialog>

namespace Ui
{
    class ProfileEditor3;
}

class Profile3;

class ProfileEditor3 : public QDialog
{
    Q_OBJECT
public:
    explicit ProfileEditor3(QWidget *parent = nullptr);
    explicit ProfileEditor3(const Profile3 &profile, QWidget *parent = nullptr);
    ~ProfileEditor3() override;
    Profile3 getProfile();

private:
    Ui::ProfileEditor3 *ui;

    void setupModels();

private slots:
    void okay();
    void versionIndexChanged(int index);
};

#endif // PROFILEEDITOR3_HPP
