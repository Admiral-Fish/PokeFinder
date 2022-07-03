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

#ifndef DREAMRADARTEMPLATE_HPP
#define DREAMRADARTEMPLATE_HPP

#include <Core/Parents/StaticTemplate.hpp>

/**
 * @brief Contains information that impact the generation of dream radar encounters
 */
class DreamRadarTemplate : public StaticTemplate
{
public:
    /**
     * @brief Construct a new DreamRadarTemplate object
     *
     * @param specie Template specie
     * @param form Template form
     * @param ability Template ability
     */
    constexpr DreamRadarTemplate(u16 specie, u8 form, u8 ability) :
        StaticTemplate(Game::BW2, specie, form, Shiny::Never, ability, 255, 0, 0),
        genie(specie == 641 || specie == 642 || specie == 645),
        legend(genie || specie == 249 || specie == 250 || specie == 483 || specie == 484 || specie == 487)
    {
    }

    /**
     * @brief Determines if the template is a genie legendary
     *
     * @return Template genie status
     */
    bool getGenie() const
    {
        return genie;
    }

    /**
     * @brief Determines if the template is a Gen 4 legendary
     *
     * @return Template Gen 4 legend status
     */
    bool getLegend() const
    {
        return legend;
    }

    /**
     * @brief Sets what gender the template should have
     * This isn't considered "Static" information and needs to be specified after the fact
     *
     * @param gender Template gender
     */
    void setGender(u8 gender)
    {
        this->gender = gender;
    }

private:
    bool genie;
    bool legend;
};

#endif // DREAMRADARTEMPLATE_HPP
