/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "WildGenerator5.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Shiny.hpp>
#include <Core/Gen5/StepEncounter.hpp>
#include <Core/Gen5/States/WildState5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

static u8 gen(MT &rng)
{
    return rng.next() >> 27;
}

static u8 getEncounterRand(BWRNG &rng, u8 max, bool bw)
{
    if (bw)
    {
        return (rng.nextUInt(0xffff) / 656) % max;
    }
    else
    {
        return rng.nextUInt(max);
    }
}

static u8 getPercentRand(BWRNG &rng, bool bw)
{
    if (bw)
    {
        return rng.nextUInt(0xffff) / 656;
    }
    else
    {
        return rng.nextUInt(100);
    }
}

static u8 getMovingTrigger(BWRNG &rng)
{
    return (rng.nextUInt() >> 16) / 656;
}

static bool isStepModifier(Lead lead)
{
    return lead == Lead::ArenaTrap;
}

static u8 getLuckyPower(u8 passPower)
{
    u8 luckyPower = PassPower5::getLuckyPower(passPower);
    return luckyPower <= PassPower5::Lucky3 ? luckyPower : PassPower5::None;
}

static u16 getEncounterPowerModifier(u8 passPower)
{
    switch (PassPower5::getEncounterPower(passPower))
    {
    case 1:
        return 150;
    case 2:
        return 200;
    case 3:
        return 300;
    default:
        return 100;
    }
}

static u16 getStepEncounterModifier(Lead lead, u8 passPower)
{
    u16 modifier = isStepModifier(lead) ? 200 : 100;
    modifier = std::min<u16>(10000, modifier * getEncounterPowerModifier(passPower) / 100);
    return modifier;
}

static u8 getMovingTrigger(BWRNG &rng, bool bw, Lead lead, Encounter encounter)
{
    if (lead != Lead::None && lead != Lead::CompoundEyes && lead != Lead::SuctionCups && !(bw && isStepModifier(lead)))
    {
        if (lead == Lead::CuteCharmM || lead == Lead::CuteCharmF)
        {
            if (getPercentRand(rng, bw) >= 67)
            {
                getPercentRand(rng, bw);
            }
        }
        else
        {
            getPercentRand(rng, bw);
        }
    }

    if (encounter == Encounter::GrassDark)
    {
        getPercentRand(rng, bw);
    }

    return getMovingTrigger(rng);
}

static u16 getItem(BWRNG &rng, bool bw, Lead lead, Encounter encounter, const PersonalInfo *info)
{
    constexpr u8 ItemTable[2][3] = { { 50, 55, 0 }, { 60, 80, 0 } };
    constexpr u8 ItemTableRare[2][3] = { { 50, 55, 56 }, { 60, 80, 85 } };

    if (info->getItem(0) == info->getItem(1))
    {
        return info->getItem(0);
    }

    const u8 *table;
    if (encounter != Encounter::GrassDark)
    {
        table = ItemTable[lead == Lead::CompoundEyes ? 1 : 0];
    }
    else
    {
        table = ItemTableRare[lead == Lead::CompoundEyes ? 1 : 0];
    }

    u8 rand = getPercentRand(rng, bw);
    for (int i = 0; i < 3; i++)
    {
        if (rand < table[i])
        {
            return info->getItem(i);
        }
    }
    return 0;
}

WildGenerator5::WildGenerator5(u32 initialAdvances, u32 maxAdvances, u32 offset, Method method, Lead lead, u8 passPower,
                               bool searchMovingTrigger, bool requireMovingTrigger,
                               const EncounterArea5 &area, const Profile5 &profile, const WildStateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, offset, method, lead, area, profile, filter),
    passPower((profile.getVersion() & Game::BW) != Game::None ? PassPower5::combine(PassPower5::None, PassPower5::getEncounterPower(passPower)) : passPower),
    searchMovingTrigger(searchMovingTrigger),
    requireMovingTrigger(requireMovingTrigger)
{
    if (!searchMovingTrigger)
    {
        this->passPower = getLuckyPower(this->passPower);
    }
}

std::vector<WildState5> WildGenerator5::generate(u64 seed, u32 initialAdvances, u32 maxAdvances) const
{
    bool bw = (profile.getVersion() & Game::BW) != Game::None;

    std::vector<std::pair<u32, std::array<u8, 6>>> ivs;

    RNGList<u8, MT, 8, gen> rngList(seed >> 32, initialAdvances + (bw ? 0 : 2));
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        std::array<u8, 6> iv;
        std::ranges::generate(iv, [&rngList] { return rngList.next(); });
        if (filter.compareIV(iv))
        {
            ivs.emplace_back(initialAdvances + cnt, iv);
        }
    }

    if (ivs.empty())
    {
        return std::vector<WildState5>();
    }
    else
    {
        return generate(seed, ivs);
    }
}

