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

#ifndef GAMECUBERTC_HPP
#define GAMECUBERTC_HPP

#include <Core/Util/Global.hpp>
#include <QMenu>

class GameCubeRTCFrame;
class GameCubeRTCModel;

namespace Ui
{
    class GameCubeRTC;
}

class GameCubeRTC : public QWidget
{
    Q_OBJECT
public:
    explicit GameCubeRTC(QWidget *parent = nullptr);
    explicit GameCubeRTC(u32 seed, QWidget *parent = nullptr);
    ~GameCubeRTC() override;

private:
    Ui::GameCubeRTC *ui;
    GameCubeRTCModel *model = nullptr;
    QMenu *contextMenu = nullptr;

    void setupModels();

private slots:
    void search();
    void updateTableView(const QVector<GameCubeRTCFrame> &results);
    void copySeed();
    void tableViewContextMenu(QPoint pos);
};

#endif // GAMECUBERTC_HPP
