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

#include "ProfileLoader5.hpp"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace ProfileLoader5
{
    namespace
    {
        Profile5 getProfile(const QJsonObject &object)
        {
            QString name = object["name"].toString();
            Game version = static_cast<Game>(object["version"].toInt());
            u16 tid = static_cast<u16>(object["tid"].toInt());
            u16 sid = static_cast<u16>(object["sid"].toInt());
            u64 mac = object["mac"].toString().toULongLong(nullptr, 16);
            std::vector<bool> keypresses(4);
            for (int i = 0; i < 4; i++)
            {
                keypresses[i] = object["keypresses"].toArray()[i].toBool();
            }
            u8 vcount = static_cast<u8>(object["vcount"].toInt());
            u8 gxstat = static_cast<u8>(object["gxstat"].toInt());
            u8 vframe = static_cast<u8>(object["vframe"].toInt());
            bool skipLR = object["skipLR"].toBool();
            u16 timer0Min = static_cast<u16>(object["timer0Min"].toInt());
            u16 timer0Max = static_cast<u16>(object["timer0Max"].toInt());
            bool softReset = object["softReset"].toBool();
            bool memoryLink = object["memoryLink"].toBool();
            bool shinyCharm = object["shinyCharm"].toBool();
            DSType dsType = static_cast<DSType>(object["dsType"].toInt());
            Language language = static_cast<Language>(object["language"].toInt());
            return Profile5(name, version, tid, sid, mac, keypresses, vcount, gxstat, vframe, skipLR, timer0Min, timer0Max, softReset,
                            memoryLink, shinyCharm, dsType, language);
        }

        QJsonObject getJson(const Profile5 &profile)
        {
            QJsonObject data;
            data["name"] = profile.getName();
            data["version"] = static_cast<int>(profile.getVersion());
            data["tid"] = profile.getTID();
            data["sid"] = profile.getSID();
            data["mac"] = QString::number(profile.getMac(), 16);
            QJsonArray keys;
            for (bool key : profile.getKeypresses())
            {
                keys.append(key);
            }
            data["keypresses"] = keys;
            data["vcount"] = profile.getVCount();
            data["gxstat"] = profile.getGxStat();
            data["vframe"] = profile.getVFrame();
            data["skipLR"] = profile.getSkipLR();
            data["timer0Min"] = profile.getTimer0Min();
            data["timer0Max"] = profile.getTimer0Max();
            data["softReset"] = profile.getSoftReset();
            data["memoryLink"] = profile.getMemoryLink();
            data["shinyCharm"] = profile.getShinyCharm();
            data["dsType"] = static_cast<int>(profile.getDSType());
            data["language"] = static_cast<int>(profile.getLanguage());
            return data;
        }
    }

    std::vector<Profile5> getProfiles()
    {
        std::vector<Profile5> profileList;

        QSettings setting;
        QFile f(setting.value("settings/profiles").toString());
        if (f.open(QIODevice::ReadOnly))
        {
            QByteArray data = f.readAll();

            QJsonObject profiles(QJsonDocument::fromJson(data).object());
            QJsonArray gen5 = profiles["gen5"].toArray();

            std::transform(gen5.begin(), gen5.end(), std::back_inserter(profileList),
                           [](const QJsonValueRef &val) { return getProfile(val.toObject()); });
        }

        return profileList;
    }

    void addProfile(const Profile5 &profile)
    {
        QSettings setting;
        QFile f(setting.value("settings/profiles").toString());
        if (f.open(QIODevice::ReadWrite))
        {
            QByteArray data = f.readAll();

            QJsonObject profiles(QJsonDocument::fromJson(data).object());
            QJsonArray gen5 = profiles["gen5"].toArray();

            gen5.append(getJson(profile));
            profiles["gen5"] = gen5;

            f.write(QJsonDocument(profiles).toJson());
        }
    }

    void removeProfile(const Profile5 &remove)
    {
        QSettings setting;
        QFile f(setting.value("settings/profiles").toString());
        if (f.open(QIODevice::ReadWrite))
        {
            QByteArray data = f.readAll();

            QJsonObject profiles(QJsonDocument::fromJson(data).object());
            QJsonArray gen5 = profiles["gen5"].toArray();

            for (int i = 0; i < gen5.size(); i++)
            {
                Profile5 profile = getProfile(gen5[i].toObject());

                if (profile == remove)
                {
                    gen5.removeAt(i);
                    profiles["gen5"] = gen5;

                    f.write(QJsonDocument(profiles).toJson());
                    break;
                }
            }
        }
    }

    void updateProfile(const Profile5 &update, const Profile5 &original)
    {
        QSettings setting;
        QFile f(setting.value("settings/profiles").toString());
        if (f.open(QIODevice::ReadWrite))
        {
            QByteArray data = f.readAll();

            QJsonObject profiles(QJsonDocument::fromJson(data).object());
            QJsonArray gen5 = profiles["gen5"].toArray();

            for (auto i = 0; i < gen5.size(); i++)
            {
                Profile5 profile = getProfile(gen5[i].toObject());

                if (original == profile && original != update)
                {
                    gen5[i] = getJson(update);
                    profiles["gen5"] = gen5;

                    f.write(QJsonDocument(profiles).toJson());
                    break;
                }
            }
        }
    }
}
