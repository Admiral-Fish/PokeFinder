/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#ifndef FRAMEGEN3_HPP
#define FRAMEGEN3_HPP
#include <libPokeFinder/Objects/Frame.hpp>
#include <QList>
#include <QStandardItem>

class FrameGen3: public Frame
{

private:
    uint32_t ABCDS[5] = { 1, 2, 5, 3, 4 };
    uint32_t ACDS[4] = { 1, 5, 3, 4 };
    uint32_t HABCDS[6] = { 0, 1, 2, 5, 3, 4 };

public:
    uint32_t inh1;
    uint32_t inh2;
    uint32_t inh3;
    std::string lockReason;
    uint32_t occidentary;
    uint32_t par1;
    uint32_t par2;
    uint32_t par3;

    FrameGen3(uint32_t tid, uint32_t sid, uint32_t psv);

    QString GetTime();

    void SetIDs(uint32_t tid, uint32_t sid, uint32_t psv);

    void SetIVsManual(uint32_t iv1, uint32_t iv2, uint32_t iv3, uint32_t iv4, uint32_t iv5, uint32_t iv6);

    QList<QStandardItem *> GetTableRow(int genderRatioIndex);

};

#endif // FRAMEGEN3_HPP
