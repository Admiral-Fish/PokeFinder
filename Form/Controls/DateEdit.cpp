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

#include "DateEdit.hpp"
#include <Core/Util/DateTime.hpp>

DateEdit::DateEdit(QWidget *parent) : QDateEdit(parent)
{
    QDateEdit::setMinimumDate(QDate(2000, 1, 1));
    QDateEdit::setMaximumDate(QDate(2099, 12, 31));
    setCalendarPopup(true);
    setDisplayFormat("yyyy-MM-dd");
}

void DateEdit::clearMaximumDate()
{
    QDateEdit::setMaximumDate(QDate(2099, 12, 31));
}

void DateEdit::clearMinimumDate()
{    
    QDateEdit::setMinimumDate(QDate(2000, 1, 1));
}

Date DateEdit::getDate() const
{
    return Date(date().toJulianDay());
}

void DateEdit::setMaximumDate(Date date)
{
    QDateEdit::setMaximumDate(QDate::fromJulianDay(date.getJD()));
}

void DateEdit::setMinimumDate(Date date)
{
    QDateEdit::setMinimumDate(QDate::fromJulianDay(date.getJD()));
}
