/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef UNOWN3_HPP
#define UNOWN3_HPP

#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <QMenu>

class UnownGeneratorModel3;
class UnownSearcherModel3;

namespace Ui
{
    class Unown3;
}

class Unown3 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Unown3(QWidget *parent = nullptr);
    ~Unown3() override;
    void updateProfiles();

private:
    Ui::Unown3 *ui;
    std::vector<Profile3> profiles;
    Profile3 currentProfile;
    UnownGeneratorModel3 *generatorModel = nullptr;
    UnownSearcherModel3 *searcherModel = nullptr;
    QMenu *generatorMenu = nullptr;
    QMenu *searcherMenu = nullptr;
    std::vector<EncounterArea3> encounterGenerator;
    std::vector<EncounterArea3> encounterSearcher;

    void setupModels();
    void updateLocationsGenerator();
    void updateLocationsSearcher();

private slots:
    void generate();
    void search();
    void profilesIndexChanged(int index);
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void seedToTime();
    void generatorLocationIndexChanged(int index);
    void searcherLocationIndexChanged(int index);
    void profileManager();
};

#endif // UNOWN3_HPP
