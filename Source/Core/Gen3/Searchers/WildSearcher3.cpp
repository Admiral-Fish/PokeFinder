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

#include "WildSearcher3.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/States/WildState3.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>
#include <Core/Util/EncounterSlot.hpp>

static bool cuteCharmGender(const PersonalInfo *info, u32 pid, Lead lead)
{
    switch (info->getGender())
    {
    case 0:
    case 254:
    case 255:
        return false;
    default:
        if (lead == Lead::CuteCharmF)
        {
            return (pid & 255) >= info->getGender();
        }
        else
        {
            return (pid & 255) < info->getGender();
        }
    }
}

static u8 getGender(u32 pid, const PersonalInfo *info)
{
    switch (info->getGender())
    {
    case 255: // Genderless
        return 2;
        break;
    case 254: // Female
        return 1;
        break;
    case 0: // Male
        return 0;
        break;
    default: // Random gender
        return (pid & 255) < info->getGender();
        break;
    }
}

static u8 getShiny(u32 pid, u16 tsv)
{
    u16 psv = (pid >> 16) ^ (pid & 0xffff);
    if (tsv == psv)
    {
        return 2; // Square
    }
    else if ((tsv ^ psv) < 8)
    {
        return 1; // Star
    }
    else
    {
        return 0;
    }
}

static bool unownCheck(u32 pid, u8 form)
{
    u8 letter = (((pid & 0x3000000) >> 18) | ((pid & 0x30000) >> 12) | ((pid & 0x300) >> 6) | (pid & 0x3)) % 0x1C;
    return letter == form;
}

WildSearcher3::WildSearcher3(Method method, Encounter encounter, Lead lead, const EncounterArea3 &encounterArea, const Profile3 &profile,
                             const WildStateFilter3 &filter) :
    WildSearcher(method, encounter, lead, encounterArea, profile, filter),
    modifiedSlots(encounterArea.getSlots(profile.getVersion(), lead)),
    progress(0),
    ivAdvance(method == Method::Method2),
    searching(false)
{
}

void WildSearcher3::cancelSearch()
{
    searching = false;
}

int WildSearcher3::getProgress() const
{
    return progress;
}

std::vector<WildSearcherState3> WildSearcher3::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data = std::move(results);
    return data;
}

void WildSearcher3::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max)
{
    searching = true;

    bool safari = encounterArea.safariZone(profile.getVersion());

    for (u8 hp = min[0]; hp <= max[0]; hp++)
    {
        for (u8 atk = min[1]; atk <= max[1]; atk++)
        {
            for (u8 def = min[2]; def <= max[2]; def++)
            {
                for (u8 spa = min[3]; spa <= max[3]; spa++)
                {
                    for (u8 spd = min[4]; spd <= max[4]; spd++)
                    {
                        for (u8 spe = min[5]; spe <= max[5]; spe++)
                        {
                            if (!searching)
                            {
                                return;
                            }

                            auto states = search(hp, atk, def, spa, spd, spe, safari);

                            std::lock_guard<std::mutex> guard(mutex);
                            results.insert(results.end(), states.begin(), states.end());
                            progress++;
                        }
                    }
                }
            }
        }
    }
}

