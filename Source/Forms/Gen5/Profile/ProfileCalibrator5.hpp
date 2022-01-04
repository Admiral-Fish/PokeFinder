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

#ifndef PROFILECALIBRATOR5_HPP
#define PROFILECALIBRATOR5_HPP

#include <QAbstractButton>
#include <QMenu>

class ProfileSearcherModel5;

namespace Ui
{
    class ProfileCalibrator5;
}

class ProfileCalibrator5 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit ProfileCalibrator5(QWidget *parent = nullptr);
    ~ProfileCalibrator5();

private:
    Ui::ProfileCalibrator5 *ui;
    ProfileSearcherModel5 *model = nullptr;
    QMenu *menu = nullptr;

    void setupModels();
    void updateParameters();

private slots:
    void openIVCalculator();
    void search();
    void createProfile();
    void tableViewContextMenu(QPoint pos);
    void versionIndexChanged(int index);
    void dsTypeIndexChanged(int index);
    void addNeedle(QAbstractButton *button);
    void removeNeedle();
    void clearNeedles();
};

#endif // PROFILECALIBRATOR5_HPP
