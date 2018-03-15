/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#include "QTextBox.hpp"

QTextBox::QTextBox(QWidget *parent) : QLineEdit(parent)
{
    connect(this, SIGNAL (textChanged(QString)), this, SLOT (onTextChanged(QString)));
}

void QTextBox::onTextChanged(QString string)
{
    if (maxValue != 0)
    {
        string = string.toUpper();
        string.remove(filter);
        u64 temp = string.toULongLong(NULL, base);

        if (temp > maxValue)
            string = QString::number(maxValue, base);
        if (temp < minValue)
            string = QString::number(minValue, base);

        setText(string);
    }
}

void QTextBox::setValues(u64 min, u64 shift, bool isDecimal)
{
    maxValue = 0xFFFFFFFFFFFFFFFF >> shift;
    minValue = min;
    base = isDecimal ? 10 : 16;
    filter = QRegExp(isDecimal ? "[^0-9]" : "[^0-9A-F]");
}

void QTextBox::setFilter(QString string)
{
    filter = QRegExp(string);
}

void QTextBox::setValue(u64 value)
{
    maxValue = value;
}

void QTextBox::setBase(u32 base)
{
    this->base = base;
}
