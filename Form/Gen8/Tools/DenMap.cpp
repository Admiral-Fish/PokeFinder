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

#include "DenMap.hpp"
#include "ui_DenMap.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Util/Translator.hpp>
#include <QPainter>

DenMap::DenMap(QWidget *parent) : QWidget(parent), ui(new Ui::DenMap)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    locationIndexChanged(0);

    connect(ui->comboBoxLocation, &QComboBox::currentIndexChanged, this, &DenMap::locationIndexChanged);
    connect(ui->comboBoxDen, &QComboBox::currentIndexChanged, this, &DenMap::denIndexChanged);
}

DenMap::~DenMap()
{
    delete ui;
}

void DenMap::denIndexChanged(int index)
{
    if (index >= 0)
    {
        int location = ui->comboBoxLocation->currentIndex();
        u16 offset = location == 0 ? 0 : location == 1 ? 100 : 190;

        auto coordinates = Encounters8::getDenCoordinates(index + offset);

        QPixmap image;
        double elipseSize = 20;
        if (location == 0)
        {
            image.load(":/images/map.png");
            this->resize(image.width() >> 1, image.height() >> 1);
        }
        else if (location == 1)
        {
            image.load(":/images/map_ioa.png");
            this->resize(image.width() >> 1, image.height() >> 1);
        }
        else
        {
            image.load(":/images/map_ct.png");
            elipseSize = 40;
            this->resize(image.width() >> 2, image.height() >> 2);
        }

        QPainter paint(&image);
        paint.setPen(QPen(QBrush(Qt::red), elipseSize));
        paint.drawEllipse(QPoint(coordinates[0], coordinates[1]), 5, 5);

        ui->labelMap->setPixmap(image);
    }
}

void DenMap::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        u16 start = index == 0 ? 0 : index == 1 ? 100 : 190;
        u16 end = index == 0 ? 100 : index == 1 ? 190 : 276;
        u16 offset = index == 0 ? 0 : index == 1 ? 100 : 190;

        std::vector<u16> indices(end - start);
        std::vector<u16> locationIndices(end - start);

        std::iota(indices.begin(), indices.end(), start);
        std::transform(indices.begin(), indices.end(), locationIndices.begin(), [](u16 i) { return Encounters8::getDenLocation(i); });

        ui->comboBoxDen->clear();
        auto locations = Translator::getLocations(locationIndices, Game::SwSh);
        for (u16 i : indices)
        {
            ui->comboBoxDen->addItem(QString("%1: %2").arg(i + 1 - offset).arg(QString::fromStdString(locations[i - offset])));
        }

        denIndexChanged(0);
    }
}
