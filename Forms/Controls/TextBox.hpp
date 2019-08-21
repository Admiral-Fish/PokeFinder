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

#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include <QLineEdit>
#include <Core/Util/Global.hpp>

enum InputType
{
    Seed64Bit   = 1 << 0,
    Frame64Bit  = 1 << 1,
    Seed32Bit   = 1 << 2,
    Frame32Bit  = 1 << 3,
    Seed16Bit   = 1 << 4,
    Delay       = 1 << 5,
    TIDSID      = 1 << 6
};

class TextBox : public QLineEdit
{
    Q_OBJECT

public:
    explicit TextBox(QWidget *parent = nullptr);
    void setValues(InputType type);
    void setValues(u64 minValue, u64 maxValue, int base = 10);
    int getInt() const;
    u16 getUShort() const;
    u32 getUInt() const;
    u64 getULong() const;

private:
    bool setup;
    u64 maxValue = 0;
    u64 minValue{};
    int base{};
    QRegExp filter;

private slots:
    void onTextEdited(QString string);

};

#endif // TEXTBOX_HPP
