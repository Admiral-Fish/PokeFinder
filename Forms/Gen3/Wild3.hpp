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

#include <QMenu>
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Models/Gen3/Searcher3Model.hpp>
#include <Models/Gen3/Wild3Model.hpp>

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
    QVector<Profile3> profiles;
    Searcher3Model *searcherModel{};
    Wild3Model *generatorModel{};
    QMenu *generatorMenu{};
    QMenu *searcherMenu{};
    QModelIndex lastIndex;
    QModelIndex targetFrame;
    QVector<EncounterArea3> encounterGenerator;
    QVector<EncounterArea3> encounterSearcher;

    void setupModels();
    void updateLocationsGenerator();
    void updateLocationsSearcher();
    void updatePokemonGenerator();
    void updatePokemonSearcher();

private slots:
    void updateProgress(const QVector<Frame3> &frames, int progress);
    void refreshProfiles();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_pushButtonGenerate_clicked();
    void on_pushButtonSearch_clicked();
    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
    void setTargetFrameGenerator();
    void jumpToTargetGenerator();
    void centerFramesAndSetTargetGenerator(u32 centerFrames);
    void seedToTime();
    void copySeedToClipboard();
    void on_pushButtonGeneratorLead_clicked();
    void on_comboBoxGeneratorEncounter_currentIndexChanged(int index);
    void on_comboBoxSearcherEncounter_currentIndexChanged(int index);
    void on_comboBoxGeneratorLocation_currentIndexChanged(int index);
    void on_comboBoxSearcherLocation_currentIndexChanged(int index);
    void on_comboBoxGeneratorPokemon_currentIndexChanged(int index);
    void on_comboBoxSearcherPokemon_currentIndexChanged(int index);
    void on_pushButtonProfileManager_clicked();

};

#endif // WILD3_HPP
