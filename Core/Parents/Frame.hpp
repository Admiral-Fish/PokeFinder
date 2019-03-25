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

#ifndef FRAME_HPP
#define FRAME_HPP

#include <QString>
#include <QVector>
#include <Core/Util/Encounter.hpp>
#include <Core/Util/Global.hpp>
#include <Core/Util/Lead.hpp>
#include <Core/Util/Method.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Power.hpp>

class Frame
{

public:
    QString getGenderString();
    QString getNatureString();
    QString getPowerString();
    QString getShinyString();
    void setIVsManual(u8 iv1, u8 iv2, u8 iv3, u8 iv4, u8 iv5, u8 iv6);
    void setIDs(u16 tid, u16 sid, u16 psv);
    void setIVs(u16 iv1, u16 iv2);
    void setPID(u16 pid1, u16 pid2);
    void setPID(u32 pid, u16 pid1, u16 pid2);
    void setPID(u32 pid);
    u32 getFrame() const;
    u8 getIV(int index) const;
    QChar getInheritance(int index) const;
    u8 getPower() const;
    u32 getPID() const;
    u8 getAbility() const;
    Lead getLeadType() const;
    u8 getEncounterSlot() const;
    void setEncounterSlot(const u8 &value);
    u8 getLevel() const;
    void setLevel(const u8 &value);
    bool getShiny() const;
    u32 getNature() const;
    void setNature(const u8 &value);
    u8 getHidden() const;
    u16 getGender() const;
    u8 getGenderRatio() const;
    void setGenderRatio(const u8 &value);
    void setFrame(const u32 &value);
    void setLeadType(const Lead &value);

protected:
    u8 ivs[6];
    QChar inheritance[6];
    u8 ability;
    u8 encounterSlot;
    Encounter encounterType;
    u32 frame;
    u16 gender;
    u8 genderRatio;
    u8 hidden;
    Lead leadType = Lead::None;
    u8 nature;
    u32 pid;
    u8 power;
    u16 psv;
    bool shiny;
    u16 sid;
    u16 tid;
    u8 level;

};

#endif // FRAME_HPP
