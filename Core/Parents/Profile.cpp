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

#include "Profile.hpp"
#include <Core/Enum/Game.hpp>
#include <QTranslator>

Profile::Profile() : name("None"), version(Game::Emerald), tid(12345), sid(54321)
{
}

Profile::Profile(const QString &name, Game version, u16 tid, u16 sid) : name(name), version(version), tid(tid), sid(sid)
{
}

QString Profile::getVersionString() const
{
    switch (version)
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
    case Game::Gales:
        return QObject::tr("Gales");
    case Game::Colosseum:
        return QObject::tr("Colosseum");
    case Game::Diamond:
        return QObject::tr("Diamond");
    case Game::Pearl:
        return QObject::tr("Pearl");
    case Game::Platinum:
        return QObject::tr("Platinum");
    case Game::HeartGold:
        return QObject::tr("Heart Gold");
    case Game::SoulSilver:
        return QObject::tr("Soul Silver");
    case Game::Black:
        return QObject::tr("Black");
    case Game::White:
        return QObject::tr("White");
    case Game::Black2:
        return QObject::tr("Black 2");
    case Game::White2:
        return QObject::tr("White 2");
    default:
        return "-";
    }
}

Game Profile::getVersion() const
{
    return version;
}

QString Profile::getName() const
{
    return name;
}

u16 Profile::getTID() const
{
    return tid;
}

u16 Profile::getSID() const
{
    return sid;
}
