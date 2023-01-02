/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef WB8_HPP
#define WB8_HPP

#include <Core/Global.hpp>
#include <Core/Parents/PersonalLoader.hpp>

enum class Game : u32;

/**
 * @brief Contains information that impact the generation of events
 */
class WB8
{
public:
    /**
     * @brief Construct a new WB8 object
     *
     * @param data Raw wondercard data
     */
    WB8(const u8 *data) :
        ec(*reinterpret_cast<const u32 *>(data + 0x28)),
        pid(*reinterpret_cast<const u32 *>(data + 0x2c)),
        sid(*reinterpret_cast<const u16 *>(data + 0x22)),
        form(data[0x28a]),
        specie(*reinterpret_cast<const u16 *>(data + 0x288)),
        tid(*reinterpret_cast<const u16 *>(data + 0x20)),
        egg(data[0x28d] == 1),
        ability(data[0x28f]),
        gender(data[0x28b]),
        level(data[0x28c]),
        nature(data[0x28e]),
        shiny(data[0x290])
    {
        // Look at HP IV for flag
        bool ivFlag = data[0x2b2] - 0xfc < 3;
        ivCount = ivFlag ? (data[0x2b2] - 0xfb) : 0;
    }

    /**
     * @brief Construct a new WB8 object
     *
     * @param tid Template TID
     * @param sid Template SID
     * @param ec Template EC
     * @param pid Template PID
     * @param specie Template specie
     * @param form Template form
     * @param gender Template gender
     * @param egg Template egg
     * @param nature Template nature
     * @param ability Template ability
     * @param shiny Template shininess
     * @param ivCount Template 31 IV count
     * @param level Template level
     */
    WB8(u16 tid, u16 sid, u32 ec, u32 pid, u16 specie, u8 form, u8 gender, bool egg, u8 nature, u8 ability, u8 shiny, u8 ivCount,
        u8 level) :
        ec(ec),
        pid(pid),
        sid(sid),
        form(form),
        specie(specie),
        tid(tid),
        egg(egg),
        ability(ability),
        gender(gender),
        ivCount(ivCount),
        level(level),
        nature(nature),
        shiny(shiny)
    {
    }

    /**
     * @brief Determines what ability the template should have
     *
     * @return Template ability
     */
    u8 getAbility() const
    {
        return ability;
    }

    /**
     * @brief Determines what EC the template should have
     * @return
     */
    u32 getEC() const
    {
        return ec;
    }

    /**
     * @brief Determines if the template is a egg or not
     *
     * @return true Template is an egg
     * @return false Template is not an egg
     */
    bool getEgg() const
    {
        return egg;
    }

    /**
     * @brief Determines what form the template should have
     *
     * @return Template form
     */
    u8 getForm() const
    {
        return form;
    }

    /**
     * @brief Determines what gender the template should have
     *
     * @return Template gender
     */
    u8 getGender() const
    {
        return gender;
    }

    /**
     * @brief Returns the pokemon information
     *
     * @return Pokemon information
     */
    const PersonalInfo *getInfo(Game version) const
    {
        return PersonalLoader::getPersonal(version, specie, form);
    }

    /**
     * @brief Determines how many 31 IVs the template should have
     *
     * @return Template 31 IV count
     */
    u8 getIVCount() const
    {
        return ivCount;
    }

    /**
     * @brief Determines what level the template should have
     *
     * @return Template level
     */
    u8 getLevel() const
    {
        return level;
    }

    /**
     * @brief Determines what nature the template should have
     *
     * @return Template nature
     */
    u8 getNature() const
    {
        return nature;
    }

    /**
     * @brief Determines what PID the template should have
     *
     * @return Template PID
     */
    u32 getPID() const
    {
        return pid;
    }

    /**
     * @brief Determines what shininess the template should have
     *
     * @return Template shininess
     */
    u8 getShiny() const
    {
        return shiny;
    }

    /**
     * @brief Determines what SID the template should have
     *
     * @return Template SID
     */
    u16 getSID() const
    {
        return sid;
    }

    /**
     * @brief Determines what specie the template should have
     *
     * @return Template specie
     */
    u16 getSpecie() const
    {
        return specie;
    }

    /**
     * @brief Determines what TID the template should have
     *
     * @return Template TID
     */
    u16 getTID() const
    {
        return tid;
    }

private:
    u32 ec;
    u32 pid;
    u16 sid;
    u16 form : 5;
    u16 specie : 11;
    u16 tid;
    bool egg;
    u8 ability; // 0: 0, 1: 1, 2: H, 3: 1/2, 4: 1/2/H
    u8 gender;
    u8 ivCount;
    u8 level;
    u8 nature; // 0xff -> unset
    u8 shiny; // 0: never, 1: random, 2: star, 3: square, 4: static
};

#endif // WB8_HPP
