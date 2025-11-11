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

#ifndef DATEEDIT_HPP
#define DATEEDIT_HPP

#include <QDateEdit>

class Date;

/**
 * @brief Utility class that converts QDate to internally used Date class
 */
class DateEdit : public QDateEdit
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new DateEdit object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    DateEdit(QWidget *parent = nullptr);

    /**
     * @brief Resets maximum date to the default value
     */
    void clearMaximumDate();

    /**
     * @brief Resets minimum date to the default value
     */
    void clearMinimumDate();

    /**
     * @brief Converts QDate to \ref Date
     *
     * @return Date information
     */
    Date getDate() const;

    /**
     * @brief Sets the maximum date
     * 
     * @param date Maximum date
     */
    void setMaximumDate(Date date);

    /**
     * @brief Sets the minimum date
     * 
     * @param date Minimum date
     */
    void setMinimumDate(Date date);
};

#endif // DATEEDIT_HPP
