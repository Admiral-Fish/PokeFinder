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

#ifndef TABLEMODEL_HPP
#define TABLEMODEL_HPP

#include <QAbstractTableModel>

template <class Item>
class TableModel : public QAbstractTableModel
{
public:
    TableModel(QObject *parent = nullptr) : QAbstractTableModel(parent)
    {
    }

    void addItems(const std::vector<Item> &items)
    {
        if (!items.empty())
        {
            int i = rowCount();
            emit beginInsertRows(QModelIndex(), i, i + static_cast<int>(items.size()) - 1);
            model.insert(model.end(), items.begin(), items.end());
            emit endInsertRows();
        }
    }

    void addItem(const Item &item)
    {
        int i = rowCount();
        emit beginInsertRows(QModelIndex(), i, i);
        model.emplace_back(item);
        emit endInsertRows();
    }

    void updateItem(const Item &item, int row)
    {
        model[row] = item;
        emit dataChanged(index(row, 0), index(row, columnCount()));
    }

    void removeItem(int row)
    {
        emit beginRemoveRows(QModelIndex(), row, row);
        model.erase(model.begin() + row);
        model.shrink_to_fit();
        emit endRemoveRows();
    }

    Item getItem(int row) const
    {
        return model[row];
    }

    std::vector<Item> getModel() const
    {
        return model;
    }

    void clearModel()
    {
        if (!model.empty())
        {
            emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
            model.clear();
            model.shrink_to_fit();
            emit endRemoveRows();
        }
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        (void)parent;
        return static_cast<int>(model.size());
    }

protected:
    std::vector<Item> model;
};

#endif // TABLEMODEL_HPP
