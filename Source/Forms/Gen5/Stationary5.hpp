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

#ifndef STATIONARY5_HPP
#define STATIONARY5_HPP

#include <Core/Gen5/Profile5.hpp>
#include <QMenu>

class StationaryGeneratorModel5;
class StationarySearcherModel5;

namespace Ui
{
    class Stationary5;
}

class Stationary5 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Stationary5(QWidget *parent = nullptr);
    ~Stationary5() override;
    void updateProfiles();
    bool hasProfiles() const;

private:
    Ui::Stationary5 *ui;
    StationaryGeneratorModel5 *generatorModel = nullptr;
    StationarySearcherModel5 *searcherModel = nullptr;
    std::vector<Profile5> profiles;
    Profile5 currentProfile;
    QMenu *generatorMenu = nullptr;
    QMenu *searcherMenu = nullptr;

    void setupModels();

private slots:
    void generate();
    void search();
    void profileIndexChanged(int index);
    void generatorLead();
    void generatorMethodIndexChanged(int index);
    void searcherMethodIndexChanged(int index);
    void tableViewGeneratorContextMenu(const QPoint &pos);
    void tableViewSearcherContextMenu(const QPoint &pos);
    void profileManager();
};

#endif // STATIONARY5_HPP
