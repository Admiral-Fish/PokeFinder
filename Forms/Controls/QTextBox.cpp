/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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
        quint64 temp = string.toULongLong(nullptr, base);

        if (temp > maxValue)
            string = QString::number(maxValue, base);
        if (temp < minValue)
            string = QString::number(minValue, base);

        int position = cursorPosition();
        setText(string);
        setCursorPosition(position);
    }
}

void QTextBox::setValues(quint64 min, quint64 shift, bool isDecimal)
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

void QTextBox::setValue(quint64 value)
{
    maxValue = value;
}

void QTextBox::setBase(int base)
{
    this->base = base;
}
