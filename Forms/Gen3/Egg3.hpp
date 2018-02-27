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

#ifndef EGG3_HPP
#define EGG3_HPP

#include <QMainWindow>
#include <libPokeFinder/Gen3/Frame3.hpp>
#include <Forms/ProfileManager/ProfileManager3.hpp>

namespace Ui {
class Egg3;
}

class Egg3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

signals:
    void alertProfiles(int);

public:
    explicit Egg3(QWidget *parent = 0);
    ~Egg3();
    void updateProfiles();

private slots:
    void on_pushButtonProfileManagerEmerald_clicked();
    void refreshProfiles();

private:
    Ui::Egg3 *ui;
    vector<Profile3> profiles;

    void setupModels();
};

#endif // EGG3_HPP
