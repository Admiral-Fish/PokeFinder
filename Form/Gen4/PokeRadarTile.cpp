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

#include "PokeRadarTile.hpp"

#include <QMouseEvent>
#include <QPainter>

PokeRadarTile::PokeRadarTile(bool center, QWidget *parent) :
    QFrame(parent),
    center(center),
    grass(!center),
    continues(false),
    mark(PokeRadarTileMark::None)
{
    setFrameShape(QFrame::StyledPanel);
    setFixedSize(22, 22);
    updateColor();
}

void PokeRadarTile::clearMark()
{
    continues = false;
    mark = PokeRadarTileMark::None;
    updateColor();
}

bool PokeRadarTile::hasGrass() const
{
    return grass;
}

void PokeRadarTile::setMark(PokeRadarTileMark mark, bool continues)
{
    if (!center)
    {
        this->continues = continues;
        this->mark = mark;
        updateColor();
    }
}

void PokeRadarTile::setGrass(bool grass)
{
    if (!center)
    {
        this->grass = grass;
        if (!grass)
        {
            continues = false;
            mark = PokeRadarTileMark::None;
        }
        updateColor();
    }
}

void PokeRadarTile::mousePressEvent(QMouseEvent *event)
{
    if (!center && event->button() == Qt::LeftButton)
    {
        setGrass(!grass);
        event->accept();
        return;
    }

    QFrame::mousePressEvent(event);
}

void PokeRadarTile::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if (mark == PokeRadarTileMark::Shiny || (mark != PokeRadarTileMark::None && continues))
    {
        QPainter painter(this);
        painter.setPen(palette().color(QPalette::WindowText));
        painter.drawText(rect(), Qt::AlignCenter, mark == PokeRadarTileMark::Shiny ? QStringLiteral("S") : QStringLiteral("C"));
    }
}

void PokeRadarTile::updateColor()
{
    if (mark == PokeRadarTileMark::Shiny)
    {
        setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #d9c63f; border: 1px solid black; }"));
    }
    else if (mark == PokeRadarTileMark::Strong)
    {
        setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #1f5f2f; border: 1px solid black; }"));
    }
    else if (mark == PokeRadarTileMark::Regular)
    {
        setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #7fbd73; border: 1px solid black; }"));
    }
    else if (center || !grass)
    {
        setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #303030; border: 1px solid black; }"));
    }
    else
    {
        setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #5a5a5a; border: 1px solid black; }"));
    }
    update();
}
