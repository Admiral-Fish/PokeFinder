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

#ifndef STATIONARY4_HPP
#define STATIONARY4_HPP

#include <Core/Gen4/Profile4.hpp>
#include <QMenu>

class StationaryFrame;
class StationaryGeneratorModel4;
class StationarySearcherModel4;

namespace Ui
{
    class Stationary4;
}

class Stationary4 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Stationary4(QWidget *parent = nullptr);
    ~Stationary4() override;
    void updateProfiles();

private:
    Ui::Stationary4 *ui;
    StationaryGeneratorModel4 *generatorModel = nullptr;
    StationarySearcherModel4 *searcherModel = nullptr;
    QVector<Profile4> profiles;
    Profile4 currentProfile;
    QMenu *generatorMenu = nullptr;
    QMenu *searcherMenu = nullptr;

    void setupModels();

private slots:
    void updateProgress(const QVector<StationaryFrame> &frames, int progress);
    void generate();
    void search();
    void profileIndexChanged(int index);
    void generatorLead();
    void seedToTime();
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void profileManager();
};

#endif // STATIONARY4_HPP
