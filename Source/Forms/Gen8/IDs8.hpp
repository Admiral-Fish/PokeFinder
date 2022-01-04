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

#ifndef IDS8_HPP
#define IDS8_HPP

#include <Core/Gen8/Profile8.hpp>
#include <QWidget>

class IDModel8;

namespace Ui
{
    class IDs8;
}

class IDs8 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit IDs8(QWidget *parent = nullptr);
    ~IDs8() override;
    void updateProfiles();

private:
    Ui::IDs8 *ui;
    IDModel8 *model;
    std::vector<Profile8> profiles;

    void setupModel();

private slots:
    void generate();
    void profileManager();
    void profilesIndexChanged(int index);
};

#endif // IDS8_HPP
