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

#ifndef WILD4_HPP
#define WILD4_HPP

#include <QMenu>
#include <QMutex>
#include <QFileDialog>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Generator4.hpp>
#include <Core/Gen4/IVSearcher4.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>
#include <Models/Gen4/Searcher4Model.hpp>
#include <Models/Gen4/Wild4Model.hpp>

namespace Ui
{
    class Wild4;
}

class Wild4 : public QWidget
{
    Q_OBJECT

signals:
    void alertProfiles(int);

public:
    explicit Wild4(QWidget *parent = nullptr);
    ~Wild4() override;
    void updateProfiles();

private:
    Ui::Wild4 *ui;
    QVector<Profile4> profiles;
    Searcher4Model *searcherModel;
    Wild4Model *generatorModel;
    QMenu *generatorMenu;
    QMenu *searcherMenu;
    QVector<EncounterArea4> encounterGenerator;
    QVector<EncounterArea4> encounterSearcher;

    void setupModels();
    void updateView(const QVector<Frame4> &frames, int progress);
    void updateLocationsGenerator();
    void updateLocationsSearcher();
    void updatePokemonGenerator();
    void updatePokemonSearcher();

private slots:
    void refreshProfiles();
    void on_pushButtonGenerate_clicked();
    void on_pushButtonSearch_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_pushButtonGeneratorLead_clicked();
    void on_comboBoxGeneratorEncounter_currentIndexChanged(int index);
    void on_comboBoxSearcherEncounter_currentIndexChanged(int index);
    void on_comboBoxGeneratorLocation_currentIndexChanged(int index);
    void on_comboBoxSearcherLocation_currentIndexChanged(int index);
    void on_comboBoxGeneratorPokemon_currentIndexChanged(int index);
    void on_comboBoxSearcherPokemon_currentIndexChanged(int index);
    void on_comboBoxGeneratorTime_currentIndexChanged(int index);
    void on_comboBoxSearcherTime_currentIndexChanged(int index);
    void seedToTime();
    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
    void on_pushButtonProfileManager_clicked();

};

#endif // WILD4_HPP
