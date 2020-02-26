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

#ifndef IVTOPIDHPP
#define IVTOPIDHPP

#include <Core/Util/Global.hpp>
#include <QStandardItemModel>
#include <QWidget>

namespace Ui
{
    class IVtoPID;
}

class IVtoPID : public QWidget
{
    Q_OBJECT
public:
    explicit IVtoPID(QWidget *parent = nullptr);
    ~IVtoPID() override;

private:
    Ui::IVtoPID *ui;
    QStandardItemModel *model = nullptr;

    void setupModels();
    QVector<QList<QStandardItem *>> getSeeds(u16 ivs1, u16 ivs2, u8 nature, u16 tid);
    QVector<QList<QStandardItem *>> getSeedsChannel(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature);

private slots:
    void find();
};

#endif // IVTOPID
