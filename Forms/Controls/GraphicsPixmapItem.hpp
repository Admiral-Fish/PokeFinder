/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef GRAPHICSPIXMAPITEM_HPP
#define GRAPHICSPIXMAPITEM_HPP

#include <Core/Util/Global.hpp>
#include <QGraphicsPixmapItem>

class GraphicsPixmapItem : public QGraphicsPixmapItem
{
public:
    explicit GraphicsPixmapItem(const QPixmap &pixmap, u16 minX = 0, u16 minY = 0, u16 maxX = 0, u16 maxY = 0);
    void setMin(u16 minX, u16 minY);
    void setMax(u16 maxX, u16 maxY);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    u16 minX, maxX;
    u16 minY, maxY;
};

#endif // GRAPHICSPIXMAPITEM_HPP
