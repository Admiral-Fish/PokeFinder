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

#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include "Profile5.hpp"

Profile5::Profile5(const QString &profileName, Game version, u16 tid, u16 sid, u64 mac, const QVector<bool> &keypresses, u8 vcount,
                   u8 gxstat, u8 vframe, bool skipLR, u16 timer0Min, u16 timer0Max, bool softReset, DSType dsType, Language language)
    : Profile(profileName, version, tid, sid, language)
{
    this->mac = mac;
    this->keypresses = keypresses;
    this->vcount = vcount;
    this->gxstat = gxstat;
    this->vframe = vframe;
    this->skipLR = skipLR;
    this->timer0Min = timer0Min;
    this->timer0Max = timer0Max;
    this->softReset = softReset;
    this->dsType = dsType;
}

Profile5::Profile5(QJsonObject data)
    : Profile(data["name"].toString(), static_cast<Game>(data["version"].toInt()), data["tid"].toInt(), data["sid"].toInt(), static_cast<Language>(data["language"].toInt()))
{
    mac = data["mac"].toString().toULongLong(nullptr, 16);
    QJsonArray keys = data["keys"].toArray();
    for (int i = 0; i < 4; i++)
    {
        keypresses.append(keys[i].toBool());
    }
    vcount = data["vcount"].toInt();
    gxstat = data["gxstat"].toInt();
    vframe = data["vframe"].toInt();
    skipLR = data["skipLR"].toBool();
    timer0Min = data["timer0Min"].toInt();
    timer0Max = data["timer0Max"].toInt();
    softReset = data["softReset"].toBool();
    dsType = static_cast<DSType>(data["dsType"].toInt());
}

Profile5::Profile5()
{
    // Default parameters for White on desmume
    version = Game::White;
    mac = 0x9BF123456;
    keypresses = { true, true, true, true };
    vcount = 0x2F;
    gxstat = 6;
    vframe = 5;
    skipLR = false;
    timer0Min = timer0Max = 0x621;
    softReset = false;
    dsType = DSType::DSOriginal;
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
    QVector<bool> keys = getKeypresses();
    for (u8 i = 0; i < keys.size(); i++)
    {
        if (i == 0 && keys.at(i))
        {
            strings.append(QObject::tr("None"));
        }
        else if (keys.at(i))
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
        default:
            return "-";
    }
}

QJsonObject Profile5::getJson() const
{
    QJsonObject profile;
    profile["name"] = profileName;
    profile["version"] = version;
    profile["language"] = language;
    profile["tid"] = tid;
    profile["sid"] = sid;
    profile["mac"] = QString::number(mac, 16);
    QJsonArray keys;
    for (u8 i = 0; i < 4; i++)
    {
        keys.append(keypresses.at(i));
    }
    profile["keys"] = keys;
    profile["vcount"] = vcount;
    profile["gxstat"] = gxstat;
    profile["vframe"] = vframe;
    profile["skipLR"] = skipLR;
    profile["timer0Min"] = timer0Min;
    profile["timer0Max"] = timer0Max;
    profile["softReset"] = softReset;
    profile["dsType"] = dsType;
    return profile;
}

QVector<Profile5> Profile5::loadProfileList()
{
    QVector<Profile5> profileList;

    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonDocument profiles(QJsonDocument::fromJson(file.readAll()));
        QJsonArray gen5 = profiles["gen5"].toArray();

        for (const auto &&i : gen5)
        {
            profileList.append(Profile5(i.toObject()));
        }
        file.close();
    }

    return profileList;
}

void Profile5::saveProfile() const
{
    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QJsonObject profiles(QJsonDocument::fromJson(file.readAll()).object());
        QJsonArray gen5 = profiles["gen5"].toArray();

        gen5.append(getJson());
        profiles["gen5"] = gen5;

        file.resize(0);
        file.write(QJsonDocument(profiles).toJson());
        file.close();
    }
}

void Profile5::deleteProfile() const
{
    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadWrite | QFile::Text))
    {
        QJsonObject profiles(QJsonDocument::fromJson(file.readAll()).object());
        QJsonArray gen5 = profiles["gen5"].toArray();

        for (int i = 0; i < gen5.size(); i++)
        {
            Profile5 profile(gen5[i].toObject());

            if (profile == *this)
            {
                gen5.removeAt(i);
                profiles["gen4"] = gen5;

                file.resize(0);
                file.write(QJsonDocument(profiles).toJson());
                break;
            }
        }

        file.close();
    }
}

void Profile5::updateProfile(const Profile5 &original) const
{
    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadWrite | QFile::Text))
    {
        QJsonObject profiles(QJsonDocument::fromJson(file.readAll()).object());
        QJsonArray gen5 = profiles["gen5"].toArray();

        for (auto &&i : gen5)
        {
            Profile5 profile(i.toObject());

            if (original == profile && original != *this)
            {
                i = getJson();
                profiles["gen5"] = gen5;

                file.resize(0);
                file.write(QJsonDocument(profiles).toJson());
                break;
            }
        }
        file.close();
    }
}

bool operator==(const Profile5 &left, const Profile5 &right)
{
    return left.profileName == right.profileName && left.version == right.version && left.language == right.language &&
           left.tid == right.tid && left.sid == right.sid && left.mac == right.mac && left.keypresses == right.keypresses &&
           left.vcount == right.vcount && left.gxstat == right.gxstat && left.vframe == right.vframe && left.skipLR == right.skipLR &&
           left.timer0Min == right.timer0Min && left.softReset == right.softReset && left.dsType == right.dsType;
}

bool operator!=(const Profile5 &left, const Profile5 &right)
{
    return !(left == right);
}
