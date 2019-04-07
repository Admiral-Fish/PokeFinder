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

#ifndef SEEDTOTIME4_HPP
#define SEEDTOTIME4_HPP

#include <QMessageBox>
#include <QSettings>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Util/Utilities.hpp>
#include <Forms/Gen4/RoamerMap.hpp>
#include <Forms/Gen4/SearchCalls.hpp>
#include <Forms/Gen4/SearchCoinFlips.hpp>
#include <Models/Gen4/SeedtoTime4Model.hpp>

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
    SeedtoTime4Model *dppt{};
    SeedtoTime4Model *dpptCalibrate{};
    SeedtoTime4Model *hgss{};
    SeedtoTime4Model *hgssCalibrate{};

    void setupModels();
    QVector<DateTime> generate(u32 seed, u32 year, bool forceSecond, int forcedSecond, Game version);
    QVector<DateTime> calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond, const DateTime &target);

private slots:
    void on_pushButtonDPPtGenerate_clicked();
    void on_pushButtonHGSSGenerate_clicked();
    void on_pushButtonHGSSCalibrate_clicked();
    void on_pushButtonDPPtCalibrate_clicked();
    void on_pushButtonDPPtSearchFlips_clicked();
    void on_pushButtonHGSSSearchCalls_clicked();
    void on_pushButtonHGSSMap_clicked();

};

#endif // SEEDTOTIME4_HPP
