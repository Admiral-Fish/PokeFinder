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

#include "PokeRadarGenerator.hpp"

#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

static u32 getBattleAdvances(const EncounterArea4 &area, Game version)
{
    u32 advances = 0;

    if (area.getEncounter() == Encounter::OldRod || area.getEncounter() == Encounter::GoodRod || area.getEncounter() == Encounter::SuperRod)
    {
        advances += 1;
    }

    if ((version & Game::DP) != Game::None)
    {
        advances += 4;
    }

    if (!area.greatMarsh(version) && !area.safariZone(version))
    {
        advances += 1;
    }

    return advances;
}

static u16 getRadarItem(u8 rand, Lead lead, const PersonalInfo *info)
{
    constexpr u8 ItemTableRange[2][2] = { { 45, 95 }, { 20, 80 } };

    if (info->getItem(0) == info->getItem(1) && info->getItem(0) != 0)
    {
        return info->getItem(0);
    }
    else if (rand < ItemTableRange[lead == Lead::CompoundEyes ? 1 : 0][0])
    {
        return 0;
    }
    else if (rand < ItemTableRange[lead == Lead::CompoundEyes ? 1 : 0][1])
    {
        return info->getItem(0);
    }
    else
    {
        return info->getItem(1);
    }
}

PokeRadarGenerator::PokeRadarGenerator(u32 initialAdvances, u32 maxAdvances, u16 chainCount, PokeRadarChainType chainType,
                                       PokeRadarResult result, const std::array<bool, 81> &grass) :
    initialAdvances(initialAdvances),
    maxAdvances(maxAdvances),
    chainCount(chainCount),
    chainType(chainType),
    result(result),
    grass(grass)
{
}

std::pair<u8, u8> PokeRadarGenerator::getSkips(u32 seed, u32 advances)
{
    PokeRNG noGrace(seed, advances);
    u8 noGraceSkip = noGrace.nextUShort<false>(100) < 40 ? 1 : 0;

    PokeRNG grace(seed, advances);
    u8 graceSkip = 0;
    if (grace.nextUShort<false>(100) < 5)
    {
        graceSkip++;
        if (grace.nextUShort<false>(100) < 40)
        {
            graceSkip++;
        }
    }

    return { noGraceSkip, graceSkip };
}

std::vector<PokeRadarState> PokeRadarGenerator::generate(u32 seed) const
{
    std::vector<PokeRadarState> states;

    PokeRNG rng(seed, initialAdvances);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        PokeRNG go(rng);
        states.emplace_back(rng.nextUShort(), initialAdvances + cnt, generatePatches(go));
    }

    return states;
}

PokeRadarState PokeRadarGenerator::generate(u32 seed, u32 advances, u32 patchAdvances) const
{
    PokeRNG display(seed, advances);
    PokeRNG go(seed, patchAdvances);
    return PokeRadarState(display.nextUShort(), advances, generatePatches(go));
}

PokeRadarState PokeRadarGenerator::generatePrevious(u32 seed, u32 advances, u32 typeAdvances) const
{
    PokeRNG display(seed);
    PokeRNG go(seed);
    if (advances == 0)
    {
        PokeRNGR reverse(seed);
        go.setSeed(reverse.next());
    }
    else
    {
        go.jump(advances - 1);
    }
    return PokeRadarState(display.nextUShort(), advances == 0 ? 0 : advances - 1, generatePatches(go, typeAdvances));
}

u32 PokeRadarGenerator::getAdvanceConsumption(u32 seed, u32 advances, PokeRadarResult patchResult) const
{
    constexpr std::array<u8, 4> ringTileCount = { 32, 24, 16, 8 };
    constexpr std::array<u8, 4> ratesDefeat = { 88, 68, 48, 28 };
    constexpr std::array<u8, 4> ratesCapture = { 98, 78, 58, 38 };

    PokeRNG go(seed, advances);
    std::array<PokeRadarPatch, 4> patches;
    u32 consumed = 0;

    for (u8 ring = 0; ring < 4; ring++)
    {
        patches[ring] = buildPatch(ring, go.nextUShort<false>(ringTileCount[ring]));
        consumed++;
    }

    if (patchResult != PokeRadarResult::ManualActivation)
    {
        go.nextUShort();
        consumed++;
    }

    const auto &continueRates = patchResult == PokeRadarResult::Capture ? ratesCapture : ratesDefeat;
    const u16 effectiveChain = chainCount == 0 && patchResult != PokeRadarResult::ManualActivation ? 1 : chainCount;
    for (auto &patch : patches)
    {
        if (!patch.active)
        {
            continue;
        }

        bool rolledContinue = go.nextUShort<false>(100) < continueRates[patch.ring];
        consumed++;
        if (rolledContinue && effectiveChain != 0)
        {
            u16 shinyRate = std::max<u16>(8200 - std::min<u16>(effectiveChain, 40) * 200, 200);
            go.nextUShort<false>(shinyRate);
            consumed++;
        }
        else if (!rolledContinue)
        {
            go.nextUShort<false>(100);
            consumed++;
        }
    }

    return consumed;
}

