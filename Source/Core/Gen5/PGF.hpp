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

#ifndef PGF_HPP
#define PGF_HPP

#include <Core/Global.hpp>

/**
 * @brief Contains information that impact the generation of events
 */
class PGF
{
public:
    /**
     * @brief Construct a new WB8 object
     *
     * @param data Raw wondercard data
     */
    PGF(const u8 *data) :
        sid(*reinterpret_cast<const u16 *>(data + 0x2)),
        species(*reinterpret_cast<const u16 *>(data + 0x1a)),
        tid(*reinterpret_cast<const u16 *>(data + 0x0)),
        egg(data[0x5C] == 1),
        ability(data[0x36]),
        gender(data[0x35]),
        ivs { data[0x43], data[0x44], data[0x45], data[0x47], data[0x48], data[0x46] },
        level(data[0x5b]),
        nature(data[0x34]),
        shiny(data[0x37])
    {
    }

    /**
     * @brief Construct a new WB8 object
     *
     * @param tid Template TID
     * @param sid Template SID
     * @param specie Template specie
     * @param nature Template nature
     * @param gender Template gender
     * @param ability Template ability
     * @param shiny Template shininess
     * @param level Template level
     * @param hp Template HP
     * @param atk Template Atk
     * @param def Template Def
     * @param spa Template SpA
     * @param spd Template SpD
     * @param spe Template Spe
     * @param egg Template egg
     */
    PGF(u16 tid, u16 sid, u16 species, u8 nature, u8 gender, u8 ability, u8 shiny, u8 level, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
        bool egg) :
        sid(sid),
        species(species),
        tid(tid),
        egg(egg),
        ability(ability),
        gender(gender),
        ivs { hp, atk, def, spa, spd, spe },
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
     * @brief Returns number of advances consumed by the template
     *
     * @return Advances
     */
    u8 getAdvances() const
    {
        u8 advances = 8;

        for (u8 iv : ivs)
        {
            if (iv == 255) // Random IV
            {
                advances += 2;
            }
        }

        if (gender == 0 || gender == 1) // Forced male/female
        {
            advances += 2;
        }

        if (nature == 255) // Random nature
        {
            advances += 2;
        }

        return advances;
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
     * @brief Determines what gender the template should have
     *
     * @return Template gender
     */
    u8 getGender() const
    {
        return gender;
    }

    /**
     * @brief Returns the specified IV of the template
     *
     * @param index IV index to get
     *
     * @return Template IV
     */
    u8 getIV(u8 index) const
    {
        return ivs[index];
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
    u16 getSpecies() const
    {
        return species;
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
    u16 sid;
    u16 species;
    u16 tid;
    bool egg;
    u8 ability; // 0: 0, 1: 1, 2: H, 3: 1/2
    u8 gender; // 0: male, 1: female, 2: random
    u8 ivs[6]; // 0xff -> unset
    u8 level;
    u8 nature; // 0xff -> unset
    u8 shiny; // 0: no shiny, 1: allow shiny, 2: force shiny
};

#endif // PGF_HPP
