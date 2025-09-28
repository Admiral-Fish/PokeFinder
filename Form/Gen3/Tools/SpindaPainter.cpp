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

#include "SpindaPainter.hpp"
#include "ui_SpindaPainter.h"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/GraphicsPixmapItem.hpp>
#include <QSettings>

constexpr u8 coords[8] = { 0, 0, 24, 1, 6, 18, 18, 19 };
constexpr u8 origin[2] = { 8, 6 };

SpindaPainter::SpindaPainter(QWidget *parent) : QWidget(parent), ui(new Ui::SpindaPainter)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->textBoxPID->setValues(InputType::Seed32Bit);

    auto *scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    auto *spinda = new QGraphicsPixmapItem(QPixmap(":/images/spinda.png"));
    spots[0] = new GraphicsPixmapItem(QPixmap(":/images/spinda_spot1"), 64, 48, 184, 168);
    spots[1] = new GraphicsPixmapItem(QPixmap(":/images/spinda_spot2"), 256, 56, 376, 176);
    spots[2] = new GraphicsPixmapItem(QPixmap(":/images/spinda_spot3"), 112, 192, 232, 312);
    spots[3] = new GraphicsPixmapItem(QPixmap(":/images/spinda_spot4"), 208, 200, 328, 320);

    scene->addItem(spinda);
    for (GraphicsPixmapItem *spot : spots)
    {
        spot->setFlag(QGraphicsItem::ItemIsMovable);
        scene->addItem(spot);
    }

    connect(ui->textBoxPID, &TextBox::textEdited, this, &SpindaPainter::pidTextEdited);
    connect(scene, &QGraphicsScene::changed, this, [=]() { updatePID(); });

    QSettings setting;
    if (setting.contains("spindaPainter/geometry"))
    {
        this->restoreGeometry(setting.value("spindaPainter/geometry").toByteArray());
    }
}

SpindaPainter::~SpindaPainter()
{
    QSettings setting;
    setting.setValue("spindaPainter/geometry", this->saveGeometry());

    delete ui;
}

void SpindaPainter::moveSpots(u32 pid)
{
    for (int i = 0; i < 4; i++)
    {
        u8 x = (pid >> (i * 8)) & 0xf;
        u8 y = (pid >> (i * 8 + 4)) & 0xf;

        x += coords[2 * i] + origin[0];
        y += coords[2 * i + 1] + origin[1];

        spots[i]->setPos(x * 8, y * 8);
    }
}

void SpindaPainter::updateInfo(u32 pid)
{
    const PersonalInfo *info = PersonalLoader::getPersonal(Game::Gen3, 327);
    u8 gender = (pid & 0xff) < info->getGender();
    u16 ability = info->getAbility(pid & 1);

    QString string
        = QString("%1, %2, %3")
              .arg(QString::fromStdString(Translator::getNature(pid % 25)), QString::fromStdString(Translator::getGender(gender)),
                   QString::fromStdString(Translator::getAbility(ability)));
    ui->labelInfo->setText(string);
}

void SpindaPainter::pidTextEdited(const QString &string)
{
    u32 pid = string.toUInt(nullptr, 16);
    moveSpots(pid);
}

void SpindaPainter::updatePID()
{
    u32 pid = 0;
    for (size_t i = 0; i < 4; i++)
    {
        u32 left = static_cast<u32>(spots[i]->x() / 8) - coords[2 * i] - origin[0];
        u32 right = static_cast<u32>(spots[i]->y() / 8) - coords[2 * i + 1] - origin[1];

        pid |= (left << (i * 8));
        pid |= (right << (i * 8 + 4));
    }

    ui->textBoxPID->setText(QString::number(pid, 16));
    updateInfo(pid);
}
