/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "TextBox.hpp"

TextBox::TextBox(QWidget *parent) : QLineEdit(parent)
{
    blockSignals(true);
    connect(this, &TextBox::textEdited, this, &TextBox::onTextEdited);
}

void TextBox::setValues(InputType type)
{
    switch (type)
    {
    case InputType::Seed64Bit:
        minValue = 0;
        maxValue = 0xffffffffffffffff;
        length = 16;
        base = 16;
        break;
    case InputType::Seed32Bit:
        minValue = 0;
        maxValue = 0xffffffff;
        length = 8;
        base = 16;
        break;
    case InputType::Advance32Bit:
        minValue = 0;
        maxValue = 0xffffffff;
        length = 10;
        base = 10;
        break;
    case InputType::Seed16Bit:
        minValue = 0;
        maxValue = 0xffff;
        length = 4;
        base = 16;
        break;
    case InputType::Delay:
        minValue = 0;
        maxValue = 0xffffffff;
        length = 10;
        base = 10;
        break;
    case InputType::TIDSID:
        minValue = 0;
        maxValue = 0xffff;
        length = 5;
        base = 10;
        break;
    }

    filter = QRegularExpression(base == 10 ? "[^0-9]" : "[^0-9A-F]");
    blockSignals(false);
}

void TextBox::setValues(u64 minValue, u64 maxValue, int length, int base)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->length = length;
    this->base = base;
    filter = QRegularExpression(base == 10 ? "[^0-9]" : "[^0-9A-F]");
    blockSignals(false);
}

int TextBox::getInt() const
{
    return this->text().toInt(nullptr, base);
}

u8 TextBox::getUChar() const
{
    return static_cast<u8>(this->text().toUInt(nullptr, base));
}

u16 TextBox::getUShort() const
{
    return this->text().toUShort(nullptr, base);
}

u32 TextBox::getUInt() const
{
    return this->text().toUInt(nullptr, base);
}

u64 TextBox::getULong() const
{
    return this->text().toULongLong(nullptr, base);
}

void TextBox::onTextEdited(QString string)
{
    // Early return for empty string
    if (string.isEmpty())
    {
        return;
    }

    // Allow hex formatted strings
    if (base == 16 && string.startsWith("0x"))
    {
        string.remove(0, 2);
    }

    // Length limit
    string.chop(string.length() - length);

    // Apply regex filter
    string = string.toUpper();
    string.remove(filter);

    // Remove any useless leading zeros
    int count = 0;
    while (string.size() > 1 && string[0] == '0')
    {
        string.remove(0, 1);
        count++;
    }

    // Bound value
    u64 value = string.toULongLong(nullptr, base);
    value = qBound(minValue, value, maxValue);

    int position = this->cursorPosition();
    this->setText(QString::number(value, base));
    this->setCursorPosition(position - count);
}
