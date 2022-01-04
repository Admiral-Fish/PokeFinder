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

#include "WildSearcher3.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

WildSearcher3::WildSearcher3(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter) :
    WildSearcher(tid, sid, genderRatio, method, filter), cache(method), searching(false), progress(0)
{
}

void WildSearcher3::setEncounterArea(const EncounterArea3 &encounterArea)
{
    this->encounterArea = encounterArea;
}

void WildSearcher3::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max)
{
    searching = true;

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

                            auto states = search(hp, atk, def, spa, spd, spe);

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

void WildSearcher3::cancelSearch()
{
    searching = false;
}

std::vector<WildState> WildSearcher3::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data = std::move(results);
    return data;
}

int WildSearcher3::getProgress() const
{
    return progress;
}

std::vector<WildState> WildSearcher3::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    std::vector<WildState> states;

    WildState state;
    state.setIVs(hp, atk, def, spa, spd, spe);
    state.calculateHiddenPower();
    if (!filter.compareHiddenPower(state))
    {
        return states;
    }

    auto seeds = cache.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const u32 val : seeds)
    {
        // Setup normal state
        PokeRNGR rng(val);
        rng.advance(method == Method::MethodH2);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        state.setPID(high, low);
        state.setAbility(low & 1);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny<8>(tsv, high ^ low);

        u32 seed = rng.next();

        // Use for loop to check both normal and sister spread
        for (const bool flag : { false, true })
        {
            if (flag)
            {
                state.setPID(state.getPID() ^ 0x80008000);
                state.setNature(state.getPID() % 25);
                seed ^= 0x80000000;
            }

            if (!filter.comparePID(state))
            {
                continue;
            }

            PokeRNGR testRNG(seed);
            u32 testPID;
            u16 nextRNG = seed >> 16;
            u16 nextRNG2 = testRNG.nextUShort();

            do
            {
                switch (lead)
                {
                case Lead::None:
                    if ((nextRNG % 25) == state.getNature())
                    {
                        state.setLead(Lead::None);
                        PokeRNGR temp(testRNG.getSeed());
                        u16 slot = temp.nextUShort();
                        state.setSeed(temp.advance(2));
                        state.setEncounterSlot(EncounterSlot::hSlot(slot, encounter));
                        if (filter.compareEncounterSlot(state))
                        {
                            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), testRNG.getSeed() >> 16));
                            states.emplace_back(state);
                        }
                    }
                    break;
                case Lead::Synchronize:
                    // Successful synch
                    if ((nextRNG & 1) == 0)
                    {
                        state.setLead(Lead::Synchronize);
                        PokeRNGR temp(testRNG.getSeed());
                        u16 slot = temp.nextUShort();
                        state.setSeed(temp.advance(2));
                        state.setEncounterSlot(EncounterSlot::hSlot(slot, encounter));
                        if (filter.compareEncounterSlot(state))
                        {
                            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), testRNG.getSeed() >> 16));
                            states.emplace_back(state);
                        }
                    }
                    // Failed synch
                    else if ((nextRNG2 & 1) == 1 && (nextRNG % 25) == state.getNature())
                    {
                        state.setLead(Lead::Synchronize);
                        PokeRNGR temp(testRNG.getSeed());
                        u16 slot = temp.advance(2) >> 16;
                        state.setSeed(temp.advance(2));
                        state.setEncounterSlot(EncounterSlot::hSlot(slot, encounter));
                        if (filter.compareEncounterSlot(state))
                        {
                            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), testRNG.getSeed() >> 16));
                            states.emplace_back(state);
                        }
                    }
                    break;
                case Lead::CuteCharm:
                    if ((nextRNG % 25) == state.getNature() && (nextRNG2 % 3) > 0)
                    {
                        state.setLead(Lead::CuteCharm);
                        PokeRNGR temp(testRNG.getSeed());
                        u16 slot = temp.advance(2) >> 16;
                        state.setSeed(temp.advance(2));
                        state.setEncounterSlot(EncounterSlot::hSlot(slot, encounter));
                        if (filter.compareEncounterSlot(state))
                        {
                            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), testRNG.getSeed() >> 16));
                            states.emplace_back(state);
                        }
                    }
                    break;
                case Lead::Search:
                default:
                    // Normal
                    if ((nextRNG % 25) == state.getNature())
                    {
                        state.setLead(Lead::None);
                        PokeRNGR temp(testRNG.getSeed());
                        u16 slot = temp.nextUShort();
                        state.setSeed(temp.advance(2));
                        state.setEncounterSlot(EncounterSlot::hSlot(slot, encounter));
                        if (filter.compareEncounterSlot(state))
                        {
                            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), testRNG.getSeed() >> 16));
                            states.emplace_back(state);
                        }

                        temp.setSeed(testRNG.getSeed());
                        slot = temp.advance(2) >> 16;
                        state.setSeed(temp.advance(2));
                        state.setEncounterSlot(EncounterSlot::hSlot(slot, encounter));
                        if (filter.compareEncounterSlot(state))
                        {
                            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), testRNG.getSeed() >> 16));

                            // Failed synch
                            if ((nextRNG2 & 1) == 1 && (nextRNG % 25) == state.getNature())
                            {
                                state.setLead(Lead::Synchronize);
                                states.emplace_back(state);
                            }

                            // Cute Charm
                            if ((nextRNG2 % 3) > 0)
                            {
                                state.setLead(Lead::CuteCharm);
                                states.emplace_back(state);
                            }
                        }
                    }
                    // Successful Synch
                    else if ((nextRNG & 1) == 0)
                    {
                        state.setLead(Lead::Synchronize);
                        PokeRNGR temp(testRNG.getSeed());
                        u16 slot = temp.nextUShort();
                        state.setSeed(temp.advance(2));
                        state.setEncounterSlot(EncounterSlot::hSlot(slot, encounter));
                        if (filter.compareEncounterSlot(state))
                        {
                            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), testRNG.getSeed() >> 16));
                            states.emplace_back(state);
                        }
                    }
                    break;
                }

                testPID = (nextRNG << 16) | nextRNG2;
                nextRNG = testRNG.nextUShort();
                nextRNG2 = testRNG.nextUShort();
            } while ((testPID % 25) != state.getNature());
        }
    }

    // RSE rock smash is dependent on origin seed for encounter check
    if (encounter == Encounter::RockSmash)
    {
        u16 rate = encounterArea.getEncounterRate() * 16;

        // 2880 means FRLG which is not dependent on origin seed for encounter check
        if (rate != 2880)
        {
            for (size_t i = 0; i < states.size();)
            {
                u16 check = PokeRNG(states[i].getSeed()).nextUShort();
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
    }

    return states;
}
