/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Profile4.hpp"
#include <QTranslator>

Profile4::Profile4() : dual(Game::Blank), radio(0), radar(false), swarm(false)
{
    version = Game::Diamond;
}

Profile4::Profile4(const QString &profileName, Game version, u16 tid, u16 sid, Game dual, int radio, bool radar, bool swarm) :
    Profile(profileName, version, tid, sid), dual(dual), radio(radio), radar(radar), swarm(swarm)
{
}

QString Profile4::getDualSlotString() const
{
    switch (dual)
    {
    case Game::Ruby:
        return QObject::tr("Ruby");
    case Game::Sapphire:
        return QObject::tr("Sapphire");
    case Game::FireRed:
        return QObject::tr("Fire Red");
    case Game::LeafGreen:
        return QObject::tr("Leaf Green");
    case Game::Emerald:
        return QObject::tr("Emerald");
    default:
        return QObject::tr("None");
    }
}

Game Profile4::getDualSlot() const
{
    return dual;
}

QString Profile4::getRadioString() const
{
    switch (radio)
    {
    case 1:
        return QObject::tr("Hoenn Sound");
    case 2:
        return QObject::tr("Sinnoh Sound");
    default:
        return QObject::tr("None");
    }
}

int Profile4::getRadio() const
{
    return radio;
}

bool Profile4::getRadar() const
{
    return radar;
}

bool Profile4::getSwarm() const
{
    return swarm;
}

bool operator==(const Profile4 &left, const Profile4 &right)
{
    return left.getName() == right.getName() && left.getVersion() == right.getVersion() && left.getTID() == right.getTID()
        && left.getSID() == right.getSID() && left.getDualSlot() == right.getDualSlot() && left.getRadio() == right.getRadio()
        && left.getRadar() == right.getRadar() && left.getSwarm() == right.getSwarm();
}

bool operator!=(const Profile4 &left, const Profile4 &right)
{
    return !(left == right);
}
