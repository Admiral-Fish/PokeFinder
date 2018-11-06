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
    connect(this, &QTextBox::textChanged, this, &QTextBox::onTextChanged);
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

// Differnt presets for different types of input
void QTextBox::setValues(InputType type)
{
    switch (type)
    {
        case InputType::Seed64Bit:
            minValue = 0;
            maxValue = 0xffffffffffffffff;
            base = 16;
            break;
        case InputType::Frame64Bit:
            minValue = 1;
            maxValue = 0xffffffffffffffff;
            base = 10;
            break;
        case InputType::Seed32Bit:
            minValue = 0;
            maxValue = 0xffffffff;
            base = 16;
            break;
        case InputType::Frame32Bit:
            minValue = 1;
            maxValue = 0xffffffff;
            base = 10;
            break;
        case InputType::Seed16Bit:
            minValue = 0;
            maxValue = 0xffff;
            base = 16;
            break;
        case InputType::Delay:
            minValue = 0;
            maxValue = 0xffffffff;
            base = 10;
            break;
        case InputType::TIDSID:
            minValue = 0;
            maxValue = 0xffff;
            base = 10;
            break;
    }

    filter = QRegExp(base == 10 ? "[^0-9]" : "[^0-9A-F]");
}

void QTextBox::setValues(quint64 minValue, quint64 maxValue, int base)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->base = base;
    filter = QRegExp(base == 10 ? "[^0-9]" : "[^0-9A-F]");
}

void QTextBox::setFilter(const QString &string)
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
