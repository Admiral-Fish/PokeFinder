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

#include "Profile4.hpp"

Profile4::Profile4(const QString &profileName, Game version, u16 tid, u16 sid, Game dual, int radio, Language language, bool radar, bool swarm)
    : Profile(profileName, version, tid, sid, language)
{
    this->dual = dual;
    this->radio = radio;
    this->radar = radar;
    this->swarm = swarm;
}

Profile4::Profile4(QJsonObject data)
    : Profile(data["name"].toString(), static_cast<Game>(data["version"].toInt()), data["tid"].toInt(), data["sid"].toInt(), static_cast<Language>(data["language"].toInt()))
{
    dual = static_cast<Game>(data["dual"].toInt());
    radio = data["radio"].toInt();
    radar = data["radar"].toBool();
    swarm = data["swarm"].toBool();
}

Profile4::Profile4()
{
    version = Game::Diamond;
    dual = Game::Blank;
    radio = 0;
    radar = false;
    swarm = false;
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

QJsonObject Profile4::getJson() const
{
    QJsonObject profile;
    profile["name"] = profileName;
    profile["version"] = version;
    profile["language"] = language;
    profile["tid"] = tid;
    profile["sid"] = sid;
    profile["dual"] = dual;
    profile["radio"] = radio;
    profile["radar"] = radar;
    profile["swarm"] = swarm;
    return profile;
}

QVector<Profile4> Profile4::loadProfileList()
{
    QVector<Profile4> profileList;

    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonDocument profiles(QJsonDocument::fromJson(file.readAll()));
        QJsonArray gen4 = profiles["gen4"].toArray();

        for (const auto &&i : gen4)
        {
            profileList.append(Profile4(i.toObject()));
        }
        file.close();
    }

    return profileList;
}

void Profile4::saveProfile() const
{
    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QJsonObject profiles(QJsonDocument::fromJson(file.readAll()).object());
        QJsonArray gen4 = profiles["gen4"].toArray();

        gen4.append(getJson());
        profiles["gen4"] = gen4;

        file.resize(0);
        file.write(QJsonDocument(profiles).toJson());
        file.close();
    }
}

void Profile4::deleteProfile() const
{
    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadWrite | QFile::Text))
    {
        QJsonObject profiles(QJsonDocument::fromJson(file.readAll()).object());
        QJsonArray gen4 = profiles["gen4"].toArray();

        for (int i = 0; i < gen4.size(); i++)
        {
            Profile4 profile(gen4[i].toObject());

            if (profile == *this)
            {
                gen4.removeAt(i);
                profiles["gen4"] = gen4;

                file.resize(0);
                file.write(QJsonDocument(profiles).toJson());
                break;
            }
        }

        file.close();
    }
}

void Profile4::updateProfile(const Profile4 &original) const
{
    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadWrite | QFile::Text))
    {
        QJsonObject profiles(QJsonDocument::fromJson(file.readAll()).object());
        QJsonArray gen4 = profiles["gen4"].toArray();

        for (auto &&i : gen4)
        {
            Profile4 profile(i.toObject());

            if (original == profile)
            {
                i = getJson();
                profiles["gen4"] = gen4;

                file.resize(0);
                file.write(QJsonDocument(profiles).toJson());
                break;
            }
        }
        file.close();
    }
}

bool operator==(const Profile4 &left, const Profile4 &right)
{
    return left.profileName == right.profileName && left.version == right.version && left.language == right.language &&
           left.tid == right.tid && left.sid == right.sid && left.dual == right.dual && left.radio == right.radio &&
           left.radar == right.radar && left.swarm == right.swarm;
}
