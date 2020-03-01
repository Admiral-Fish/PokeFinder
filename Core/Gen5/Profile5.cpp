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

#include "Profile5.hpp"
#include <Core/Enum/Game.hpp>
#include <QStringList>
#include <QTranslator>

Profile5::Profile5() :
    mac(0x9BF123456),
    keypresses({ true, true, true, true }),
    vcount(0x2f),
    gxstat(6),
    vframe(5),
    skipLR(false),
    timer0Min(0x621),
    timer0Max(0x621),
    softReset(false),
    dsType(DSType::DSOriginal),
    language(Language::English)
{
    // Default parameters for White on desmume
    version = Game::White;
}

Profile5::Profile5(const QString &name, Game version, u16 tid, u16 sid, u64 mac, const QVector<bool> &keypresses, u8 vcount, u8 gxstat,
                   u8 vframe, bool skipLR, u16 timer0Min, u16 timer0Max, bool softReset, DSType dsType, Language language) :
    Profile(name, version, tid, sid),
    mac(mac),
    keypresses(keypresses),
    vcount(vcount),
    gxstat(gxstat),
    vframe(vframe),
    skipLR(skipLR),
    timer0Min(timer0Min),
    timer0Max(timer0Max),
    softReset(softReset),
    dsType(dsType),
    language(language)
{
}

u64 Profile5::getMac() const
{
    return mac;
}

QVector<bool> Profile5::getKeypresses() const
{
    return keypresses;
}

QString Profile5::getKeypressesString() const
{
    QStringList strings;
    for (u8 i = 0; i < keypresses.size(); i++)
    {
        if (i == 0 && keypresses.at(i))
        {
            strings.append(QObject::tr("None"));
        }
        else if (keypresses.at(i))
        {
            strings.append(QString::number(i));
        }
    }
    return strings.join(", ");
}

u8 Profile5::getVCount() const
{
    return vcount;
}

u8 Profile5::getGxStat() const
{
    return gxstat;
}

u8 Profile5::getVFrame() const
{
    return vframe;
}

bool Profile5::getSkipLR() const
{
    return skipLR;
}

u16 Profile5::getTimer0Min() const
{
    return timer0Min;
}

u16 Profile5::getTimer0Max() const
{
    return timer0Max;
}

bool Profile5::getSoftReset() const
{
    return softReset;
}

DSType Profile5::getDSType() const
{
    return dsType;
}

QString Profile5::getDSTypeString() const
{
    switch (dsType)
    {
    case DSType::DSOriginal:
        return QObject::tr("DS Lite");
    case DSType::DSi:
        return QObject::tr("DSi");
    case DSType::DS3:
        return QObject::tr("3DS");
    }
    return "-";
}

Language Profile5::getLanguage() const
{
    return language;
}

QString Profile5::getLanguageString() const
{
    switch (language)
    {
    case Language::English:
        return "ENG";
    case Language::French:
        return "FRE";
    case Language::German:
        return "DEU";
    case Language::Italian:
        return "ITA";
    case Language::Japanese:
        return "JPN";
    case Language::Korean:
        return "KOR";
    case Language::Spanish:
        return "SPA";
    }
    return "-";
}

bool operator==(const Profile5 &left, const Profile5 &right)
{
    return left.getName() == right.getName() && left.getVersion() == right.getVersion() && left.getTID() == right.getTID()
        && left.getSID() == right.getSID() && left.getMac() == right.getMac() && left.getKeypresses() == right.getKeypresses()
        && left.getVCount() == right.getVCount() && left.getGxStat() == right.getGxStat() && left.getVFrame() == right.getVFrame()
        && left.getSkipLR() == right.getSkipLR() && left.getTimer0Min() == right.getTimer0Min()
        && left.getTimer0Max() == right.getTimer0Max() && left.getSoftReset() == right.getSoftReset()
        && left.getDSType() == right.getDSType() && left.getLanguage() == right.getLanguage();
}

bool operator!=(const Profile5 &left, const Profile5 &right)
{
    return !(left == right);
}
