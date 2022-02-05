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

#ifndef IDS5_HPP
#define IDS5_HPP

#include <Core/Gen5/Profile5.hpp>
#include <QWidget>

class IDModel5;
class QMenu;

namespace Ui
{
    class IDs5;
}

class IDs5 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit IDs5(QWidget *parent = nullptr);
    ~IDs5();
    void updateProfiles();
    bool hasProfiles() const;

private:
    Ui::IDs5 *ui;
    IDModel5 *model;
    std::vector<Profile5> profiles;
    Profile5 currentProfile;
    QMenu *menu;

    void setupModels();

private slots:
    void setXOR(bool checked);
    void search();
    void find();
    void profileIndexChanged(int index);
    void tableViewContextMenu(const QPoint &pos);
    void profileManager();
};

#endif // IDS5_HPP