std::vector<WildSearcherState3> WildSearcher3::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, bool safari) const
{
    std::vector<WildSearcherState3> states;
    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, method);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);
        if (ivAdvance)
        {
            rng.next();
        }

        u32 pid = rng.nextUShort() << 16;
        pid |= rng.nextUShort();

        u8 nature = pid % 25;
        if (!filter.compareNature(nature))
        {
            continue;
        }

        u8 huntNature;
        u16 nextRNG = rng.nextUShort();
        u16 nextRNG2 = rng.nextUShort();

        do
        {
            PokeRNGR test(rng);

            u8 encounterSlot;
            u8 level;
            bool cuteCharmFlag = false;
            bool valid = false;
            switch (lead)
            {
            case Lead::None:
                if ((nextRNG % 25) == nature)
                {
                    u16 prng = safari ? test.nextUShort() : nextRNG2;
                    encounterSlot = EncounterSlot::hSlot(test.nextUShort(100), encounter);
                    level = encounterArea.EncounterArea::calculateLevel(encounterSlot, prng);
                    valid = filter.compareEncounterSlot(encounterSlot);
                }
                break;
            case Lead::CuteCharmF:
            case Lead::CuteCharmM:
                // Check gender
                if ((nextRNG % 25) == nature)
                {
                    if (safari)
                    {
                        test.next();
                    }
                    u16 prng = test.nextUShort();
                    encounterSlot = EncounterSlot::hSlot(test.nextUShort(100), encounter);
                    level = encounterArea.EncounterArea::calculateLevel(encounterSlot, prng);
                    cuteCharmFlag = (nextRNG2 % 3) > 0;
                    valid = filter.compareEncounterSlot(encounterSlot);
                }
                break;
            case Lead::Synchronize:
                if ((nextRNG & 1) == 0)
                {
                    u16 prng = safari ? test.nextUShort() : nextRNG2;
                    encounterSlot = EncounterSlot::hSlot(test.nextUShort(100), encounter);
                    level = encounterArea.EncounterArea::calculateLevel(encounterSlot, prng);
                    valid = filter.compareEncounterSlot(encounterSlot);
                }
                else if ((nextRNG2 & 1) == 1 && (nextRNG % 25) == nature)
                {
                    if (safari)
                    {
                        test.next();
                    }
                    u16 prng = test.nextUShort();
                    encounterSlot = EncounterSlot::hSlot(test.nextUShort(100), encounter);
                    level = encounterArea.EncounterArea::calculateLevel(encounterSlot, prng);
                    valid = filter.compareEncounterSlot(encounterSlot);
                }
                break;
            case Lead::MagnetPull:
            case Lead::Static:
                if ((nextRNG % 25) == nature)
                {
                    u16 levelRand = safari ? test.nextUShort() : nextRNG2;
                    u16 encounterRand = test.nextUShort();
                    if (test.nextUShort(2) == 0 && !modifiedSlots.empty())
                    {
                        encounterSlot = modifiedSlots[encounterRand % modifiedSlots.size()];
                    }
                    else
                    {
                        encounterSlot = EncounterSlot::hSlot(encounterRand % 100, encounter);
                    }
                    level = encounterArea.EncounterArea::calculateLevel(encounterSlot, levelRand);
                    valid = filter.compareEncounterSlot(encounterSlot);
                }
                break;
            case Lead::Pressure:
                if ((nextRNG % 25) == nature)
                {
                    u16 rand = safari ? test.nextUShort() : nextRNG2;
                    u16 levelRand = test.nextUShort();
                    encounterSlot = EncounterSlot::hSlot(test.nextUShort(100), encounter);
                    level = encounterArea.calculateLevel(encounterSlot, levelRand, (rand & 1) == 0);
                    valid = filter.compareEncounterSlot(encounterSlot);
                }
                break;
            default:
                break;
            }

            if (valid)
            {
                const Slot &slot = encounterArea.getPokemon(encounterSlot);
                const PersonalInfo *info = slot.getInfo();
                if ((!cuteCharmFlag || cuteCharmGender(info, pid, lead)) && (slot.getSpecie() != 201 || unownCheck(pid, slot.getForm())))
                {
                    WildSearcherState3 state(test.next(), pid, ivs, pid & 1, getGender(pid, info), level, nature, getShiny(pid, tsv),
                                             encounterSlot, slot.getSpecie(), slot.getForm(), info);
                    if (filter.compareState(state))
                    {
                        states.emplace_back(state);
                    }
                }
            }

            huntNature = static_cast<u32>((nextRNG << 16) | nextRNG2) % 25;
            nextRNG = rng.nextUShort();
            nextRNG2 = rng.nextUShort();
        } while (huntNature != nature);
    }

    // RSE rock smash is dependent on origin seed for encounter check
    if ((profile.getVersion() & Game::RSE) != Game::None && encounter == Encounter::RockSmash)
    {
        u16 rate = encounterArea.getRate() * 16;
        for (size_t i = 0; i < states.size();)
        {
            u16 check = states[i].getSeed() >> 16;
            if ((check % 2880) >= rate)
            {
                states.erase(states.begin() + i);
            }
            else
            {
                states[i].setSeed(PokeRNGR(states[i].getSeed()).next());
                i++;
            }
        }
    }

    return states;
}
