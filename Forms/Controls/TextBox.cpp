/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

namespace PokeFinderForms
{
    TextBox::TextBox(QWidget *parent) : QLineEdit(parent)
    {
        connect(this, &TextBox::textEdited, this, &TextBox::onTextEdited);
        setup = false;
    }

    void TextBox::setValues(InputType type)
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
        setup = true;
    }

    void TextBox::setValues(u64 minValue, u64 maxValue, int base)
    {
        this->minValue = minValue;
        this->maxValue = maxValue;
        this->base = base;
        filter = QRegExp(base == 10 ? "[^0-9]" : "[^0-9A-F]");
        setup = true;
    }

    int TextBox::getInt() const
    {
        Q_ASSERT(setup);
        return this->text().toInt(nullptr, base);
    }

    u16 TextBox::getUShort() const
    {
        Q_ASSERT(setup);
        return this->text().toUShort(nullptr, base);
    }

    u32 TextBox::getUInt() const
    {
        Q_ASSERT(setup);
        return this->text().toUInt(nullptr, base);
    }

    void TextBox::onTextEdited(QString string)
    {
        if (setup)
        {
            string = string.toUpper();
            string.remove(filter);

            u64 temp = string.toULongLong(nullptr, base);
            temp = qBound(minValue, temp, maxValue);
            string = QString::number(temp, base);

            int position = cursorPosition();
            setText(string);
            setCursorPosition(position);
        }
    }
}
