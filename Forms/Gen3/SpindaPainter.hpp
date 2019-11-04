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

#ifndef SPINDAPAINTER_HPP
#define SPINDAPAINTER_HPP

#include <Core/Util/Global.hpp>
#include <QGraphicsScene>
#include <QWidget>

namespace PokeFinderForms
{
    namespace Ui
    {
        class SpindaPainter;
    }

    class GraphicsPixmapItem;

    class SpindaPainter : public QWidget
    {
        Q_OBJECT
    public:
        explicit SpindaPainter(QWidget *parent = nullptr);
        ~SpindaPainter() override;

    private:
        std::unique_ptr<Ui::SpindaPainter> ui;
        QGraphicsScene *scene {};
        QGraphicsPixmapItem *spinda {};
        GraphicsPixmapItem *spot1 {};
        GraphicsPixmapItem *spot2 {};
        GraphicsPixmapItem *spot3 {};
        GraphicsPixmapItem *spot4 {};
        u32 pid = 0;
        bool text = false;

        void setupModels();
        void moveSpot(GraphicsPixmapItem *item, int index);
        void updateInfo();

    private slots:
        void on_textBoxPID_textEdited(const QString &arg1);
        void updatePID(const QList<QRectF> &);
    };
}

#endif // SPINDAPAINTER_HPP
