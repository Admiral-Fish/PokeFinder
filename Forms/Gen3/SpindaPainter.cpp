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

#include "SpindaPainter.hpp"
#include "ui_SpindaPainter.h"

SpindaPainter::SpindaPainter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpindaPainter)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

SpindaPainter::~SpindaPainter()
{
    delete ui;
    delete spinda;
    delete spot1;
    delete spot2;
    delete spot3;
    delete spot4;
}

void SpindaPainter::setupModels()
{
    ui->textBoxPID->setValues(InputType::Seed32Bit);

    scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    spinda = new QGraphicsPixmapItem(QPixmap(":/images/spinda.png"));
    spot1 = new QGraphicsPixmapItem(QPixmap(":/images/spinda_spot1"));
    spot2 = new QGraphicsPixmapItem(QPixmap(":/images/spinda_spot2"));
    spot3 = new QGraphicsPixmapItem(QPixmap(":/images/spinda_spot3"));
    spot4 = new QGraphicsPixmapItem(QPixmap(":/images/spinda_spot4"));

    spot1->setFlag(QGraphicsItem::ItemIsMovable);
    spot2->setFlag(QGraphicsItem::ItemIsMovable);
    spot3->setFlag(QGraphicsItem::ItemIsMovable);
    spot4->setFlag(QGraphicsItem::ItemIsMovable);

    scene->addItem(spinda);
    scene->addItem(spot1);
    scene->addItem(spot2);
    scene->addItem(spot3);
    scene->addItem(spot4);

    ui->textBoxPID->setText("0");
}

void SpindaPainter::moveSpot(QGraphicsPixmapItem *item, int index)
{
    int left = (pid >> (index * 8)) & 0xf;
    int top = (pid >> (index * 8 + 4)) & 0xf;

    left += coords[2 * index] + origin[0];
    top += coords[2 * index + 1] + origin[1];

    item->setPos(left * 8, top * 8);
}

void SpindaPainter::on_textBoxPID_textChanged(const QString &arg1)
{
    u32 newPID = arg1.toUInt(nullptr, 16);

    if (pid != newPID)
    {
        pid = newPID;

        moveSpot(spot1, 0);
        moveSpot(spot2, 1);
        moveSpot(spot3, 2);
        moveSpot(spot4, 3);
    }
}
