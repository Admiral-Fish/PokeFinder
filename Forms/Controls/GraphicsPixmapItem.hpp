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

#ifndef GRAPHICSPIXMAPITEM_HPP
#define GRAPHICSPIXMAPITEM_HPP

#include <QGraphicsPixmapItem>
#include <Core/Util/Global.hpp>

class GraphicsPixmapItem : public QGraphicsPixmapItem
{

public:
    GraphicsPixmapItem(const QPixmap &pixmap);
    void setMin(u16 x, u16 y);
    void setMax(u16 x, u16 y);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    u16 minX, maxX;
    u16 minY, maxY;

};

#endif // GRAPHICSPIXMAPITEM_HPP
