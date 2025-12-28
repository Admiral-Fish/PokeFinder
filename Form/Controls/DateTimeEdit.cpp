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

#include "DateTimeEdit.hpp"
#include <Core/Util/DateTime.hpp>

DateTimeEdit::DateTimeEdit(QWidget *parent) : QDateTimeEdit(parent)
{
    setMinimumDateTime(QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0)));
    setMaximumDateTime(QDateTime(QDate(2099, 12, 31), QTime(23, 59, 59)));
    setCalendarPopup(true);
    setDisplayFormat("yyyy-MM-dd HH:mm");
}

DateTime DateTimeEdit::getDateTime() const
{
    int jd = date().toJulianDay();
    int md = time().msecsSinceStartOfDay() / 1000;
    return DateTime(jd, md);
}