std::vector<WildState5> WildGenerator5::generate(u64 seed, const std::vector<std::pair<u32, std::array<u8, 6>>> &ivs) const
{
    u8 luckyPower = getLuckyPower(passPower);
    u32 advances = Utilities5::initialAdvances(seed, profile);
    u32 start = advances + initialAdvances;
    bool bw2 = (profile.getVersion() & Game::BW2) != Game::None;
    bool bw = (profile.getVersion() & Game::BW) != Game::None;
    BWRNG rng(seed, start);
    BWRNG encounterRNG(seed, start + (searchMovingTrigger && bw2 ? 1 : 0));
    u32 triggerOffset = 0;
    if (searchMovingTrigger)
    {
        if (bw2)
        {
            triggerOffset = isStepModifier(lead) ? 0 : 1;
        }
        else if (bw && lead == Lead::None)
        {
            triggerOffset = 1;
        }
    }
    BWRNG triggerRNG(seed, start + triggerOffset);
    auto jump = rng.getJump(offset);

    auto modifiedSlots = area.getSlots(lead);

    u8 rate = area.getRate();
    if (area.getEncounter() == Encounter::SuperRod && lead == Lead::SuctionCups)
    {
        rate *= 2;
    }

    u8 shinyRolls = 1;
    if ((profile.getVersion() & Game::BW2) != Game::None)
    {
        if (profile.getShinyCharm())
        {
            shinyRolls += 2;
        }

        if (luckyPower == 3)
        {
            shinyRolls++;
        }
    }

    std::vector<WildState5> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(searchMovingTrigger ? encounterRNG : rng, jump);

        bool cuteCharm = false;
        bool magnetStatic = false;
        bool pressure = false;
        bool sync = false;

        if (searchMovingTrigger && bw2 && lead == Lead::None)
        {
            getPercentRand(go, bw);
            getPercentRand(go, bw);
        }
        else if (searchMovingTrigger && bw && lead == Lead::None)
        {
            getPercentRand(go, bw);
        }
        else if (lead != Lead::CompoundEyes && lead != Lead::SuctionCups && (!searchMovingTrigger || !(bw && isStepModifier(lead))))
        {
            // Failed cute charm continues to check for other leads
            if ((lead == Lead::CuteCharmM || lead == Lead::CuteCharmF) && getPercentRand(go, bw) < 67)
            {
                cuteCharm = true;
            }
            else
            {
                bool flag = getPercentRand(go, bw) >= 50;
                if (lead == Lead::MagnetPull || lead == Lead::Static)
                {
                    magnetStatic = flag;
                }
                else if (lead == Lead::Pressure)
                {
                    pressure = flag;
                }
                else if (lead <= Lead::SynchronizeEnd)
                {
                    sync = flag;
                }
            }
        }

        bool doubleBattle = false;
        if (area.getEncounter() == Encounter::GrassDark && getPercentRand(go, bw) < 40)
        {
            doubleBattle = true;
        }

        if (area.getEncounter() == Encounter::SuperRod && getPercentRand(go, bw) > rate)
        {
            rng.next();
            continue;
        }

        BWRNG triggerGo(triggerRNG, jump);
        u8 movingTrigger = searchMovingTrigger ? (bw2 ? getMovingTrigger(triggerGo) : getMovingTrigger(triggerGo, bw, lead, area.getEncounter()))
                                               : StepEncounter5::impossible;
        u8 movingSteps = searchMovingTrigger
            ? StepEncounter5::getSteps(profile.getVersion(), area.getEncounter(), area.getRate(), movingTrigger,
                                       getStepEncounterModifier(lead, passPower))
            : StepEncounter5::impossible;
        bool valid = !searchMovingTrigger || movingSteps != StepEncounter5::impossible;
        if (requireMovingTrigger && movingSteps == StepEncounter5::impossible)
        {
            rng.nextUInt(0x1fff);
            encounterRNG.next();
            triggerRNG.next();
            continue;
        }

        if (searchMovingTrigger && !bw2)
        {
            getMovingTrigger(go);
        }

        u8 encounterSlot;
        if (magnetStatic && !modifiedSlots.empty())
        {
            encounterSlot = modifiedSlots[getEncounterRand(go, modifiedSlots.count, bw)];
        }
        else
        {
            encounterSlot = EncounterSlot::bwSlot(getPercentRand(go, bw), area.getEncounter(), luckyPower);
        }

        u8 level = area.calculateLevel(encounterSlot, getPercentRand(go, bw), pressure);

        // RNG calls for left encounter slot and level
        if (doubleBattle)
        {
            go.advance(2);
        }

        const Slot &slot = area.getPokemon(encounterSlot);
        const PersonalInfo *info = slot.getInfo();

        u32 pid;
        for (u8 i = 0; i < shinyRolls; i++)
        {
            // Only allow cute charm if the target isn't fixed gender
            u8 gender = cuteCharm && !info->getFixedGender() ? (lead == Lead::CuteCharmF ? 0 : 1) : 255;

            pid = Utilities5::createPID(tsv, 2, gender, Shiny::Random, true, info->getGender(), go);
            if (Utilities::isShiny<true>(pid, tsv))
            {
                break;
            }
        }

        u8 ability = (pid >> 16) & 1;
        u8 gender = Utilities::getGender(pid, info);
        u8 shiny = Utilities::getShiny<true>(pid, tsv);

        u8 nature = go.nextUInt(25);
        if (sync)
        {
            nature = toInt(lead);
        }

        u16 item = getItem(go, bw, lead, area.getEncounter(), info);

        u32 prng = rng.nextUInt();
        if (searchMovingTrigger)
        {
            encounterRNG.next();
            triggerRNG.next();
        }
        for (const auto &iv : ivs)
        {
            WildState5 state(prng, movingTrigger, movingSteps, advances + initialAdvances + cnt, iv.first, pid, iv.second, ability, gender,
                             level, nature, shiny, encounterSlot, item, slot.getSpecie(), slot.getForm(), info, valid);
            if (!valid || filter.compareState(static_cast<const WildState &>(state)))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
