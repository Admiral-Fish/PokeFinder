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

#include "ComboBox.hpp"
#include <QListView>
#include <QStandardItemModel>

ComboBox::ComboBox(QWidget *parent) : QComboBox(parent)
{
}

void ComboBox::setup(const std::vector<QVariant> &data)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        this->setItemData(i, data[i]);
    }
}

u8 ComboBox::getCurrentByte() const
{
    return static_cast<u8>(this->currentData().toUInt());
}

u16 ComboBox::getCurrentUShort() const
{
    return static_cast<u16>(this->currentData().toUInt());
}

u32 ComboBox::getCurrentUInt() const
{
    return this->currentData().toUInt();
}

int ComboBox::getCurrentInt() const
{
    return this->currentData().toInt();
}

void ComboBox::setItemHidden(int row, bool hide)
{
    auto *model = qobject_cast<QStandardItemModel *>(this->model());
    auto *view = qobject_cast<QListView *>(this->view());

    model->item(row)->setEnabled(!hide);
    view->setRowHidden(row, hide);
}
