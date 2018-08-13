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

#ifndef QTEXTBOX_HPP
#define QTEXTBOX_HPP

#include <QLineEdit>

enum InputType
{
    Seed64Bit,
    Frame64Bit,
    Seed32Bit,
    Frame32Bit,
    Seed16Bit,
    Delay,
    TIDSID
};

class QTextBox : public QLineEdit
{
    Q_OBJECT

private:
    quint64 maxValue = 0;
    quint64 minValue;
    int base;
    QRegExp filter;

private slots:
    void onTextChanged(QString string);

public:
    QTextBox(QWidget *parent = nullptr);
    void setValues(InputType type);
    void setValues(quint64 minValue, quint64 maxValue, int base = 10);
    void setFilter(QString string);
    void setValue(quint64 value);
    void setBase(int base);

};

#endif // QTEXTBOX_HPP
