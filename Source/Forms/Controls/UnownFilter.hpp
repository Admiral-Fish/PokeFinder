/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef UNOWNFILTER_HPP
#define UNOWNFILTER_HPP

#include <Core/Util/Global.hpp>
#include "Filter.hpp"
#include <QWidget>

namespace Ui
{
    class UnownFilter;
}

class UnownFilter  : public QWidget
{
    Q_OBJECT
public:
    explicit UnownFilter(QWidget *parent = nullptr);
    ~UnownFilter() override ;

    std::array<u8, 6> getMinIVs() const;
    std::array<u8, 6> getMaxIVs() const;
    u8 getAbility() const;
    u8 getGender() const;
    u8 getGenderRatio() const;
    std::vector<bool> getLetters();
    void setLetters(const std::vector<std::string> &letters) const;
    void toggleLetters(const std::vector<bool> &letters) const;
    void resetLetters() const;
    std::vector<bool> getHiddenPowers();
    std::vector<bool> getNatures();
    u8 getShiny() const;
    bool useDelay() const;
    u32 getDelay() const;
    bool getDisableFilters();
    void disableControls(u16 control);
    void setGenderRatio(u8 genderRatio);
    void enableHiddenAbility();

private:
    Ui::UnownFilter *ui;
};

#endif // UNOWNFILTER_HPP
