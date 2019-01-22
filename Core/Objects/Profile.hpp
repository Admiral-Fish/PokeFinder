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

#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>
#include <Core/Objects/Game.hpp>
#include <Core/Objects/Global.hpp>
#include <Core/Objects/Language.hpp>

class Profile
{

public:
    Profile(const QString &profileName, Game version, u16 tid, u16 sid, Language language = Language::Nil);
    Profile();
    QString getVersionString() const;
    QString getLanguageString() const;
    Game getVersion() const;
    Language getLanguage() const;
    QString getProfileName() const;
    u16 getTID() const;
    u16 getSID() const;

protected:
    QString profileName;
    Game version;
    Language language;
    u16 tid;
    u16 sid;

};

#endif // PROFILE_HPP
