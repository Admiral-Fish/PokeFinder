/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef POKESPOT_HPP
#define POKESPOT_HPP

#include <QMainWindow>
#include <QSettings>
#include <QVector>
#include <PokeFinderCore/RNG/LCRNG.hpp>
#include <Models/Gen3/PokeSpotModel.hpp>
#include <PokeFinderCore/Objects/Nature.hpp>
#include <PokeFinderCore/Gen3/Frame3.hpp>
#include <PokeFinderCore/Objects/FrameCompare.hpp>

namespace Ui
{
    class PokeSpot;
}

class PokeSpot : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *);

private:
    Ui::PokeSpot *ui;
    PokeSpotModel *model = new PokeSpotModel(this);

    void setupModels();
    void saveSettings();
    void loadSettings();

private slots:
    void on_pushButtonAnyAbility_clicked();
    void on_pushButtonAnyNature_clicked();
    void on_pushButtonAnySpotType_clicked();
    void on_pushButtonGenerate_clicked();

public:
    explicit PokeSpot(QWidget *parent = nullptr);
    ~PokeSpot();

};

#endif // POKESPOT_HPP
