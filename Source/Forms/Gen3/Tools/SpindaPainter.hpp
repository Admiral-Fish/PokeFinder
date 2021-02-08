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

#ifndef SPINDAPAINTER_HPP
#define SPINDAPAINTER_HPP

#include <Forms/Controls/GraphicsPixmapItem.hpp>
#include <QWidget>

namespace Ui
{
    class SpindaPainter;
}

class SpindaPainter : public QWidget
{
    Q_OBJECT

public:
    explicit SpindaPainter(QWidget *parent = nullptr);
    ~SpindaPainter() override;

private:
    Ui::SpindaPainter *ui;
    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *spinda = nullptr;
    GraphicsPixmapItem *spot1 = nullptr;
    GraphicsPixmapItem *spot2 = nullptr;
    GraphicsPixmapItem *spot3 = nullptr;
    GraphicsPixmapItem *spot4 = nullptr;

    void setupModels();
    void moveSpot(GraphicsPixmapItem *item, int index, u32 pid);
    void updateInfo(u32 pid);
    bool text;

private slots:
    void pidTextEdited(const QString &string);
    void updatePID(const QList<QRectF> &);
};

#endif // SPINDAPAINTER_HPP
