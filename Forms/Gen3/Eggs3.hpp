/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef EGGS3_HPP
#define EGGS3_HPP

#include <QMainWindow>
#include <QSettings>
#include <Core/Gen3/Egg3.hpp>
#include <Core/Gen3/Frame3.hpp>
#include <Core/Objects/FrameCompare.hpp>
#include <Forms/Gen3/ProfileManager3.hpp>
#include <Models/Gen3/Egg3Model.hpp>

namespace Ui
{
    class Eggs3;
}

class Eggs3 : public QMainWindow
{
    Q_OBJECT

signals:
    void alertProfiles(int);

public:
    explicit Eggs3(QWidget *parent = nullptr);
    ~Eggs3() override;
    void updateProfiles();

private:
    Ui::Eggs3 *ui;
    QVector<Profile3> profiles;
    Egg3Model *emeraldIVs = new Egg3Model(this, Method::EBred);
    Egg3Model *emeraldPID = new Egg3Model(this, Method::EBredPID);
    Egg3Model *rs = new Egg3Model(this, Method::RSBred);
    Egg3Model *frlg = new Egg3Model(this, Method::FRLGBred);

    void setupModels();

private slots:
    void refreshProfiles();
    void on_pushButtonEmeraldPIDGenerate_clicked();
    void on_pushButtonEmeraldIVsGenerate_clicked();
    void on_pushButtonRSGenerate_clicked();
    void on_pushButtonFRLGGenerate_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_pushButtonEmeraldAnyAbility_clicked();
    void on_pushButtonRSAnyAbility_clicked();
    void on_pushButtonFRLGAnyAbility_clicked();
    void on_pushButtonProfileManager_clicked();

};

#endif // EGGS3_HPP
