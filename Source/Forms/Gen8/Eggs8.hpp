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

#ifndef EGGS8_HPP
#define EGGS8_HPP

#include <Core/Gen8/Profile8.hpp>
#include <QMenu>

class EggModel8;

namespace Ui
{
    class Eggs8;
}

class Eggs8 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Eggs8(QWidget *parent = nullptr);
    ~Eggs8() override;
    void updateProfiles();

private:
    Ui::Eggs8 *ui;
    EggModel8 *model = nullptr;
    std::vector<Profile8> profiles;
    Profile8 currentProfile;
    QMenu *menu = nullptr;

    void setupModels();

private slots:
    void generate();
    void profileIndexChanged(int index);
    void tableViewContextMenu(QPoint pos);
    void profileManager();
};

#endif // EGGS8_HPP
