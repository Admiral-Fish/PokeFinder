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

#include "ComboBoxProxy.hpp"
#include <Model/SortFilterProxyModel.hpp>
#include <QCompleter>
#include <QStandardItemModel>

/**
 * @brief Provides a proxy to sort strings placing "None" at the top
 */
class ComboBoxProxyModel : public SortFilterProxyModel
{
public:
    /**
     * @brief Construct a new ComboBoxProxyModel object
     *
     * @param parent Parent object, which takes memory ownership
     * @param model Source model to be processed by proxy
     */
    ComboBoxProxyModel(QObject *parent, QAbstractItemModel *model) : SortFilterProxyModel(parent, model)
    {
    }

    /**
     * @brief Compares two items in the model
     *
     * @param source_left First item to compare
     * @param source_right Second item to compare
     *
     * @return true First item is less than second item
     * @return false First item is not less than second item
     */
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override
    {
        QString left = source_left.data().toString();
        QString right = source_right.data().toString();

        if (left == tr("None"))
        {
            return true;
        }
        else if (right == tr("None"))
        {
            return false;
        }
        else
        {
            return left < right;
        }
    }
};

ComboBoxProxy::ComboBoxProxy(QWidget *parent) : QComboBox(parent)
{
    model = new QStandardItemModel(this);
    proxyModel = new ComboBoxProxyModel(this, model);

    setModel(proxyModel);
}

void ComboBoxProxy::addItem(const QString &string)
{
    QStandardItem *item = new QStandardItem(string);
    model->appendRow(item);
}

void ComboBoxProxy::addItems(const std::vector<std::string> &strings, bool sort)
{
    for (const auto &string : strings)
    {
        addItem(QString::fromStdString(string));
    }

    if (sort)
    {
        proxyModel->sort(0);

        // Set index back to 0 after sorting
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(0, 0));
        setCurrentIndex(index.row());
    }
}

int ComboBoxProxy::currentIndex() const
{
    QModelIndex index = proxyModel->mapToSource(proxyModel->index(QComboBox::currentIndex(), 0));
    return index.row();
}

void ComboBoxProxy::enableAutoComplete()
{
    setEditable(true);
    setInsertPolicy(QComboBox::NoInsert);
    completer()->setCompletionMode(QCompleter::PopupCompletion);
}

void ComboBoxProxy::setCurrentIndex(int index)
{
    QModelIndex modelIndex = proxyModel->mapFromSource(model->index(index, 0));
    QComboBox::setCurrentIndex(modelIndex.row());
}
