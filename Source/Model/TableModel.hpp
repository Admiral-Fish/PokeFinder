/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

/**
 * @brief Provides a templated implementation for children to add/edit/remove their data to a table model
 */
template <class Item>
class TableModel : public QAbstractTableModel
{
public:
    /**
     * @brief Construct a new TableModel object
     *
     * @param parent Parent object, which takes memory ownership
     */
    TableModel(QObject *parent = nullptr) : QAbstractTableModel(parent)
    {
    }

    /**
     * @brief Add a \p item to the model
     *
     * @param item Item to add
     */
    void addItem(const Item &item)
    {
        int i = rowCount();
        emit beginInsertRows(QModelIndex(), i, i);
        model.emplace_back(item);
        emit endInsertRows();
    }

    /**
     * @brief Add a collection of \p items to the model
     *
     * @param items Vector of items to add
     */
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

    /**
     * @brief Removes all items from the model
     */
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

    /**
     * @brief Returns the item from the \p row
     *
     * @param row Row to get
     *
     * @return Item data of the model
     */
    Item getItem(int row) const
    {
        return model[row];
    }

    /**
     * @brief Returns the underlying model data
     *
     * @return Vector of Items of the model
     */
    std::vector<Item> getModel() const
    {
        return model;
    }

    /**
     * @brief Returns the number of rows in the model
     *
     * @param parent Unused parent index
     *
     * @return Number of rows
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return static_cast<int>(model.size());
    }

    /**
     * @brief Removes an item from the model
     *
     * @param row Row to remove
     */
    void removeItem(int row)
    {
        emit beginRemoveRows(QModelIndex(), row, row);
        model.erase(model.begin() + row);
        model.shrink_to_fit();
        emit endRemoveRows();
    }

    /**
     * @brief Updates a \p item in the model
     *
     * @param item Updated item
     *
     * @param row Row to edit
     */
    void updateItem(const Item &item, int row)
    {
        model[row] = item;
        emit dataChanged(index(row, 0), index(row, columnCount()));
    }

protected:
    std::vector<Item> model;
};

#endif // TABLEMODEL_HPP
