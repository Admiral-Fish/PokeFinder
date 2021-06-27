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

#ifndef DREAMRADAR_HPP
#define DREAMRADAR_HPP

#include <Core/Gen5/Generators/DreamRadarGenerator.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <QMenu>

class DreamRadarGeneratorModel5;
class DreamRadarSearcherModel5;

namespace Ui
{
    class DreamRadar;
}

class DreamRadar : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit DreamRadar(QWidget *parent = nullptr);
    ~DreamRadar() override;
    void updateProfiles();
    bool hasProfiles() const;

private:
    Ui::DreamRadar *ui;
    DreamRadarGeneratorModel5 *generatorModel = nullptr;
    DreamRadarSearcherModel5 *searcherModel = nullptr;
    std::vector<Profile5> profiles;
    Profile5 currentProfile;
    QMenu *generatorMenu = nullptr;
    QMenu *searcherMenu = nullptr;

    void setupModels();

private slots:
    void generate();
    void search();
    std::vector<DreamRadarSlot> getGeneratorSettings();
    std::vector<DreamRadarSlot> getSearcherSettings();
    void profileIndexChanged(int index);
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void profileManager();
};

#endif // DREAMRADAR_HPP
