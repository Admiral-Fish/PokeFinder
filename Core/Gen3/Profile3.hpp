/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef PROFILE3_HPP
#define PROFILE3_HPP

#include <Core/Objects/Profile.hpp>

class Profile3 : public Profile
{

public:
    Profile3(const QString &profileName, Game version, u16 tid, u16 sid, int language = 0, bool deadBattery = false);
    Profile3(QJsonObject data);
    Profile3();
    bool getDeadBattery() const;
    QJsonObject getJson() const;
    static QVector<Profile3> loadProfileList();
    void saveProfile() const;
    void deleteProfile() const;
    void updateProfile(const Profile3 &original) const;
    friend bool operator==(const Profile3 &left, const Profile3 &right);

private:
    bool deadBattery;

};

bool operator==(const Profile3 &left, const Profile3 &right);

#endif // PROFILE3_HPP
