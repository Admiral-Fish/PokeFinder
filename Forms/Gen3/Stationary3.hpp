/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#ifndef Generator_H
#define Generator_H

#include <Forms/QCheckList.hpp>
#include <Forms/QTextBox.hpp>
#include <libPokeFinder/Gen3/Frame3.hpp>
#include <libPokeFinder/Gen3/Generator3.hpp>
#include <libPokeFinder/Gen3/Searcher3.hpp>
#include <libPokeFinder/Objects/FrameCompare.hpp>
#include <libPokeFinder/Objects/Nature.hpp>
#include <libPokeFinder/Objects/Power.hpp>
#include <QDir>
#include <libPokeFinder/Gen3/Profile3.hpp>
#include <Forms/ProfileManager/ProfileManager3.hpp>
#include <Forms/IVFilter.hpp>
#include <Models/Gen3/Stationary3Model.hpp>
#include <Models/Gen3/Searcher3Model.hpp>
#include <thread>

namespace Ui {
class Stationary3;
}

class Stationary3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

signals:
    void updateView(vector<Frame3>);

private:
    Ui::Stationary3 *ui;
    Searcher3Model *s = NULL;
    Stationary3Model *g = NULL;
    bool isSearching = false;
    vector<Profile3> profiles;

    void setupModels();
    void createProfileXml();
    void search();

private slots:
    void on_generate_clicked();
    void updateProfiles();
    void on_saveProfileGenerator_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_anyNatureGenerator_clicked();
    void on_anyHiddenPowerGenerator_clicked();
    void on_checkBoxDelayGenerator_clicked();
    void on_saveSearcher_clicked();
    void on_search_clicked();
    void on_anyNatureSearcher_clicked();
    void on_anyHiddenPowerSearcher_clicked();
    void updateViewSearcher(vector<Frame3> frames);
    void on_checkBoxDelaySearcher_clicked();
    void on_comboBoxMethodSearcher_currentIndexChanged(int index);

public:
    explicit Stationary3(QWidget *parent = 0);
    ~Stationary3();

};

#endif // Generator_H
