/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef SHADOWLOCK_HPP
#define SHADOWLOCK_HPP

#include <Core/Global.hpp>
#include <vector>

class ShadowTemplate;

/**
 * @brief Computes whether a origin seed for a PID/IV combination passes the shadow locks of the target
 */
namespace ShadowLock
{
    /**
     * @brief Determines whether the \p seed matches the lock criteria for Colosseum
     * The following is technically shiny locked by the trainer TID/SID.  It is extremely hard to know what that value is working from only
     * IVs. Assume that the shiny lock won't play a significant factor.
     *
     * @param seed Origin seed
     * @param shadowTemplate Pokemon template
     *
     * @return true Seed matches the lock criteria
     * @return false Seed does not match the lock criteria
     */
    bool coloShadow(u32 seed, const ShadowTemplate *shadowTemplate);

    /**
     * @brief Determines whether the \p seed matches the lock criteria for Colosseum E-Reader
     * The following is technically shiny locked by the trainer TID/SID. It is extremely hard to know what that value is working from only
     * IVs. Assume that the shiny lock won't play a significant factor.
     *
     * @param seed Origin seed
     * @param readerPID PID of the E-Reader pokemon
     * @param shadowTemplate Pokemon template
     *
     * @return true Seed matches the lock criteria
     * @return false Seed does not match the lock criteria
     */
    bool ereader(u32 seed, u32 readerPID, const ShadowTemplate *shadowTemplate);

    /**
     * @brief firstShadowNormal
     * Working backwards it is hard to know what PID would get rerolled from shiny lock. Only check shiny lock working forwards which should
     * be good enough.
     *
     * @param seed Origin seed
     * @param tsv Trainer shiny value
     * @param shadowTemplate Pokemon template
     *
     * @return true Seed matches the lock criteria
     * @return false Seed does not match the lock criteria
     */
    bool firstShadowNormal(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate);

    /**
     * @brief firstShadowSet
     *
     * @param seed Origin seed
     * @param tsv Trainer shiny value
     * @param shadowTemplate Pokemon template
     *
     * @return true Seed matches the lock criteria
     * @return false Seed does not match the lock criteria
     */
    bool firstShadowSet(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate);

    /**
     * @brief firstShadowUnset
     *
     * @param seed Origin seed
     * @param tsv Trainer shiny value
     * @param shadowTemplate Pokemon template
     *
     * @return true Seed matches the lock criteria
     * @return false Seed does not match the lock criteria
     */
    bool firstShadowUnset(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate);

    /**
     * @brief salamenceSet
     *
     * @param seed Origin seed
     * @param tsv Trainer shiny value
     * @param shadowTemplate Pokemon template
     *
     * @return true Seed matches the lock criteria
     * @return false Seed does not match the lock criteria
     */
    bool salamenceSet(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate);

    /**
     * @brief salamenceUnset
     *
     * @param seed Origin seed
     * @param tsv Trainer shiny value
     * @param shadowTemplate Pokemon template
     *
     * @return true Seed matches the lock criteria
     * @return false Seed does not match the lock criteria
     */
    bool salamenceUnset(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate);

    /**
     * @brief singleNL
     *
     * @param seed Origin seed
     * @param tsv Trainer shiny value
     * @param shadowTemplate Pokemon template
     *
     * @return true Seed matches the lock criteria
     * @return false Seed does not match the lock criteria
     */
    bool singleNL(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate);
};

#endif // SHADOWLOCK_HPP
