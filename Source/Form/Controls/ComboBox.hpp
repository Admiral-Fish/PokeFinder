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

#ifndef COMBOBOX_HPP
#define COMBOBOX_HPP

#include <Core/Global.hpp>
#include <QComboBox>

/**
 * @brief Provides extended combo box functionality for data types and enums
 */
class ComboBox : public QComboBox
{
public:
    /**
     * @brief Construct a new ComboBox object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    ComboBox(QWidget *parent);

    /**
     * @brief Enables type to autocomplete
     */
    void enableAutoComplete();

    /**
     * @brief Gets current selected index data as u8
     *
     * @return Current data
     */
    u8 getCurrentUChar() const;

    /**
     * @brief Gets current selected index data as u16
     *
     * @return Current data
     */
    u16 getCurrentUShort() const;

    /**
     * @brief Gets current selected index data as u32
     *
     * @return Current data
     */
    u32 getCurrentUInt() const;

    /**
     * @brief Gets current selected index data as int
     *
     * @return Current data
     */
    int getCurrentInt() const;

    /**
     * @brief Gets current selected index data as templated enum type
     *
     * @tparam Enum Enum type to convert data too
     *
     * @return Current data as enum type
     */
    template <typename Enum>
    constexpr Enum getEnum() const
    {
        static_assert(std::is_same<u8, typename std::underlying_type<Enum>::type>::value
                          || std::is_same<u16, typename std::underlying_type<Enum>::type>::value
                          || std::is_same<u32, typename std::underlying_type<Enum>::type>::value,
                      "Unsupported enum base type");

        if constexpr (std::is_same<u8, typename std::underlying_type<Enum>::type>::value)
        {
            return static_cast<Enum>(getCurrentUChar());
        }
        else if constexpr (std::is_same<u16, typename std::underlying_type<Enum>::type>::value)
        {
            return static_cast<Enum>(getCurrentUShort());
        }
        else if constexpr (std::is_same<u32, typename std::underlying_type<Enum>::type>::value)
        {
            return static_cast<Enum>(getCurrentUInt());
        }
        else
        {
            return static_cast<Enum>(0);
        }
    }

    /**
     * @brief Enables/disables and hide/unhides the item in \p row
     *
     * @param row Item row
     * @param hide Whether item should be hidden
     */
    void setItemHidden(int row, bool hide);

    /**
     * @brief Sets the data for each item in the combo box
     *
     * @param data Vector of data to set
     */
    void setup(const std::vector<QVariant> &data);
};

#endif // COMBOBOX_HPP
