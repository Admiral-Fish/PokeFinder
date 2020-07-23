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

#ifndef EGGS5_HPP
#define EGGS5_HPP

#include <Core/Gen5/Profile5.hpp>
#include <QMenu>

class EggGeneratorModel5;
class EggSearcherModel5;
class EggState;
template <typename StateType>
class SearcherState5;

namespace Ui
{
    class Eggs5;
}

class Eggs5 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Eggs5(QWidget *parent = nullptr);
    ~Eggs5() override;
    void updateProfiles();
    bool hasProfiles() const;

private:
    Ui::Eggs5 *ui;
    EggGeneratorModel5 *generatorModel = nullptr;
    EggSearcherModel5 *searcherModel = nullptr;
    QVector<Profile5> profiles;
    Profile5 currentProfile;
    QMenu *generatorMenu = nullptr;
    QMenu *searcherMenu = nullptr;

    void setupModels();

private slots:
    void updateProgress(const QVector<SearcherState5<EggState>> &states, int progress);
    void generate();
    void search();
    void calculateInitialAdvances();
    void profileIndexChanged(int index);
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void profileManager();
};

#endif // EGGS5_HPP
