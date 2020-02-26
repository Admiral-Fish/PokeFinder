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

#ifndef SEEDTOTIME4_HPP
#define SEEDTOTIME4_HPP

#include <Core/Gen4/Profile4.hpp>
#include <QWidget>

class DateTime;
class SeedtoTimeModel4;

namespace Ui
{
    class SeedtoTime4;
}

class SeedtoTime4 : public QWidget
{
    Q_OBJECT
public:
    explicit SeedtoTime4(QWidget *parent = nullptr);
    explicit SeedtoTime4(const QString &seed, const Profile4 &profile, QWidget *parent = nullptr);
    ~SeedtoTime4() override;

private:
    Ui::SeedtoTime4 *ui;
    SeedtoTimeModel4 *dpptModel = nullptr;
    SeedtoTimeModel4 *dpptCalibrateModel = nullptr;
    SeedtoTimeModel4 *hgssModel = nullptr;
    SeedtoTimeModel4 *hgssCalibrateModel = nullptr;

    void setupModels();
    QVector<DateTime> generate(u32 seed, u32 year, bool forceSecond, int forcedSecond, Game version);
    QVector<DateTime> calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond, const DateTime &target);

private slots:
    void dpptGenerate();
    void dpptCalibrate();
    void hgssGenerate();
    void hgssCalibrate();
    void searchFlips();
    void searchCalls();
    void map();
};

#endif // SEEDTOTIME4_HPP
