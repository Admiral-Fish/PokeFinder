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

#include "Frame.hpp"
#include <Core/Util/Translator.hpp>

namespace PokeFinderCore
{

    QString Frame::getGenderString() const
    {
        switch (gender)
        {
        case 1:
            return "♂";
        case 2:
            return "♀";
        case 0:
        default:
            return "-";
        }
    }

    QString Frame::getNatureString() const { return Translator::getNature(nature); }

    QString Frame::getPowerString() const { return Translator::getPower(hidden); }

    QString Frame::getShinyString() const { return shiny ? "!!!" : ""; }

    void Frame::setIVs(u8 iv1, u8 iv2, u8 iv3, u8 iv4, u8 iv5, u8 iv6)
    {
        ivs[0] = iv1;
        ivs[1] = iv2;
        ivs[2] = iv3;
        ivs[3] = iv4;
        ivs[4] = iv5;
        ivs[5] = iv6;
        calculateHidden();
        calculatePower();
    }

    void Frame::setIDs(u16 tid, u16 sid, u16 psv)
    {
        this->tid = tid;
        this->sid = sid;
        this->psv = psv;
    }

    void Frame::setIVs(u16 iv1, u16 iv2)
    {
        ivs[0] = iv1 & 0x1f;
        ivs[1] = (iv1 >> 5) & 0x1f;
        ivs[2] = (iv1 >> 10) & 0x1f;
        ivs[3] = (iv2 >> 5) & 0x1f;
        ivs[4] = (iv2 >> 10) & 0x1f;
        ivs[5] = iv2 & 0x1f;
        calculateHidden();
        calculatePower();
    }

    u32 Frame::getFrame() const { return frame; }

    u8 Frame::getIV(int index) const { return ivs.at(index); }

    u16 Frame::getTID() const { return tid; }

    u16 Frame::getSID() const { return sid; }

    QChar Frame::getInheritance(int index) const { return inheritance.at(index); }

    u8 Frame::getPower() const { return power; }

    u32 Frame::getPID() const { return pid; }

    u8 Frame::getAbility() const { return ability; }

    Lead Frame::getLeadType() const { return leadType; }

    u8 Frame::getEncounterSlot() const { return encounterSlot; }

    void Frame::setEncounterSlot(u8 value) { encounterSlot = value; }

    u8 Frame::getLevel() const { return level; }

    void Frame::setLevel(u8 value) { level = value; }

    bool Frame::getShiny() const { return shiny; }

    u32 Frame::getNature() const { return nature; }

    void Frame::setNature(u8 value) { nature = value; }

    u8 Frame::getHidden() const { return hidden; }

    u8 Frame::getGender() const { return gender; }

    void Frame::setGender(u8 value) { gender = value; }

    void Frame::setFrame(u32 value) { frame = value; }

    void Frame::setLeadType(PokeFinderCore::Lead value) { leadType = value; }

    void Frame::calculateHidden()
    {
        hidden = ((((ivs.at(0) & 1) + 2 * (ivs.at(1) & 1) + 4 * (ivs.at(2) & 1) + 8 * (ivs.at(5) & 1)
                       + 16 * (ivs.at(3) & 1) + 32 * (ivs.at(4) & 1))
                      * 15)
            / 63);
    }

    void Frame::calculatePower()
    {
        power = 30
            + ((((ivs.at(0) >> 1) & 1) + 2 * ((ivs.at(1) >> 1) & 1) + 4 * ((ivs.at(2) >> 1) & 1)
                   + 8 * ((ivs.at(5) >> 1) & 1) + 16 * ((ivs.at(3) >> 1) & 1) + 32 * ((ivs.at(4) >> 1) & 1))
                * 40 / 63);
    }

}
