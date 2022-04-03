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

#ifndef SEEDTOTIME4_HPP
#define SEEDTOTIME4_HPP

#include <QWidget>
#include <Core/Util/Global.hpp>

class SeedTime;
class SeedtoTimeModel4;
enum class Game : u32;

namespace Ui
{
    class SeedtoTime4;
}

class SeedtoTime4 : public QWidget
{
    Q_OBJECT
public:
    explicit SeedtoTime4(QWidget *parent = nullptr);
    explicit SeedtoTime4(const QString &seed, Game version, QWidget *parent = nullptr);
    ~SeedtoTime4() override;

private:
    Ui::SeedtoTime4 *ui;
    SeedtoTimeModel4 *dpptModel;
    SeedtoTimeModel4 *dpptCalibrateModel;
    SeedtoTimeModel4 *hgssModel;
    SeedtoTimeModel4 *hgssCalibrateModel;

    void setupModels();
    std::vector<SeedTime> generate(u32 seed, u32 year, bool forceSecond, int forcedSecond, Game version);
    std::vector<SeedTime> calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond, const SeedTime &target);

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
