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

#include "ComboBox.hpp"

ComboBox::ComboBox(QWidget *parent) : QComboBox(parent)
{
}

void ComboBox::setup(const QVector<QVariant> &data)
{
    for (auto i = 0; i < data.size(); i++)
    {
        this->setItemData(i, data.at(i));
    }
}

u8 ComboBox::getCurrentByte() const
{
    return static_cast<u8>(this->currentData().toInt());
}

u16 ComboBox::getCurrentUShort() const
{
    return static_cast<u16>(this->currentData().toUInt());
}

int ComboBox::getCurrentInt() const
{
    return this->currentData().toInt();
}
