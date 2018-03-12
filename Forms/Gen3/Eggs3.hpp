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

#ifndef EGGS3_HPP
#define EGGS3_HPP

#include <QMainWindow>
#include <libPokeFinder/Gen3/Frame3.hpp>
#include <Forms/ProfileManager/ProfileManager3.hpp>
#include <Models/Gen3/Egg3Model.hpp>
#include <libPokeFinder/Objects/FrameCompare.hpp>
#include <libPokeFinder/Gen3/Egg3.hpp>

namespace Ui {
class Eggs3;
}

class Eggs3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

signals:
    void alertProfiles(int);

public:
    explicit Eggs3(QWidget *parent = 0);
    ~Eggs3();
    void updateProfiles();

private slots:
    void on_pushButtonProfileManagerEmerald_clicked();
    void refreshProfiles();
    void on_pushButtonGenerateEmeraldPID_clicked();
    void on_pushButtonGenerateEmeraldIVs_clicked();
    void on_pushButtonProfileManagerRS_clicked();
    void on_pushButtonGenerateRS_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_pushButtonGenerateFRLG_clicked();
    void on_pushButtonProfileManagerFRLG_clicked();

private:
    Ui::Eggs3 *ui;
    vector<Profile3> profiles;
    Egg3Model *emeraldIVs = new Egg3Model(this, EBred);
    Egg3Model *emeraldPID = new Egg3Model(this, EBredPID);
    Egg3Model *rs = new Egg3Model(this, RSBred);
    Egg3Model *frlg = new Egg3Model(this, FRLGBred);

    void setupModels();
};

#endif // EGGS3_HPP
