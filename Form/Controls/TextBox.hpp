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

#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include <Core/Global.hpp>
#include <QLineEdit>
#include <QRegularExpression>

/**
 * @brief Enum that encompasses the different common input types for the \ref TextBox
 */
enum class InputType : u8
{
    Advance32Bit,
    Delay,
    Seed64Bit,
    Seed32Bit,
    Seed16Bit,
    TIDSID
};

/**
 * @brief Provides a line edit purely for numbers and validates the contents based on regex for number base and minimum/maximum bounding
 * values.
 */
class TextBox : public QLineEdit
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new TextBox object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    TextBox(QWidget *parent = nullptr);

    /**
     * @brief Sets regex and min/max bounding values
     *
     * @param type Predefined validation type
     */
    void setValues(InputType type);

    /**
     * @brief Sets regex and min/max bounding values
     *
     * @param minValue Minimum bounding value
     * @param maxValue Maximum bounding value
     * @param length Maximum allowed length
     * @param base Number base
     */
    void setValues(u64 minValue, u64 maxValue, int length, int base);

    /**
     * @brief Gets current text as int
     *
     * @return Converted value
     */
    int getInt() const;

    /**
     * @brief Gets current text as u8
     *
     * @return Converted value
     */
    u8 getUChar() const;

    /**
     * @brief Gets current text as u16
     *
     * @return Converted value
     */
    u16 getUShort() const;

    /**
     * @brief Gets current text as u32
     *
     * @return Converted value
     */
    u32 getUInt() const;

    /**
     * @brief Gets current text as u64
     *
     * @return Converted value
     */
    u64 getULong() const;

private:
    QRegularExpression filter;
    u64 maxValue;
    u64 minValue;
    int base;
    int length;

private slots:
    /**
     * @brief Validates text based on regex and min/max bounding values
     *
     * @param string Newly edited text
     */
    void onTextEdited(QString string);
};

#endif // TEXTBOX_HPP
