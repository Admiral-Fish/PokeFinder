/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <QDataStream>
#include <QIODevice>
#include <QMimeData>
#include <algorithm>
#include <iterator>
#include <vector>

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
        beginInsertRows(QModelIndex(), i, i);
        model.emplace_back(item);
        endInsertRows();
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
            beginInsertRows(QModelIndex(), i, i + static_cast<int>(items.size()) - 1);
            model.insert(model.end(), items.begin(), items.end());
            endInsertRows();
        }
    }

    /**
     * @brief Removes all items from the model
     */
    void clearModel()
    {
        if (!model.empty())
        {
            beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
            model.clear();
            model.shrink_to_fit();
            endRemoveRows();
        }
    }

    /**
     * @brief Returns the item from the \p row
     *
     * @param row Row to get
     *
     * @return Item data of the model
     */
    const Item &getItem(int row) const
    {
        return model[row];
    }

    /**
     * @brief Returns the underlying model data
     *
     * @return Vector of Items of the model
     */
    const std::vector<Item> &getModel() const
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
     * @brief Moves rows in the model
     *
     * @param sourceParent Unused source parent index
     * @param sourceRow First row to move
     * @param count Number of consecutive rows to move
     * @param destinationParent Unused destination parent index
     * @param destinationChild Row to insert before
     *
     * @return true Rows moved
     * @return false Invalid move
     */
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent,
                  int destinationChild) override
    {
        if (sourceParent.isValid() || destinationParent.isValid() || count < 1 || sourceRow < 0 || sourceRow + count > rowCount()
            || destinationChild < 0 || destinationChild > rowCount() || (destinationChild >= sourceRow && destinationChild <= sourceRow + count))
        {
            return false;
        }

        beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);

        std::vector<Item> items(std::make_move_iterator(model.begin() + sourceRow),
                                std::make_move_iterator(model.begin() + sourceRow + count));
        model.erase(model.begin() + sourceRow, model.begin() + sourceRow + count);
        if (sourceRow < destinationChild)
        {
            destinationChild -= count;
        }
        model.insert(model.begin() + destinationChild, std::make_move_iterator(items.begin()), std::make_move_iterator(items.end()));

        endMoveRows();
        return true;
    }

    /**
     * @brief Returns model MIME types for drag and drop
     *
     * @return MIME type list
     */
    QStringList mimeTypes() const override
    {
        return { "application/x-pokefinder-table-row" };
    }

    /**
     * @brief Returns MIME data for the dragged row
     *
     * @param indexes Selected indexes
     *
     * @return MIME data containing the dragged row
     */
    QMimeData *mimeData(const QModelIndexList &indexes) const override
    {
        if (indexes.isEmpty())
        {
            return nullptr;
        }

        std::vector<int> rows;
        rows.reserve(indexes.size());
        for (const auto &index : indexes)
        {
            rows.emplace_back(index.row());
        }
        std::ranges::sort(rows);
        rows.erase(std::ranges::unique(rows).begin(), rows.end());

        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << static_cast<int>(rows.size());
        for (int row : rows)
        {
            stream << row;
        }

        auto *mime = new QMimeData;
        mime->setData("application/x-pokefinder-table-row", data);
        return mime;
    }

    /**
     * @brief Handles dropped row data
     *
     * @param data MIME data
     * @param action Drop action
     * @param row Destination row
     * @param column Unused destination column
     * @param parent Destination parent
     *
     * @return true Data was dropped
     * @return false Invalid drop
     */
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override
    {
        if (action == Qt::IgnoreAction)
        {
            return true;
        }

        if (action != Qt::MoveAction || column > 0 || !data->hasFormat("application/x-pokefinder-table-row"))
        {
            return false;
        }

        QByteArray encoded = data->data("application/x-pokefinder-table-row");
        QDataStream stream(&encoded, QIODevice::ReadOnly);
        int size;
        stream >> size;
        if (size < 1)
        {
            return false;
        }

        std::vector<int> rows(size);
        for (int &sourceRow : rows)
        {
            stream >> sourceRow;
            if (sourceRow < 0 || sourceRow >= rowCount())
            {
                return false;
            }
        }

        int destinationRow = row;
        if (destinationRow == -1)
        {
            destinationRow = parent.isValid() ? parent.row() : rowCount();
        }
        if (destinationRow < 0 || destinationRow > rowCount())
        {
            return false;
        }

        bool contiguous = true;
        for (size_t i = 1; i < rows.size(); i++)
        {
            if (rows[i] != rows[i - 1] + 1)
            {
                contiguous = false;
                break;
            }
        }

        if (contiguous)
        {
            return moveRows(QModelIndex(), rows.front(), static_cast<int>(rows.size()), QModelIndex(), destinationRow);
        }

        if (std::ranges::find(rows, destinationRow) != rows.end())
        {
            return false;
        }

        beginResetModel();

        std::vector<Item> moved;
        moved.reserve(rows.size());
        for (int sourceRow : rows)
        {
            moved.emplace_back(std::move(model[sourceRow]));
        }

        std::vector<Item> remaining;
        remaining.reserve(model.size() - rows.size());
        for (int i = 0; i < rowCount(); i++)
        {
            if (!std::ranges::binary_search(rows, i))
            {
                remaining.emplace_back(std::move(model[i]));
            }
        }

        destinationRow -= static_cast<int>(std::ranges::count_if(rows, [destinationRow](int sourceRow) { return sourceRow < destinationRow; }));
        remaining.insert(remaining.begin() + destinationRow, std::make_move_iterator(moved.begin()), std::make_move_iterator(moved.end()));
        model = std::move(remaining);

        endResetModel();
        return true;
    }

    /**
     * @brief Removes an item from the model
     *
     * @param row Row to remove
     */
    void removeItem(int row)
    {
        beginRemoveRows(QModelIndex(), row, row);
        model.erase(model.begin() + row);
        model.shrink_to_fit();
        endRemoveRows();
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
        emit dataChanged(index(row, 0), index(row, columnCount() - 1));
    }

protected:
    std::vector<Item> model;
};

#endif // TABLEMODEL_HPP
