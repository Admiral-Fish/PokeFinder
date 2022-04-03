/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef EVENT8_HPP
#define EVENT8_HPP

#include <QWidget>

class EventModel8;
class Profile8;
class WB8;
class QMenu;

namespace Ui
{
    class Event8;
}

class Event8 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Event8(QWidget *parent = nullptr);
    ~Event8() override;
    void updateProfiles();

private:
    Ui::Event8 *ui;
    EventModel8 *model;
    std::vector<Profile8> profiles;
    Profile8 *currentProfile;
    QMenu *menu;

    void setupModels();
    WB8 getParameters() const;

private slots:
    void generate();
    void importEvent();
    void profileIndexChanged(int index);
    void tableViewContextMenu(QPoint pos);
    void profileManager();
};

#endif // EVENT5_HPP
