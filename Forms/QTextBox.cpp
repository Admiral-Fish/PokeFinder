/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
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
    connect(this, &QLineEdit::textChanged, this, &QTextBox::OnTextChanged);
}

void QTextBox::OnTextChanged(QString string)
{
    if (maxValue != NULL)
    {
        string = string.toUpper();
        string.remove(filter);
        u64 temp = string.toULongLong(NULL, base);
        if (temp > maxValue)
        {
            string.remove(string.length() - 1, 1);
        }
        setText(string);
    }
}

void QTextBox::SetValues(QString string, u64 value, u32 base)
{
    maxValue = value;
    this->base = base;
    filter = QRegExp(string);
}
