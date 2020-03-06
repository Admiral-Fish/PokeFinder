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

#ifndef EGGS3_HPP
#define EGGS3_HPP

#include <Core/Gen3/Profile3.hpp>
#include <QWidget>

class EggModel3;

namespace Ui
{
    class Eggs3;
}

class Eggs3 : public QWidget
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
    Profile3 currentProfile;
    EggModel3 *emerald = nullptr;
    EggModel3 *rs = nullptr;
    EggModel3 *frlg = nullptr;

    void setupModels();

private slots:
    void emeraldGenerate();
    void rsGenerate();
    void frlgGenerate();
    void profilesIndexChanged(int index);
    void profileManager();
};

#endif // EGGS3_HPP
