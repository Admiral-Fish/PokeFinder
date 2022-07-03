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

#include "ShadowLock.hpp"
#include <Core/Gen3/ShadowTemplate.hpp>
#include <Core/RNG/LCRNG.hpp>

/* Each non-shadow before a shadow has to match
 * a specific gender/nature and these preset
 * values directly impact what spreads are available */

static inline bool isShiny(u32 pid, u16 tsv)
{
    return ((pid >> 16) ^ (pid & 0xffff) ^ tsv) < 8;
}

static inline u32 getPIDBackward(XDRNGR &rng)
{
    u32 pid = rng.nextUShort();
    pid |= rng.next() & 0xffff0000;
    return pid;
}

static inline u32 getPIDForward(XDRNG &rng)
{
    u32 pid = rng.next() & 0xffff0000;
    pid |= rng.nextUShort();
    return pid;
}

namespace ShadowLock
{
    bool coloShadow(u32 seed, const ShadowTemplate *shadowTemplate)
    {
        XDRNGR backward(seed);
        backward.advance(1);

        // Grab PID from first non-shadow going backwards
        // If it doesn't match spread fails
        u32 pidOriginal = getPIDBackward(backward);
        s8 index = shadowTemplate->getCount() - 1;
        if (!shadowTemplate->getLock(index).compare(pidOriginal))
        {
            return false;
        }

        u32 pid;
        for (index--; index >= 0; index--)
        {
            const LockInfo &lock = shadowTemplate->getLock(index);
            backward.advance(5);
            if (!lock.getIgnore())
            {
                do
                {
                    pid = getPIDBackward(backward);
                } while (!lock.compare(pid));
            }
        }

        XDRNG forward(backward);
        forward.advance(1);

        for (index = 1; index < shadowTemplate->getCount(); index++)
        {
            const LockInfo &lock = shadowTemplate->getLock(index);
            forward.advance(5);
            if (!lock.getIgnore())
            {
                do
                {
                    pid = getPIDForward(forward);
                } while (!lock.compare(pid));
            }
        }

        // Check if we end on the same PID as first non-shadow going backwards
        return pidOriginal == pid;
    }

    bool ereader(u32 seed, u32 readerPID, const ShadowTemplate *shadowTemplate)
    {
        // Check if PID is even valid for E-Reader
        // E-Reader have set nature/gender
        s8 index = shadowTemplate->getCount() - 1;
        if (!shadowTemplate->getLock(index).compare(readerPID))
        {
            return false;
        }

        XDRNGR backward(seed);
        backward.advance(1);

        u32 pid;
        for (index--; index >= 0; index--)
        {
            const LockInfo &lock = shadowTemplate->getLock(index);
            if (index != shadowTemplate->getCount() - 2)
            {
                backward.advance(5);
            }

            do
            {
                pid = getPIDBackward(backward);
            } while (!lock.compare(pid));
        }

        XDRNG forward(backward);
        forward.advance(1);

        for (index = 1; index < shadowTemplate->getCount(); index++)
        {
            const LockInfo &lock = shadowTemplate->getLock(index);
            forward.advance(5);
            do
            {
                pid = getPIDForward(forward);
            } while (!lock.compare(pid));
        }

        // Checks if PID matches original
        return pid == readerPID;
    }

    bool firstShadowNormal(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate)
    {
        XDRNGR backward(seed);
        backward.advance(1);

        // Grab PID from first non-shadow going backwards
        // If it doesn't match spread fails
        u32 pidOriginal = getPIDBackward(backward);
        s8 index = shadowTemplate->getCount() - 1;
        if (!shadowTemplate->getLock(index).compare(pidOriginal))
        {
            return false;
        }

        u32 pid;
        for (index--; index >= 0; index--)
        {
            const LockInfo &lock = shadowTemplate->getLock(index);
            backward.advance(5);
            if (!lock.getIgnore())
            {
                do
                {
                    pid = getPIDBackward(backward);
                } while (!lock.compare(pid));
            }
        }

        XDRNG forward(backward);
        forward.advance(1);

        for (index = 1; index < shadowTemplate->getCount(); index++)
        {
            const LockInfo &lock = shadowTemplate->getLock(index);
            forward.advance(5);
            if (!lock.getIgnore())
            {
                do
                {
                    pid = getPIDForward(forward);
                } while (!lock.compare(pid) || isShiny(pid, tsv));
            }
        }

        // Check if we end on the same PID as first non-shadow going backwards
        return pidOriginal == pid;
    }