u32 PokeRadarGenerator::getPostBattleAdvanceConsumption(const std::array<PokeRadarPatch, 4> &patches) const
{
    return static_cast<u32>(std::ranges::count_if(patches, [](const PokeRadarPatch &patch) { return patch.active; })) * 2;
}

PokeRadarPatch PokeRadarGenerator::buildPatch(u8 ring, u8 rand) const
{
    u8 size = 9 - ring * 2;
    u8 offset = ring;
    u8 x;
    u8 y;

    if (rand < size)
    {
        x = offset + rand;
        y = offset;
    }
    else if (rand < size * 2)
    {
        x = offset + rand - size;
        y = offset + size - 1;
    }
    else
    {
        u8 side = rand - size * 2;
        x = offset + (side % 2 == 0 ? 0 : size - 1);
        y = offset + (side / 2) + 1;
    }

    PokeRadarPatch patch = { x, y, ring, isGrass(x, y), false, false, false };
    return patch;
}

std::array<PokeRadarPatch, 4> PokeRadarGenerator::generatePatches(PokeRNG &go, u32 typeAdvances) const
{
    constexpr std::array<u8, 4> ringTileCount = { 32, 24, 16, 8 };
    std::array<PokeRadarPatch, 4> patches;
    PokeRadarResult effectiveResult = chainCount == 0 ? PokeRadarResult::ManualActivation : result;

    for (u8 ring = 0; ring < 4; ring++)
    {
        patches[ring] = buildPatch(ring, go.nextUShort<false>(ringTileCount[ring]));
    }

    go.jump(typeAdvances);

    if (effectiveResult != PokeRadarResult::ManualActivation)
    {
        go.nextUShort();
    }

    constexpr std::array<u8, 4> ratesDefeat = { 88, 68, 48, 28 };
    constexpr std::array<u8, 4> ratesCapture = { 98, 78, 58, 38 };
    const auto &continueRates = effectiveResult == PokeRadarResult::Capture ? ratesCapture : ratesDefeat;

    const bool strongChain = chainType == PokeRadarChainType::Strong || chainType == PokeRadarChainType::StrongShiny;
    for (auto &patch : patches)
    {
        if (!patch.active)
        {
            continue;
        }

        bool rolledContinue = go.nextUShort<false>(100) < continueRates[patch.ring];
        patch.continueChain = chainCount == 0 || rolledContinue;
        if (rolledContinue)
        {
            patch.strong = chainCount != 0 && strongChain;

            if (chainCount != 0)
            {
                u16 shinyRate = std::max<u16>(8200 - std::min<u16>(chainCount, 40) * 200, 200);
                patch.shiny = go.nextUShort<false>(shinyRate) == 0;
            }
        }
        else
        {
            patch.strong = go.nextUShort<false>(100) >= 50;
        }
    }

    return patches;
}

bool PokeRadarGenerator::isGrass(u8 x, u8 y) const
{
    return (x != 4 || y != 4) && grass[y * 9 + x];
}

PokeRadarPokemonGenerator::PokeRadarPokemonGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, Lead lead, bool shiny,
                                                     const EncounterArea4 &area, const Profile4 &profile,
                                                     const WildStateFilter &filter) :
    initialAdvances(initialAdvances),
    maxAdvances(maxAdvances),
    offset(offset),
    lead(lead),
    shiny(shiny),
    area(area),
    profile(profile),
    filter(filter),
    tsv(profile.getTID() ^ profile.getSID())
{
}

std::vector<WildGeneratorState4> PokeRadarPokemonGenerator::generate(u32 seed) const
{
    return shiny ? generateShiny(seed, std::nullopt) : generateNormal(seed, std::nullopt);
}

std::vector<WildGeneratorState4> PokeRadarPokemonGenerator::generate(u32 seed, u8 index) const
{
    return shiny ? generateShiny(seed, index) : generateNormal(seed, index);
}

