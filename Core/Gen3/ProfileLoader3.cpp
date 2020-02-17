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

#include "ProfileLoader3.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace ProfileLoader3
{
    namespace
    {
        Profile3 getProfile(const QJsonObject &object)
        {
            QString name = object["name"].toString();
            Game version = static_cast<Game>(object["version"].toInt());
            u16 tid = static_cast<u16>(object["tid"].toInt());
            u16 sid = static_cast<u16>(object["sid"].toInt());
            bool deadBattery = object["deadBattery"].toBool();

            return Profile3(name, version, tid, sid, deadBattery);
        }

        QJsonObject getJson(const Profile3 &profile)
        {
            QJsonObject data;
            data["name"] = profile.getName();
            data["version"] = static_cast<int>(profile.getVersion());
            data["tid"] = profile.getTID();
            data["sid"] = profile.getSID();
            data["battery"] = profile.getDeadBattery();
            return data;
        }
    }

    QVector<Profile3> getProfiles()
    {
        QVector<Profile3> profileList;

        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        for (const auto i : gen3)
        {
            profileList.append(getProfile(i.toObject()));
        }

        return profileList;
    }

    void addProfile(const Profile3 &profile)
    {
        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        gen3.append(getJson(profile));
        profiles["gen3"] = gen3;

        setting.setValue("profiles", QJsonDocument(profiles).toJson());
    }

    void removeProfile(const Profile3 &remove)
    {
        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        for (int i = 0; i < gen3.size(); i++)
        {
            Profile3 profile = getProfile(gen3[i].toObject());

            if (profile == remove)
            {
                gen3.removeAt(i);
                profiles["gen3"] = gen3;

                setting.setValue("profiles", QJsonDocument(profiles).toJson());
                break;
            }
        }
    }

    void updateProfile(const Profile3 &update, const Profile3 &original)
    {
        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        for (int i = 0; i < gen3.size(); i++)
        {
            Profile3 profile = getProfile(gen3[i].toObject());

            if (original == profile && original != update)
            {
                gen3[i] = getJson(update);
                profiles["gen3"] = gen3;

                setting.setValue("profiles", QJsonDocument(profiles).toJson());
                break;
            }
        }
    }
}
