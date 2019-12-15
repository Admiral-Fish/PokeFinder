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

#ifndef WILD3_HPP
#define WILD3_HPP

#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <QMenu>
#include <QModelIndex>

class Searcher3Model;
class Wild3Model;

namespace PokeFinderCore
{
    class Frame3;
}

namespace Ui
{
    class Wild3;
}

class Wild3 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Wild3(QWidget *parent = nullptr);
    ~Wild3() override;
    void updateProfiles();

private:
    Ui::Wild3 *ui;
    QVector<PokeFinderCore::Profile3> profiles;
    Searcher3Model *searcherModel {};
    Wild3Model *generatorModel {};
    QMenu *generatorMenu {};
    QMenu *searcherMenu {};
    QModelIndex lastIndex;
    QModelIndex targetFrame;
    QVector<PokeFinderCore::EncounterArea3> encounterGenerator;
    QVector<PokeFinderCore::EncounterArea3> encounterSearcher;

    void setupModels();
    void updateLocationsGenerator();
    void updateLocationsSearcher();
    void updatePokemonGenerator();
    void updatePokemonSearcher();

private slots:
    void updateProgress(const QVector<PokeFinderCore::Frame3> &frames, int progress);
    void refreshProfiles();
    void generate();
    void search();
    void profilesIndexChanged(int index);
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void setTargetFrameGenerator();
    void jumpToTargetGenerator();
    void centerFramesAndSetTargetGenerator(u32 centerFrames);
    void seedToTime();
    void copySeedToClipboard();
    void generatorLead();
    void generatorEncounterIndexChanged(int index);
    void searcherEncounterIndexChanged(int index);
    void generatorLocationIndexChanged(int index);
    void searcherLocationIndexChanged(int index);
    void generatorPokemonIndexChanged(int index);
    void searcherPokemonIndexChanged(int index);
    void profileManager();
};

#endif // WILD3_HPP