    bool firstShadowSet(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate)
    {
        XDRNGR backward(seed);
        backward.advance(6);

        // Grab PID from first non-shadow going backwards
        // If it doesn't match spread fails
        u32 pidOriginal = getPIDBackward(backward);
        s8 index = shadowTemplate->getCount() - 2;
        if (!shadowTemplate->getLock(index).compare(pidOriginal))
        {
            return false;
        }

        u32 pid;
        for (index--; index >= 0; index--)
        {
            const LockInfo &lock = shadowTemplate->getLock(index);
            backward.advance(5);
            if (!lock.getIgnore())
            {
                do
                {
                    pid = getPIDBackward(backward);
                } while (!lock.compare(pid));
            }
        }

        XDRNG forward(backward);
        forward.advance(1);

        for (index = 1; index < shadowTemplate->getCount(); index++)
        {
            const LockInfo &lock = shadowTemplate->getLock(index);
            forward.advance(5);
            if (!lock.getIgnore())
            {
                do
                {
                    pid = getPIDForward(forward);
                } while (!lock.compare(pid) || isShiny(pid, tsv));
            }
        }

        // Check if we end on the same PID as first non-shadow going backwards
        return pidOriginal == pid;
    }

    bool firstShadowUnset(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate)
    {
        XDRNGR backward(seed);
        backward.advance(3);

        // Shiny lock test for first shadow
        XDRNGR test(backward);
        u16 shadowPSV = test.nextUShort() ^ test.nextUShort();
        while ((shadowPSV ^ tsv) < 8)
        {
            backward.setSeed(test.getSeed());
            shadowPSV = test.nextUShort() ^ test.nextUShort();
        }

        backward.advance(5);

        // Grab PID from first non-shadow going backwards
        // If it doesn't match spread fails
        u32 pidOriginal = getPIDBackward(backward);
        s8 index = shadowTemplate->getCount() - 2;
        if (!shadowTemplate->getLock(index).compare(pidOriginal))
        {
            return false;
        }

        u32 pid;
        for (index--; index >= 0; index--)
        {
            const LockInfo &lock = shadowTemplate->getLock(index);
            backward.advance(5);
            if (!lock.getIgnore())
            {
                do
                {
                    pid = getPIDBackward(backward);
                } while (!lock.compare(pid));
            }
        }

        XDRNG forward(backward);
        forward.advance(1);

        for (index = 1; index < shadowTemplate->getCount(); index++)
        {
            const LockInfo &lock = shadowTemplate->getLock(index);
            forward.advance(5);
            if (!lock.getIgnore())
            {
                do
                {
                    pid = getPIDForward(forward);
                } while (!lock.compare(pid) || isShiny(pid, tsv));
            }
        }

        // Check if we end on the same PID as first non-shadow going backwards
        return pidOriginal == pid;
    }

    bool salamenceSet(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate)
    {
        XDRNGR backward(seed);
        backward.advance(6);

        // Build PID of non-shadow
        u32 pid = getPIDBackward(backward);

        return shadowTemplate->getLock(0).compare(pid) && !isShiny(pid, tsv);
    }

    bool salamenceUnset(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate)
    {
        XDRNGR backward(seed);
        backward.advance(3);

        // Shiny lock test for first shadow
        XDRNGR test(backward);
        u16 shadowPSV = test.nextUShort() ^ test.nextUShort();
        while ((shadowPSV ^ tsv) < 8)
        {
            backward.setSeed(test.getSeed());
            shadowPSV = test.nextUShort() ^ test.nextUShort();
        }

        backward.advance(5);

        // Build PID of non-shadow
        u32 pid = getPIDBackward(backward);

        // Backwards nature lock check
        return shadowTemplate->getLock(0).compare(pid) && !isShiny(pid, tsv);
    }

    bool singleNL(u32 seed, u16 tsv, const ShadowTemplate *shadowTemplate)
    {
        XDRNGR backward(seed);
        backward.advance(1);

        // Build PID of non-shadow
        u32 pid = getPIDBackward(backward);

        // Backwards nature lock check
        return shadowTemplate->getLock(0).compare(pid) && !isShiny(pid, tsv);
    }
}
