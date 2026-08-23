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

#include <QColor>
#include <QMouseEvent>
#include <QPainter>

static QColor getMarkColor(PokeRadarTileMark mark)
{
    switch (mark)
    {
    case PokeRadarTileMark::Weak:
        return QColor(QStringLiteral("#7fbd73"));
    case PokeRadarTileMark::Strong:
        return QColor(QStringLiteral("#1f5f2f"));
    case PokeRadarTileMark::Shiny:
        return QColor(QStringLiteral("#d9c63f"));
    case PokeRadarTileMark::None:
        return QColor(QStringLiteral("#5a5a5a"));
    }

    return QColor(QStringLiteral("#5a5a5a"));
}

PokeRadarTile::PokeRadarTile(bool center, QWidget *parent) :
    QFrame(parent),
    center(center),
    grass(!center),
    continues(false),
    split(false),
    mark(PokeRadarTileMark::None),
    rightMark(PokeRadarTileMark::None)
{
    setFrameShape(QFrame::StyledPanel);
    setFixedSize(22, 22);
    updateColor();
}

void PokeRadarTile::clearMark()
{
    continues = false;
    split = false;
    mark = PokeRadarTileMark::None;
    rightMark = PokeRadarTileMark::None;
    updateColor();
}

bool PokeRadarTile::hasGrass() const
{
    return grass;
}

bool PokeRadarTile::hasMark() const
{
    return mark != PokeRadarTileMark::None || split;
}

bool PokeRadarTile::isContinueMark() const
{
    return continues;
}

PokeRadarTileMark PokeRadarTile::getMark() const
{
    return mark;
}

void PokeRadarTile::setMark(PokeRadarTileMark mark, bool continues)
{
    if (!center)
    {
        this->continues = continues;
        split = false;
        this->mark = mark;
        rightMark = PokeRadarTileMark::None;
        updateColor();
    }
}

void PokeRadarTile::setSplitMark(PokeRadarTileMark leftMark, PokeRadarTileMark rightMark)
{
    if (!center)
    {
        continues = false;
        split = true;
        mark = leftMark;
        this->rightMark = rightMark;
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
            split = false;
            mark = PokeRadarTileMark::None;
            rightMark = PokeRadarTileMark::None;
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

    if (split)
    {
        QPainter painter(this);
        QRect inner = rect().adjusted(1, 1, -1, -1);
        QRect left = inner;
        left.setWidth(inner.width() / 2);
        QRect right = inner;
        right.setLeft(left.right() + 1);
        painter.fillRect(left, getMarkColor(mark));
        painter.fillRect(right, getMarkColor(rightMark));
    }

    if (!split && (mark == PokeRadarTileMark::Shiny || (mark != PokeRadarTileMark::None && continues)))
    {
        QPainter painter(this);
        painter.setPen(palette().color(QPalette::WindowText));
        painter.drawText(rect(), Qt::AlignCenter, mark == PokeRadarTileMark::Shiny ? QStringLiteral("S") : QStringLiteral("C"));
    }
}

void PokeRadarTile::updateColor()
{
    if (split)
    {
        setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #5a5a5a; border: 1px solid black; }"));
    }
    else if (mark == PokeRadarTileMark::Shiny)
    {
        setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #d9c63f; border: 1px solid black; }"));
    }
    else if (mark == PokeRadarTileMark::Strong)
    {
        setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #1f5f2f; border: 1px solid black; }"));
    }
    else if (mark == PokeRadarTileMark::Weak)
    {
        setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #7fbd73; border: 1px solid black; }"));
    }
    else if (center || !grass)
    {
        if (center)
        {
            setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #c04040; border: 1px solid black; }"));
        }
        else
        {
            setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #303030; border: 1px solid black; }"));
        }
    }
    else
    {
        setStyleSheet(QStringLiteral("PokeRadarTile { background-color: #5a5a5a; border: 1px solid black; }"));
    }
    update();
}
