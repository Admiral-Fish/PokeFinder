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

#ifndef PROFILEMANAGER4_HPP
#define PROFILEMANAGER4_HPP

#include <QWidget>

class ProfileModel4;

namespace Ui
{
    class ProfileManager4;
}

class ProfileManager4 : public QWidget
{
    Q_OBJECT
signals:
    void updateProfiles();

public:
    explicit ProfileManager4(QWidget *parent = nullptr);
    ~ProfileManager4() override;

private:
    Ui::ProfileManager4 *ui;
    ProfileModel4 *model = nullptr;

    void setupModels();

private slots:
    void create();
    void edit();
    void remove();
};

#endif // PROFILEMANAGER4_HPP
