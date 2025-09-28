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

#ifndef COMBOBOXPROXY_HPP
#define COMBOBOXPROXY_HPP

#include <Core/Global.hpp>
#include <QComboBox>

class ComboBoxProxyModel;
class QStandardItemModel;

/**
 * @brief Provides combo box that sorts the data via a proxy model
 */
class ComboBoxProxy : public QComboBox
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new ComboBox object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    ComboBoxProxy(QWidget *parent);

    /**
     * @brief Add item to the combobox
     *
     * @param string Item that should be added to the combo box
     */
    void addItem(const QString &string);

    /**
     * @brief Adds items to the combobox
     *
     * @param strings List of items that should be added to the combo box
     * @param sort Whether or not to sort the proxy model
     */
    void addItems(const std::vector<std::string> &strings, bool sort = true);

    /**
     * @brief Gets the current index. Maps the index from the proxy model to the model
     */
    int currentIndex() const;

    /**
     * @brief Enables type to autocomplete
     */
    void enableAutoComplete();

    /**
     * @brief Sets the current index. Maps the index from the model to the proxy model
     * 
     * @param index Index to set
     */
    void setCurrentIndex(int index);

private:
    QStandardItemModel *model;
    ComboBoxProxyModel *proxyModel;
};

#endif // COMBOBOXPROXY_HPP
