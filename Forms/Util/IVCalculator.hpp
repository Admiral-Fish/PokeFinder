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

#ifndef IVCALCULATOR_HPP
#define IVCALCULATOR_HPP

#include <Core/Util/Global.hpp>
#include <QLabel>

class PersonalInfo;

namespace Ui
{
    class IVCalculator;
}

class IVCalculator : public QWidget
{
    Q_OBJECT
public:
    explicit IVCalculator(QWidget *parent = nullptr);
    ~IVCalculator() override;

private:
    Ui::IVCalculator *ui;
    QVector<PersonalInfo> personalInfo;

    void setupModels();
    void displayIVs(QLabel *label, const QVector<u8> &ivs);
    PersonalInfo getPersonalInfo(const PersonalInfo &base);

private slots:
    void findIVs();
    void pokemonIndexChanged(int index);
    void altformIndexChanged(int index);
    void generationIndexChanged(int index);
};

#endif // IVCALCULATOR_HPP
