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

#ifndef PROFILECALIBRATOR5_HPP
#define PROFILECALIBRATOR5_HPP

#include <QMenu>
#include <QStandardItemModel>

namespace Ui
{
    class ProfileCalibrator5;
}

class ProfileCalibrator5 : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileCalibrator5(QWidget *parent = nullptr);
    ~ProfileCalibrator5();

private:
    Ui::ProfileCalibrator5 *ui;
    QStandardItemModel *model = nullptr;
    QMenu *menu = nullptr;

    void setupModels();
    void updateParameters();

private slots:
    void openIVCalculator();
    void clearTable();
    void search();
    void createProfile();
    void tableViewContextMenu(QPoint pos);
    void versionIndexChanged(int index);
    void dsTypeIndexChanged(int index);
};

#endif // PROFILECALIBRATOR5_HPP
