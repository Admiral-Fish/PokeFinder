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

#ifndef GAMECUBE_HPP
#define GAMECUBE_HPP

#include <Core/Gen3/Profile3.hpp>
#include <QMenu>

class GameCubeFrame;
class GameCubeGeneratorModel;
class GameCubeSearcherModel;

namespace Ui
{
    class GameCube;
}

class GameCube : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit GameCube(QWidget *parent = nullptr);
    ~GameCube() override;
    void updateProfiles();

private:
    Ui::GameCube *ui;
    GameCubeGeneratorModel *generatorModel = nullptr;
    GameCubeSearcherModel *searcherModel = nullptr;
    QVector<Profile3> profiles;
    Profile3 currentProfile;
    QMenu *generatorMenu = nullptr;
    QMenu *searcherMenu = nullptr;

    void setupModels();

private slots:
    void updateProgress(const QVector<GameCubeFrame> &frames, int progress);
    void generate();
    void search();
    void profilesIndexChanged(int index);
    void generatorMethodIndexChanged(int index);
    void generatorShadowIndexChanged(int index);
    void searcherMethodIndexChanged(int index);
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void seedToTime();
    void copySeedToClipboard();
    void profileManager();
};

#endif // GAMECUBE_HPP
