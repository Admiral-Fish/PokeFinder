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

#ifndef WILD3_HPP
#define WILD3_HPP

#include <Forms/QCheckList.hpp>
#include <Forms/QTextBox.hpp>
#include <libPokeFinder/Gen3/Frame3.hpp>
#include <libPokeFinder/Gen3/Generator3.hpp>
#include <libPokeFinder/Gen3/Searcher3.hpp>
#include <libPokeFinder/Objects/FrameCompare.hpp>
#include <libPokeFinder/Objects/Nature.hpp>
#include <libPokeFinder/Objects/Power.hpp>
#include <vector>
#include <QDir>
#include <libPokeFinder/Gen3/Profile3.hpp>
#include <Forms/ProfileManager/ProfileManager3.hpp>
#include <Forms/IVFilter.hpp>
#include <Models/Gen3/Wild3Model.hpp>
#include <Models/Gen3/Searcher3Model.hpp>
#include <thread>

namespace Ui {
class Wild3;
}

class Wild3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

signals:
    void UpdateView(vector<Frame3>);

public:
    explicit Wild3(QWidget *parent = 0);
    ~Wild3();

private slots:
    void on_generate_clicked();
    void on_saveProfileGenerator_clicked();
    void on_anyNatureGenerator_clicked();
    void on_anyHiddenPowerGenerator_clicked();
    void on_search_clicked();
    void on_saveSearcher_clicked();
    void on_anyNatureSearcher_clicked();
    void on_anyHiddenPowerSearcher_clicked();
    void updateViewSearcher(vector<Frame3> frames);
    void updateProfiles();
    void on_checkBoxDelaySearcher_clicked();
    void on_checkBoxDelayGenerator_clicked();

private:
    Ui::Wild3 *ui;
    vector<Profile3> profiles;
    bool isSearching = false;
    Searcher3Model *s = NULL;
    Wild3Model *g = NULL;

    void setupModels();
    void createProfileXml();
    void search();

};

#endif // WILD3_HPP
