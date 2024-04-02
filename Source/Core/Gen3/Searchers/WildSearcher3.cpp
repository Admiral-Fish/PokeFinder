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

#include "WildSearcher3.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Parents/States/WildState.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Utilities.hpp>

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

static u8 unownLetter(u32 pid)
{
    return (((pid & 0x3000000) >> 18) | ((pid & 0x30000) >> 12) | ((pid & 0x300) >> 6) | (pid & 0x3)) % 0x1c;
}

WildSearcher3::WildSearcher3(Method method, Lead lead, const EncounterArea3 &area, const Profile3 &profile, const WildStateFilter &filter) :
    WildSearcher(method, lead, area, profile, filter), modifiedSlots(area.getSlots(lead)), ivAdvance(method == Method::Method2), rate(0)
{
    if ((profile.getVersion() & Game::RSE) != Game::None && area.getEncounter() == Encounter::RockSmash)
    {
        rate = area.getRate() * 16;
    }
}

void WildSearcher3::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max)
{
    searching = true;

    bool safari = area.safariZone(profile.getVersion());
    bool tanoby = area.tanobyChamber(profile.getVersion());

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

                            auto states = search(hp, atk, def, spa, spd, spe, safari, tanoby);

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

std::vector<WildSearcherState> WildSearcher3::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, bool safari, bool tanoby) const
{
    std::vector<WildSearcherState> states;
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

        u32 pid;

        u8 letter;
        if (tanoby)
        {
            pid = rng.nextUShort();
            pid |= rng.nextUShort() << 16;
            letter = unownLetter(pid);
        }
        else
        {
            pid = rng.nextUShort() << 16;
            pid |= rng.nextUShort();
        }

        u8 nature = pid % 25;
        if (!filter.compareNature(nature))
        {
            continue;
        }

        u16 nextRNG = rng.nextUShort();
        u16 nextRNG2 = rng.nextUShort();

        do
        {
            bool cuteCharmFlag = false;
            u8 encounterSlot[2] = { false, false };
            u8 level[2];
            PokeRNGR test[2] = { rng, rng };
            bool valid[2] = { false, false };

            switch (lead)
            {
            case Lead::None:
                if (tanoby)
                {
                    encounterSlot[0] = EncounterSlot::hSlot(nextRNG2 % 100, area.getEncounter());
                    level[0] = area.EncounterArea::calculateLevel(encounterSlot[0], nextRNG);
                    valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                }
                else if ((nextRNG % 25) == nature)
                {
                    u16 prng = safari ? test[0].nextUShort() : nextRNG2;
                    encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                    level[0] = area.EncounterArea::calculateLevel(encounterSlot[0], prng);
                    valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                }
                break;
            case Lead::CuteCharmF:
            case Lead::CuteCharmM:
                // Check gender
                if ((nextRNG % 25) == nature)
                {
                    if (safari)
                    {
                        test[0].next();
                    }
                    u16 prng = test[0].nextUShort();
                    encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                    level[0] = area.EncounterArea::calculateLevel(encounterSlot[0], prng);
                    cuteCharmFlag = (nextRNG2 % 3) > 0;
                    valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                }
                break;
            case Lead::Synchronize:
                if ((nextRNG & 1) == 0)
                {
                    u16 prng = safari ? test[0].nextUShort() : nextRNG2;
                    encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                    level[0] = area.EncounterArea::calculateLevel(encounterSlot[0], prng);
                    valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                }

                if ((nextRNG2 & 1) == 1 && (nextRNG % 25) == nature)
                {
                    if (safari)
                    {
                        test[1].next();
                    }
                    u16 prng = test[1].nextUShort();
                    encounterSlot[1] = EncounterSlot::hSlot(test[1].nextUShort(100), area.getEncounter());
                    level[1] = area.EncounterArea::calculateLevel(encounterSlot[1], prng);
                    valid[1] = filter.compareEncounterSlot(encounterSlot[1]);
                }
                break;
            case Lead::MagnetPull:
            case Lead::Static:
                if ((nextRNG % 25) == nature)
                {
                    u16 levelRand = safari ? test[0].nextUShort() : nextRNG2;
                    u16 encounterRand = test[0].nextUShort();
                    if (test[0].nextUShort(2) == 0 && !modifiedSlots.empty())
                    {
                        encounterSlot[0] = modifiedSlots[encounterRand % modifiedSlots.size()];
                    }
                    else
                    {
                        encounterSlot[0] = EncounterSlot::hSlot(encounterRand % 100, area.getEncounter());
                    }
                    level[0] = area.EncounterArea::calculateLevel(encounterSlot[0], levelRand);
                    valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                }
                break;
            case Lead::Pressure:
                if ((nextRNG % 25) == nature)
                {
                    u16 rand = safari ? test[0].nextUShort() : nextRNG2;
                    u16 levelRand = test[0].nextUShort();
                    encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                    level[0] = area.calculateLevel(encounterSlot[0], levelRand, (rand & 1) == 0);
                    valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                }
                break;
            default:
                break;
            }

            for (int i = 0; i < 2; i++)
            {
                if (valid[i] && (rate == 0 || (test[i].nextUShort(2880) < rate)))
                {
                    const Slot &slot = area.getPokemon(encounterSlot[i]);
                    const PersonalInfo *info = slot.getInfo();
                    if ((!cuteCharmFlag || cuteCharmGender(info, pid, lead))
                        && (slot.getSpecie() != 201 || unownLetter(pid) == slot.getForm()))
                    {
                        WildSearcherState state(test[i].next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), level[i], nature,
                                                Utilities::getShiny<true>(pid, tsv), encounterSlot[i], 0, slot.getSpecie(), slot.getForm(), info);
                        if (filter.compareState(state))
                        {
                            states.emplace_back(state);
                        }
                    }
                }
            }

            if (tanoby)
            {
                u8 huntLetter = unownLetter(static_cast<u32>((nextRNG2 << 16) | nextRNG));
                if (huntLetter == letter)
                {
                    break;
                }
            }
            else
            {
                u8 huntNature = static_cast<u32>((nextRNG << 16) | nextRNG2) % 25;
                if (huntNature == nature)
                {
                    break;
                }
            }

            nextRNG = rng.nextUShort();
            nextRNG2 = rng.nextUShort();
        } while (true);
    }

    return states;
}
