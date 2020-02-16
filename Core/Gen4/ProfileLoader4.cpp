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

#include "ProfileLoader4.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace ProfileLoader4
{
    namespace
    {
        Profile4 getProfile(const QJsonObject &object)
        {
            QString name = object["name"].toString();
            Game version = static_cast<Game>(object["version"].toInt());
            u16 tid = static_cast<u16>(object["tid"].toInt());
            u16 sid = static_cast<u16>(object["sid"].toInt());
            Game dual = static_cast<Game>(object["dual"].toInt());
            int radio = object["radio"].toInt();
            bool radar = object["radar"].toBool();
            bool swarm = object["swarm"].toBool();

            return Profile4(name, version, tid, sid, dual, radio, radar, swarm);
        }

        QJsonObject getJson(const Profile4 &profile)
        {
            QJsonObject data;
            data["name"] = profile.getName();
            data["version"] = static_cast<int>(profile.getVersion());
            data["tid"] = profile.getTID();
            data["sid"] = profile.getSID();
            data["dual"] = static_cast<int>(profile.getDualSlot());
            data["radio"] = profile.getRadio();
            data["radar"] = profile.getRadar();
            data["swarm"] = profile.getSwarm();
            return data;
        }
    }

    QVector<Profile4> getProfiles()
    {
        QVector<Profile4> profileList;

        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen4 = profiles[QString("gen4")].toArray();

        for (const auto i : gen4)
        {
            profileList.append(getProfile(i.toObject()));
        }

        return profileList;
    }

    void addProfile(const Profile4 &profile)
    {
        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen4 = profiles["gen4"].toArray();

        gen4.append(getJson(profile));
        profiles["gen4"] = gen4;

        setting.setValue("profiles", QJsonDocument(profiles).toJson());
    }

    void removeProfile(const Profile4 &remove)
    {
        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen4 = profiles["gen4"].toArray();

        for (int i = 0; i < gen4.size(); i++)
        {
            Profile4 profile = getProfile(gen4[i].toObject());

            if (profile == remove)
            {
                gen4.removeAt(i);
                profiles["gen4"] = gen4;

                setting.setValue("profiles", QJsonDocument(profiles).toJson());
                break;
            }
        }
    }

    void updateProfile(const Profile4 &update, const Profile4 &original)
    {
        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen4 = profiles["gen4"].toArray();

        for (auto i = 0; i < gen4.size(); i++)
        {
            Profile4 profile = getProfile(gen4[i].toObject());

            if (original == profile && original != update)
            {
                gen4[i] = getJson(update);
                profiles["gen4"] = gen4;

                setting.setValue("profiles", QJsonDocument(profiles).toJson());
                break;
            }
        }
    }
}
