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

#ifndef GRAPHICSPIXMAPITEM_HPP
#define GRAPHICSPIXMAPITEM_HPP

#include <Core/Global.hpp>
#include <QGraphicsPixmapItem>

/**
 * @brief Utility class that bounds X and Y position of QGraphicsPixmapItem
 */
class GraphicsPixmapItem : public QGraphicsPixmapItem
{
public:
    /**
     * @brief Construct a new GraphicsPixmapItem object
     *
     * @param pixmap Image to display
     * @param minX Minimum X position
     * @param minY Minimum Y position
     * @param maxX Maximum X position
     * @param maxY Maximum X position
     */
    GraphicsPixmapItem(const QPixmap &pixmap, u16 minX, u16 minY, u16 maxX, u16 maxY);

    /**
     * @brief Handles bounding the item with the min/max X/Y position when it is moved
     *
     * @param event Contains mouse event information
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    u16 maxX;
    u16 maxY;
    u16 minX;
    u16 minY;
};

#endif // GRAPHICSPIXMAPITEM_HPP
