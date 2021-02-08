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

#ifndef IDS3_HPP
#define IDS3_HPP

#include <QWidget>

class IDModel3;

namespace Ui
{
    class IDs3;
}

class IDs3 : public QWidget
{
    Q_OBJECT
public:
    explicit IDs3(QWidget *parent = nullptr);
    ~IDs3() override;

private:
    Ui::IDs3 *ui;
    IDModel3 *xdcolo = nullptr;
    IDModel3 *frlge = nullptr;
    IDModel3 *rs = nullptr;

    void setupModels();

private slots:
    void xdColoSearch();
    void frlgeSearch();
    void rsSearch();
    void rsDeadBattery(bool checked);
    void rsDate(bool checked);
    void rsInitialSeed(bool checked);
};

#endif // IDS3_HPP
