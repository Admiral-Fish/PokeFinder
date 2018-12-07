/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef STATIONARY4_HPP
#define STATIONARY4_HPP

#include <QMainWindow>
#include <QMenu>
#include <QFileDialog>
#include <QSettings>
#include <thread>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>
#include <Models/Gen4/Searcher4Model.hpp>
#include <Models/Gen4/Stationary4Model.hpp>
#include <Gen4/Generator4.hpp>
#include <Gen4/Searcher4.hpp>
#include <Translator.hpp>

namespace Ui
{
    class Stationary4;
}

class Stationary4 : public QMainWindow
{
    Q_OBJECT

signals:
    void alertProfiles(int);
    void updateView(QVector<Frame4>);
    void updateProgress();

private:
    Ui::Stationary4 *ui;
    Searcher4Model *s = new Searcher4Model(this, Method::Method1);
    Stationary4Model *g = new Stationary4Model(this, Method::Method1);
    bool isSearching = false;
    bool cancel = false;
    int progress;
    QVector<Profile4> profiles;
    QMenu *searcherMenu = new QMenu(this);

    void loadSettings();
    void saveSettings();
    void search();
    void setupModels();
    void updateSearch();

private slots:
    void on_generate_clicked();
    void refreshProfiles();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_anyNatureGenerator_clicked();
    void on_anyHiddenPowerGenerator_clicked();
    void on_search_clicked();
    void on_anyNatureSearcher_clicked();
    void on_anyHiddenPowerSearcher_clicked();
    void on_pushButtonProfileManager_clicked();
    void updateProgressBar();
    void updateViewSearcher(const QVector<Frame4> &frames);
    void on_pushButtonLeadGenerator_clicked();
    void seedToTime();
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);

public:
    explicit Stationary4(QWidget *parent = nullptr);
    ~Stationary4() override;
    void updateProfiles();
};

#endif // STATIONARY4_HPP
