/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <QSet>
#include <QtMath>
#include "IVChecker.hpp"
#include <Core/Util/Nature.hpp>

namespace PokeFinderCore
{

    QVector<QVector<u8> > IVChecker::calculateIVs(Pokemon pokemon, const QVector<u16> &stats, u8 level, u8 nature, Characteristic characteristic, int hiddenPower) const
    {
        QVector<bool> valid(6, false);
        QVector<u8> minIVs(6, 31);
        QVector<u8> maxIVs(6, 0);
        QVector<u16> baseStats = pokemon.getBaseStats();

        // HP
        for (u8 iv = 0; iv < 32; iv++)
        {
            double hp = (((2 * baseStats.at(0) + iv) * level) / 100.0) + level + 10;

            if (static_cast<u16>(hp) == stats.at(0))
            {
                valid[0] = true;

                if (iv >= maxIVs.at(0))
                {
                    maxIVs[0] = iv;
                }
                if (iv <= minIVs.at(0))
                {
                    minIVs[0] = iv;
                }
            }
        }

        // Atk, Def, SpA, SpD, Spe
        for (int i = 1; i < 6; i++)
        {
            for (u8 iv = 0; iv < 32; iv++)
            {
                double stat = qFloor((((2 * baseStats.at(i) + iv) * level) / 100.0) + 5) * Nature::getNatureModifier(nature).at(i);

                if (static_cast<u16>(stat) == stats.at(i))
                {
                    valid[i] = true;

                    if (iv >= maxIVs.at(i))
                    {
                        maxIVs[i] = iv;
                    }
                    if (iv <= minIVs.at(i))
                    {
                        minIVs[i] = iv;
                    }
                }
            }
        }

        // Handle stat affected by characteristic
        QVector<QVector<u8>> possible(6);
        u8 characteristicHigh = 31;
        if (characteristic.getActive())
        {
            if (valid.at(characteristic.getStat()))
            {
                //  Set this to zero so we can begin to keep track
                characteristicHigh = 0;

                //  If this is not null we need to iterate through the ranges
                //  of the IV that is referenced and cull out those that are
                //  not possible.
                for (u8 i = minIVs.at(characteristic.getStat()); i <= maxIVs.at(characteristic.getStat()); i++)
                {
                    if ((i % 5) == characteristic.getResult())
                    {
                        possible[characteristic.getStat()].append(i);
                        characteristicHigh = i;
                    }
                }
            }
        }

        // Handle rest of stats
        for (u8 i = 0; i < 6; i++)
        {
            if (valid.at(i))
            {
                //  Make sure we dont make any changes to the characteristic stat
                if (!characteristic.getActive() || characteristic.getStat() != i)
                {
                    //
                    for (u8 j = minIVs.at(i); j <= maxIVs.at(i); j++)
                    {
                        if (j <= characteristicHigh)
                        {
                            possible[i].append(j);
                        }
                    }
                }
            }
        }

        if (hiddenPower != -1)
        {
            QVector<QSet<u8>> final(6);
            for (const u8 hp : possible.at(0))
            {
                for (const u8 atk : possible.at(1))
                {
                    for (const u8 def : possible.at(2))
                    {
                        for (const u8 spa : possible.at(3))
                        {
                            for (const u8 spd : possible.at(4))
                            {
                                for (const u8 spe : possible.at(5))
                                {
                                    u8 hpType = ((((hp & 1) + 2 * (atk & 1) + 4 * (def & 1) + 8 * (spe & 1) + 16 * (spa & 1) + 32 * (spd & 1)) * 15) / 63);
                                    if (hpType == hiddenPower)
                                    {
                                        final[0].insert(hp);
                                        final[1].insert(atk);
                                        final[2].insert(def);
                                        final[3].insert(spa);
                                        final[4].insert(spd);
                                        final[5].insert(spe);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (u8 i = 0; i < 6; i++)
            {
                possible[i].clear();
                for (const u8 stat : final.at(i))
                {
                    possible[i].append(stat);
                }
            }
        }

        return possible;
    }

}
