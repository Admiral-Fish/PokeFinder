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

#ifndef RAIDS_HPP
#define RAIDS_HPP

#include <Core/Gen8/Profile8.hpp>
#include <QWidget>

class RaidModel;
class QMenu;

namespace Ui
{
    class Raids;
}

class Raids : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Raids(QWidget *parent = nullptr);
    ~Raids() override;
    void updateProfiles();

private:
    Ui::Raids *ui;
    RaidModel *model;
    std::vector<Profile8> profiles;
    Profile8 currentProfile;
    QMenu *menu;

    void setupModels();

private slots:
    void generate();
    void profileIndexChanged(int index);
    void locationIndexChanged(int index);
    void denIndexChanged(int index);
    void rarityIndexChange(int index);
    void speciesIndexChanged(int index);
    void tableViewContextMenu(QPoint pos);
    void profileManager();
};

#endif // RAIDS_HPP
