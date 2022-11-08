/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen8/DenLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <QPainter>

DenMap::DenMap(QWidget *parent) : QWidget(parent), ui(new Ui::DenMap)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    locationIndexChanged(0);

    connect(ui->comboBoxLocation, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DenMap::locationIndexChanged);
    connect(ui->comboBoxDen, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DenMap::denIndexChanged);
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
        int offset = location == 0 ? 0 : 100;

        std::array<u16, 2> coordinates = DenLoader::getCoordinates(index + offset);

        QPixmap image;
        if (location == 0)
        {
            image.load(":/images/map.png");
            this->resize(245, 578);
        }
        else
        {
            image.load(":/images/map_ioa.png");
            this->resize(609, 637);
        }

        QPainter paint(&image);
        paint.setPen(QPen(QBrush(Qt::red), 20));
        paint.drawEllipse(QPoint(coordinates[0], coordinates[1]), 5, 5);

        ui->labelMap->setPixmap(image);
    }
}

void DenMap::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        u8 start = index == 0 ? 0 : 100;
        u8 end = index == 0 ? 100 : 190;
        u8 offset = index == 0 ? 0 : 100;

        std::vector<u16> indices(end - start);
        std::vector<u16> locationIndices(end - start);

        std::iota(indices.begin(), indices.end(), start);
        std::transform(indices.begin(), indices.end(), locationIndices.begin(), [](u16 i) { return DenLoader::getLocation(i); });

        ui->comboBoxDen->clear();
        auto locations = Translator::getLocations(locationIndices, Game::SwSh);
        for (u8 i : indices)
        {
            ui->comboBoxDen->addItem(QString("%1: %2").arg(i + 1 - offset).arg(QString::fromStdString(locations[i - offset])));
        }

        denIndexChanged(0);
    }
}
