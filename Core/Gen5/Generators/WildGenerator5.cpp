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
#include <Core/Gen5/States/WildState5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <unordered_map>

struct WildStateKey
{
    std::array<u8, 6> ivs;
    u32 advances;
    u32 ivAdvances;
    u32 pid;
    u16 item;
    u16 specie;
    u8 ability;
    u8 gender;
    u8 level;
    u8 nature;
    u8 shiny;
    u8 encounterSlot;
    u8 form;

    bool operator==(const WildStateKey &other) const = default;
};

struct WildStateKeyHash
{
    size_t operator()(const WildStateKey &key) const
    {
        size_t hash = 0;
        auto combine = [&hash](auto value) {
            hash ^= static_cast<size_t>(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        };

        for (u8 iv : key.ivs)
        {
            combine(iv);
        }
        combine(key.advances);
        combine(key.ivAdvances);
        combine(key.pid);
        combine(key.item);
        combine(key.specie);
        combine(key.ability);
        combine(key.gender);
        combine(key.level);
        combine(key.nature);
        combine(key.shiny);
        combine(key.encounterSlot);
        combine(key.form);

        return hash;
    }
};

struct WildTargetKey
{
    std::array<u8, 6> ivs;
    u32 advances;
    u32 ivAdvances;
    u32 pid;
    u16 item;
    u16 specie;
    u8 ability;
    u8 gender;
    u8 level;
    u8 nature;
    u8 shiny;
    u8 encounterSlot;
    u8 form;

    bool operator==(const WildTargetKey &other) const = default;
};

struct WildTargetKeyHash
{
    size_t operator()(const WildTargetKey &key) const
    {
        size_t hash = 0;
        auto combine = [&hash](auto value) {
            hash ^= static_cast<size_t>(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        };

        for (u8 iv : key.ivs)
        {
            combine(iv);
        }
        combine(key.advances);
        combine(key.ivAdvances);
        combine(key.pid);
        combine(key.item);
        combine(key.specie);
        combine(key.ability);
        combine(key.gender);
        combine(key.level);
        combine(key.nature);
        combine(key.shiny);
        combine(key.encounterSlot);
        combine(key.form);

        return hash;
    }
};

static WildStateKey getStateKey(const WildState5 &state, bool normalizeNature)
{
    return { { state.getIV(0), state.getIV(1), state.getIV(2), state.getIV(3), state.getIV(4), state.getIV(5) },
             state.getAdvances(),
             state.getIVAdvances(),
             state.getPID(),
             state.getItem(),
             state.getSpecie(),
             state.getAbility(),
             state.getGender(),
             state.getLevel(),
             normalizeNature ? static_cast<u8>(255) : state.getNature(),
             state.getShiny(),
             state.getEncounterSlot(),
             state.getForm() };
}

static WildTargetKey getTargetKey(const WildState5 &state, u32 advances)
{
    return { { state.getIV(0), state.getIV(1), state.getIV(2), state.getIV(3), state.getIV(4), state.getIV(5) },
             advances,
             state.getIVAdvances(),
             state.getPID(),
             state.getItem(),
             state.getSpecie(),
             state.getAbility(),
             state.getGender(),
             state.getLevel(),
             state.getNature(),
             state.getShiny(),
             state.getEncounterSlot(),
             state.getForm() };
}

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

WildGenerator5::WildGenerator5(u32 initialAdvances, u32 maxAdvances, u32 offset, Method method, Lead lead, u8 luckyPower,
                               const EncounterArea5 &area, const Profile5 &profile, const WildStateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, offset, method, lead, area, profile, filter),
    luckyPower((profile.getVersion() & Game::BW) != Game::None ? 0 : luckyPower),
    leads({ lead })
{
}

WildGenerator5::WildGenerator5(u32 initialAdvances, u32 maxAdvances, u32 offset, Method method, const std::vector<Lead> &leads,
                               u8 luckyPower, const EncounterArea5 &area, const Profile5 &profile, const WildStateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, offset, method, leads.front(), area, profile, filter),
    luckyPower((profile.getVersion() & Game::BW) != Game::None ? 0 : luckyPower),
    leads(leads)
{
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
    std::vector<WildState5> states;
    std::unordered_map<WildStateKey, size_t, WildStateKeyHash> seen;
    for (Lead activeLead : leads)
    {
        auto leadStates = generate(seed, ivs, activeLead);
        states.reserve(states.size() + leadStates.size());
        bool normalizeNature = activeLead <= Lead::SynchronizeEnd && filter.allowsAllNatures();
        for (auto state : leadStates)
        {
            if (normalizeNature)
            {
                state.setVariableNature(true);
            }

            auto key = getStateKey(state, normalizeNature);
            auto entry = seen.find(key);
            if (entry == seen.end())
            {
                states.emplace_back(state);
                seen.emplace(key, states.size() - 1);
            }
            else
            {
                states[entry->second].addLead(state.getLead());
            }
        }
    }

    std::unordered_map<WildTargetKey, bool, WildTargetKeyHash> noneTargets;
    for (const auto &state : states)
    {
        if (state.getLead() == Lead::None)
        {
            noneTargets.emplace(getTargetKey(state, state.getAdvances()), true);
        }
    }

    if (!noneTargets.empty())
    {
        std::vector<WildState5> filtered;
        filtered.reserve(states.size());
        for (const auto &state : states)
        {
            if (state.getLead() == Lead::None || !noneTargets.contains(getTargetKey(state, state.getAdvances() + 1)))
            {
                filtered.emplace_back(state);
            }
        }
        return filtered;
    }

    return states;
}

std::vector<WildState5> WildGenerator5::generate(u64 seed, const std::vector<std::pair<u32, std::array<u8, 6>>> &ivs, Lead lead) const
{
    u32 advances = Utilities5::initialAdvances(seed, profile);
    BWRNG rng(seed, advances + initialAdvances);
    auto jump = rng.getJump(offset);

    bool bw = (profile.getVersion() & Game::BW) != Game::None;
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
        BWRNG go(rng, jump);

        bool cuteCharm = false;
        bool magnetStatic = false;
        bool pressure = false;
        bool sync = false;

        if (lead != Lead::CompoundEyes && lead != Lead::SuctionCups)
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

        u16 chatot = rng.nextUInt(0x1fff);
        for (const auto &iv : ivs)
        {
            WildState5 state(chatot, advances + initialAdvances + cnt, iv.first, pid, iv.second, ability, gender, level, nature, shiny,
                             encounterSlot, item, slot.getSpecie(), slot.getForm(), info, lead);
            if (filter.compareState(static_cast<const WildState &>(state)))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
