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

#ifndef POKERADARTILE_HPP
#define POKERADARTILE_HPP

#include <QFrame>

enum class PokeRadarTileMark
{
    None,
    Weak,
    Strong,
    Shiny
};

class PokeRadarTile : public QFrame
{
    Q_OBJECT
public:
    PokeRadarTile(bool center, QWidget *parent = nullptr);

    void clearMark();
    bool hasGrass() const;
    bool hasMark() const;
    bool isContinueMark() const;
    PokeRadarTileMark getMark() const;
    void setMark(PokeRadarTileMark mark, bool continues = false);
    void setSplitMark(PokeRadarTileMark leftMark, PokeRadarTileMark rightMark);
    void setGrass(bool grass);

private:
    bool center;
    bool grass;
    bool continues;
    bool split;
    PokeRadarTileMark mark;
    PokeRadarTileMark rightMark;

    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void updateColor();
};

#endif // POKERADARTILE_HPP
