/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef SPINDAPAINTER_HPP
#define SPINDAPAINTER_HPP

#include <Core/Global.hpp>
#include <QWidget>

class GraphicsPixmapItem;
class QGraphicsScene;
class QGraphicsPixmapItem;

namespace Ui
{
    class SpindaPainter;
}

/**
 * @brief Provides settings determine Spinda PID from the spots
 */
class SpindaPainter : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new SpindaPainter object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    SpindaPainter(QWidget *parent = nullptr);

    /**
     * @brief Destroy the SpindaPainter object
     */
    ~SpindaPainter() override;

private:
    Ui::SpindaPainter *ui;

    GraphicsPixmapItem *spots[4];

    /**
     * @brief Moves the spots on the Spinda based on the \p pid
     *
     * @param pid PID value
     */
    void moveSpots(u32 pid);

    /**
     * @brief Updates displayed nature/gender/ability when the \p pid is changed
     *
     * @param pid PID value
     */
    void updateInfo(u32 pid);

private slots:
    /**
     * @brief Updates the location of the Spinda spots when the PID is changed
     *
     * @param string PID text
     */
    void pidTextEdited(const QString &string);

    /**
     * @brief Updates the PID of the Spinda based on the location of the spots
     */
    void updatePID();
};

#endif // SPINDAPAINTER_HPP
