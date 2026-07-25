/*
 * This file is part of PokeFinder
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

#include "LeadDisplay.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/Util/Translator.hpp>
#include <QCoreApplication>
#include <QStringList>

namespace
{
    bool has(u64 mask, Lead lead)
    {
        return (mask & getLeadFlag(lead)) != 0;
    }

    QString trLead(const char *text)
    {
        return QCoreApplication::translate("LeadDisplay", text);
    }
}

namespace LeadDisplay
{
    QString getLeadName(u64 leadMask, bool showSynchronizeNature)
    {
        if (leadMask == 0)
        {
            return trLead("None");
        }

        QStringList leads;
        if (has(leadMask, Lead::None))
        {
            leads.append(trLead("None"));
        }

        constexpr u64 synchronizeMask = (1ULL << 25) - 1;
        u64 synchronize = leadMask & synchronizeMask;
        if (synchronize != 0 && !showSynchronizeNature)
        {
            leads.append(trLead("Synchronize"));
        }
        else if (synchronize == synchronizeMask)
        {
            leads.append(QString("%1: %2").arg(trLead("Synchronize"), trLead("Any Nature")));
        }
        else
        {
            for (u8 i = 0; i < 25; i++)
            {
                if ((synchronize & (1ULL << i)) != 0)
                {
                    leads.append(QString("%1: %2").arg(trLead("Synchronize"), QString::fromStdString(Translator::getNature(i))));
                }
            }
        }

        bool cuteCharmF = has(leadMask, Lead::CuteCharmF);
        bool cuteCharmM = has(leadMask, Lead::CuteCharmM);
        if (cuteCharmF && cuteCharmM)
        {
            leads.append(trLead("Cute Charm: \u2642 or \u2640 Lead"));
        }
        else if (cuteCharmF)
        {
            leads.append(trLead("Cute Charm: \u2640 Lead"));
        }
        else if (cuteCharmM)
        {
            leads.append(trLead("Cute Charm: \u2642 Lead"));
        }

        if (has(leadMask, Lead::MagnetPull))
        {
            leads.append(trLead("Magnet Pull"));
        }
        if (has(leadMask, Lead::Static))
        {
            leads.append(trLead("Static"));
        }
        if (has(leadMask, Lead::Harvest))
        {
            leads.append(trLead("Harvest"));
        }
        if (has(leadMask, Lead::FlashFire))
        {
            leads.append(trLead("Flash Fire"));
        }
        if (has(leadMask, Lead::StormDrain))
        {
            leads.append(trLead("Storm Drain"));
        }
        if (has(leadMask, Lead::Pressure))
        {
            leads.append(trLead("Level Modifier"));
        }
        if (has(leadMask, Lead::CompoundEyes))
        {
            leads.append(trLead("Compound Eyes"));
        }
        if (has(leadMask, Lead::SuctionCups) || has(leadMask, Lead::ArenaTrap))
        {
            leads.append(trLead("Encounter Modifier"));
        }

        return leads.join(" / ");
    }
}
