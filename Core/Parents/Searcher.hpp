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

#ifndef SEARCHER_HPP
#define SEARCHER_HPP

#include <QVector>
#include <Core/Parents/FrameCompare.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/Encounter.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Lead.hpp>
#include <Core/Util/Method.hpp>
#include <Core/Util/Utilities.hpp>

class Searcher
{

public:
    Encounter getEncounterType() const;
    void setEncounterType(const Encounter &value);
    Method getFrameType() const;
    void setFrameType(const Method &value);
    Lead getLeadType() const;
    void setLeadType(const Lead &value);

protected:
    u16 psv{};
    u16 sid{};
    u16 tid{};
    Encounter encounterType = Encounter::Stationary;
    Method frameType = Method::Method1;
    Lead leadType = Lead::None;
    u8 synchNature{};
    u8 cuteCharm{};

};

#endif // SEARCHER_HPP
