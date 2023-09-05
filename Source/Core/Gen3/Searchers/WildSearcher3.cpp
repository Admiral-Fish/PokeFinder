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

constexpr u8 feebasSlots[] = { 2, 3, 5 };

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

WildSearcher3::WildSearcher3(Method method, Lead lead, bool feebasTile, const EncounterArea3 &area, const Profile3 &profile,
                             const WildStateFilter &filter) :
    WildSearcher(method, lead, area, profile, filter),
    ivAdvance(method == Method::Method2),
    feebasTile(feebasTile),
    rate(0),
    modifiedSlots(area.getSlots(lead))
{
    if ((profile.getVersion() & Game::RSE) != Game::None && area.getEncounter() == Encounter::RockSmash)
    {
        rate = area.getRate() * 16;
    }
}

void WildSearcher3::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max)
{
    searching = true;

    bool feebas = area.feebasLocation(profile.getVersion())
        && (area.getEncounter() == Encounter::OldRod || area.getEncounter() == Encounter::GoodRod
            || area.getEncounter() == Encounter::SuperRod);
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

                            auto states = search(hp, atk, def, spa, spd, spe, feebas, safari, tanoby);

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

std::vector<WildSearcherState> WildSearcher3::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, bool feebas, bool safari,
                                                     bool tanoby) const
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
            u8 encounterSlot[4];
            bool force = false;
            u16 levelRand[2];
            PokeRNGR test[4] = { rng, rng, rng, rng };
            bool valid[4] = { false, false, false, false };

            switch (lead)
            {
            case Lead::None:
                if (tanoby)
                {
                    levelRand[0] = nextRNG;
                    encounterSlot[0] = EncounterSlot::hSlot(nextRNG2 % 100, area.getEncounter());
                    valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                }
                else if ((nextRNG % 25) == nature)
                {
                    levelRand[0] = safari ? test[0].nextUShort() : nextRNG2;

                    if (feebas)
                    {
                        if (feebasTile)
                        {
                            if (test[0].nextUShort(100) < 50)
                            {
                                encounterSlot[0] = feebasSlots[toInt(area.getEncounter() - Encounter::OldRod)];
                                valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                            }

                            u8 rand = test[1].nextUShort(100);
                            if (test[1].nextUShort(100) >= 50)
                            {
                                encounterSlot[1] = EncounterSlot::hSlot(rand, area.getEncounter());
                                valid[1] = filter.compareEncounterSlot(encounterSlot[1]);
                            }
                        }
                        else
                        {
                            test[0].advance(1);
                            encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                            valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                        }
                    }
                    else
                    {
                        encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                        valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                    }
                }
                break;
            case Lead::CuteCharmF:
            case Lead::CuteCharmM:
                if ((nextRNG % 25) == nature)
                {
                    cuteCharmFlag = (nextRNG2 % 3) > 0;
                    if (safari)
                    {
                        test[0].next();
                    }
                    levelRand[0] = test[0].nextUShort();

                    if (feebas)
                    {
                        if (feebasTile)
                        {
                            if (test[0].nextUShort(100) < 50)
                            {
                                encounterSlot[0] = feebasSlots[toInt(area.getEncounter() - Encounter::OldRod)];
                                valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                            }

                            u8 rand = test[1].nextUShort(100);
                            if (test[1].nextUShort(100) >= 50)
                            {
                                encounterSlot[1] = EncounterSlot::hSlot(rand, area.getEncounter());
                                valid[1] = filter.compareEncounterSlot(encounterSlot[1]);
                            }
                        }
                        else
                        {
                            test[0].advance(1);
                            encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                            valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                        }
                    }
                    else
                    {
                        encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                        valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                    }
                }
                break;
            case Lead::Synchronize:
                if ((nextRNG & 1) == 0)
                {
                    levelRand[0] = safari ? test[0].nextUShort() : nextRNG2;

                    if (feebas)
                    {
                        if (feebasTile)
                        {
                            if (test[0].nextUShort(100) < 50)
                            {
                                encounterSlot[0] = feebasSlots[toInt(area.getEncounter() - Encounter::OldRod)];
                                valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                            }

                            u8 rand = test[1].nextUShort(100);
                            if (test[1].nextUShort(100) >= 50)
                            {
                                encounterSlot[1] = EncounterSlot::hSlot(rand, area.getEncounter());
                                valid[1] = filter.compareEncounterSlot(encounterSlot[1]);
                            }
                        }
                        else
                        {
                            test[0].advance(1);
                            encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                            valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                        }
                    }
                    else
                    {
                        encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                        valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                    }
                }

                if ((nextRNG2 & 1) == 1 && (nextRNG % 25) == nature)
                {
                    if (safari)
                    {
                        test[1].next();
                    }
                    levelRand[1] = test[2].nextUShort();

                    if (feebas)
                    {
                        if (feebasTile)
                        {
                            if (test[2].nextUShort(100) < 50)
                            {
                                encounterSlot[2] = feebasSlots[toInt(area.getEncounter() - Encounter::OldRod)];
                                valid[2] = filter.compareEncounterSlot(encounterSlot[2]);
                            }

                            test[3].advance(1);
                            u8 rand = test[3].nextUShort(100);
                            if (test[3].nextUShort(100) >= 50)
                            {
                                encounterSlot[3] = EncounterSlot::hSlot(rand, area.getEncounter());
                                valid[3] = filter.compareEncounterSlot(encounterSlot[3]);
                            }
                        }
                        else
                        {
                            test[2].advance(1);
                            encounterSlot[2] = EncounterSlot::hSlot(test[2].nextUShort(100), area.getEncounter());
                            valid[2] = filter.compareEncounterSlot(encounterSlot[2]);
                        }
                    }
                    else
                    {
                        encounterSlot[2] = EncounterSlot::hSlot(test[2].nextUShort(100), area.getEncounter());
                        valid[2] = filter.compareEncounterSlot(encounterSlot[2]);
                    }
                }
                break;
            case Lead::MagnetPull:
            case Lead::Static:
                // Not possible to use this lead for fishing so skip the Feebas logic
                if ((nextRNG % 25) == nature)
                {
                    levelRand[0] = safari ? test[0].nextUShort() : nextRNG2;
                    u16 encounterRand = test[0].nextUShort();
                    if (test[0].nextUShort(2) == 0 && !modifiedSlots.empty())
                    {
                        encounterSlot[0] = modifiedSlots[encounterRand % modifiedSlots.count];
                    }
                    else
                    {
                        encounterSlot[0] = EncounterSlot::hSlot(encounterRand % 100, area.getEncounter());
                    }
                    valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                }
                break;
            case Lead::Pressure:
                if ((nextRNG % 25) == nature)
                {
                    force = ((safari ? test[0].nextUShort() : nextRNG2) & 1) == 0;
                    levelRand[0] = test[0].nextUShort();

                    if (feebas)
                    {
                        if (feebasTile)
                        {
                            if (test[0].nextUShort(100) < 50)
                            {
                                encounterSlot[0] = feebasSlots[toInt(area.getEncounter() - Encounter::OldRod)];
                                valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                            }

                            u8 rand = test[1].nextUShort(100);
                            if (test[1].nextUShort(100) >= 50)
                            {
                                encounterSlot[1] = EncounterSlot::hSlot(rand, area.getEncounter());
                                valid[1] = filter.compareEncounterSlot(encounterSlot[1]);
                            }
                        }
                        else
                        {
                            test[0].advance(1);
                            encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                            valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                        }
                    }
                    else
                    {
                        encounterSlot[0] = EncounterSlot::hSlot(test[0].nextUShort(100), area.getEncounter());
                        valid[0] = filter.compareEncounterSlot(encounterSlot[0]);
                    }
                }
                break;
            default:
                break;
            }

            for (int i = 0; i < 4; i++)
            {
                if (valid[i] && (rate == 0 || (test[i].nextUShort(2880) < rate)))
                {
                    const Slot &slot = area.getPokemon(encounterSlot[i]);
                    const PersonalInfo *info = slot.getInfo();
                    if ((!cuteCharmFlag || cuteCharmGender(info, pid, lead))
                        && (slot.getSpecie() != 201 || unownLetter(pid) == slot.getForm()))
                    {
                        u8 level;
                        if (lead == Lead::Pressure)
                        {
                            level = area.calculateLevel(encounterSlot[i], levelRand[i >> 1], force);
                        }
                        else
                        {
                            level = area.EncounterArea::calculateLevel(encounterSlot[i], levelRand[i >> 1]);
                        }

                        WildSearcherState state(test[i].next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), level, nature,
                                                Utilities::getShiny<true>(pid, tsv), encounterSlot[i], 0, slot.getSpecie(), slot.getForm(),
                                                info);
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