std::vector<WildGeneratorState4> PokeRadarPokemonGenerator::generateNormal(u32 seed, std::optional<u8> index) const
{
    std::vector<WildGeneratorState4> states;

    PokeRNG rng(seed, initialAdvances);
    auto jump = rng.getJump(offset);
    u32 battleAdvancesConst = getBattleAdvances(area, profile.getVersion());

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        u32 battleAdvances = battleAdvancesConst + initialAdvances + offset + cnt;
        PokeRNG go(rng, jump);

        u8 encounterSlot = index.value_or(EncounterSlot::jSlot(go.nextUShort<false>(100, &battleAdvances), area.getEncounter()));
        if (!index.has_value() && !filter.compareEncounterSlot(encounterSlot))
        {
            rng.next();
            continue;
        }

        const Slot &slot = area.getPokemon(encounterSlot);
        const PersonalInfo *info = slot.getInfo();

        u8 buffer = 0;
        bool cuteCharm = false;
        if ((lead == Lead::CuteCharmF || lead == Lead::CuteCharmM) && !info->getFixedGender())
        {
            cuteCharm = true;
            if (lead == Lead::CuteCharmF)
            {
                buffer = 25 * ((info->getGender() / 25) + 1);
            }
        }

        bool cuteCharmFlag = false;
        if (cuteCharm)
        {
            cuteCharmFlag = go.nextUShort<false>(3, &battleAdvances) != 0;
        }

        u8 nature;
        if (lead <= Lead::SynchronizeEnd)
        {
            nature = go.nextUShort<false>(2, &battleAdvances) == 0 ? toInt(lead) : go.nextUShort<false>(25, &battleAdvances);
        }
        else
        {
            nature = go.nextUShort<false>(25, &battleAdvances);
        }

        if (!filter.compareNature(nature))
        {
            rng.next();
            continue;
        }

        u32 pid;
        if (cuteCharmFlag)
        {
            pid = buffer + nature;
        }
        else
        {
            do
            {
                u16 low = go.nextUShort(&battleAdvances);
                u16 high = go.nextUShort(&battleAdvances);
                pid = (high << 16) | low;
            } while (pid % 25 != nature);
        }

        u16 iv1 = go.nextUShort(&battleAdvances);
        u16 iv2 = go.nextUShort(&battleAdvances);
        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        u16 item = getRadarItem(go.nextUShort(100, &battleAdvances), lead, info);

        WildGeneratorState4 state(rng.nextUShort(), battleAdvances, initialAdvances + cnt, pid, ivs, pid & 1,
                                  Utilities::getGender(pid, info), slot.getMaxLevel(), nature, Utilities::getShiny<true>(pid, tsv),
                                  encounterSlot, item, slot.getSpecie(), 0, info);
        if (filter.compareState(static_cast<const WildGeneratorState &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<WildGeneratorState4> PokeRadarPokemonGenerator::generateShiny(u32 seed, std::optional<u8> index) const
{
    std::vector<WildGeneratorState4> states;

    PokeRNG rng(seed, initialAdvances);
    auto jump = rng.getJump(offset);
    u32 battleAdvancesConst = getBattleAdvances(area, profile.getVersion());

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        u32 battleAdvances = battleAdvancesConst + initialAdvances + offset + cnt;
        PokeRNG go(rng, jump);

        u8 encounterSlot = index.value_or(EncounterSlot::jSlot(go.nextUShort<false>(100, &battleAdvances), area.getEncounter()));
        if (!index.has_value() && !filter.compareEncounterSlot(encounterSlot))
        {
            rng.next();
            continue;
        }

        const Slot &slot = area.getPokemon(encounterSlot);
        const PersonalInfo *info = slot.getInfo();
        bool cuteCharm = (lead == Lead::CuteCharmF || lead == Lead::CuteCharmM) && !info->getFixedGender();

        auto cuteCharmCheck = [this](const PersonalInfo *info, u32 pid) {
            if (lead == Lead::CuteCharmF)
            {
                return (pid & 0xff) >= info->getGender();
            }
            return (pid & 0xff) < info->getGender();
        };

        auto shinyPID = [this, &go, &battleAdvances]() {
            u16 low = go.nextUShort(&battleAdvances) & 7;
            u16 high = go.nextUShort(&battleAdvances) & 7;
            u16 shinyValue = tsv >> 3;
            for (int i = 0; i < 13; i++)
            {
                u16 pidBit = 1 << (i + 3);
                if (shinyValue & (1 << i))
                {
                    if (go.nextUShort(&battleAdvances) & 1)
                    {
                        low |= pidBit;
                    }
                    else
                    {
                        high |= pidBit;
                    }
                }
                else if (go.nextUShort(&battleAdvances) & 1)
                {
                    low |= pidBit;
                    high |= pidBit;
                }
            }
            return static_cast<u32>((high << 16) | low);
        };

        u32 pid;
        if (cuteCharm && go.nextUShort<false>(3, &battleAdvances) != 0)
        {
            do
            {
                pid = shinyPID();
            } while (!cuteCharmCheck(info, pid));
        }
        else if (lead <= Lead::SynchronizeEnd && go.nextUShort<false>(2, &battleAdvances) == 0)
        {
            do
            {
                pid = shinyPID();
            } while (pid % 25 != toInt(lead));
        }
        else
        {
            pid = shinyPID();
        }

        u8 nature = pid % 25;
        if (!filter.compareNature(nature))
        {
            rng.next();
            continue;
        }

        u16 iv1 = go.nextUShort(&battleAdvances);
        u16 iv2 = go.nextUShort(&battleAdvances);
        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        u16 item = getRadarItem(go.nextUShort(100, &battleAdvances), lead, info);

        WildGeneratorState4 state(rng.nextUShort(), battleAdvances, initialAdvances + cnt, pid, ivs, pid & 1,
                                  Utilities::getGender(pid, info), slot.getMaxLevel(), nature, Utilities::getShiny<true>(pid, tsv),
                                  encounterSlot, item, slot.getSpecie(), 0, info);
        if (filter.compareState(static_cast<const WildGeneratorState &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
