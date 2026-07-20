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

#ifndef INDEXFILTERPROXYMODEL_HPP
#define INDEXFILTERPROXYMODEL_HPP

#include <QSortFilterProxyModel>

/**
 * @brief Provides a proxy to filter data from a table model by indexes
 */
class IndexFilterProxyModel : public QSortFilterProxyModel
{
public:
    /**
     * @brief Construct a new IndexFilterProxyModel object
     *
     * @param parent Parent object, which takes memory ownership
     * @param model Source model to be processed by proxy
     */
    IndexFilterProxyModel(QObject *parent, QAbstractItemModel *model) : QSortFilterProxyModel(parent)
    {
        setSourceModel(model);
    }

    /**
     * @brief Set indexes the proxy model will filter to show
     *
     * @param indexes Indexes to be displayed
     */
    void setFilteredIndexes(const QModelIndexList &indexes)
    {
        allowedIndexes.clear();
        for (const QModelIndex &index : indexes)
        {
            allowedIndexes.insert(QPersistentModelIndex(index));
        }
        invalidateFilter();
    }

    /**
     * @brief Set indexes the proxy model will filter to show
     *
     * @param rows Rows to be displayed
     */
    void setFilteredIndexes(const std::vector<size_t> &rows)
    {
        allowedIndexes.clear();
        for (size_t row : rows)
        {
            allowedIndexes.insert(QPersistentModelIndex(sourceModel()->index(row, 0)));
        }
        invalidateFilter();
    }

protected:
    /**
     * @brief Checks if row should be displayed
     *
     * @param sourceRow Row to check
     * @param sourceParent Parent index of row
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
        return allowedIndexes.contains(QPersistentModelIndex(sourceIndex));
    }

private:
    QSet<QPersistentModelIndex> allowedIndexes;
};

#endif // INDEXFILTERPROXYMODEL_HPP
