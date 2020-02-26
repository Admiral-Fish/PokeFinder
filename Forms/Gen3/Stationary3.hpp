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

#ifndef STATIONARY3_H
#define STATIONARY3_H

#include <Core/Gen3/Profile3.hpp>
#include <QMenu>

class Frame;
class StationaryGeneratorModel3;
class StationarySearcherModel3;

namespace Ui
{
    class Stationary3;
}

class Stationary3 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Stationary3(QWidget *parent = nullptr);
    ~Stationary3() override;
    void updateProfiles();

private:
    Ui::Stationary3 *ui;
    StationaryGeneratorModel3 *generatorModel = nullptr;
    StationarySearcherModel3 *searcherModel = nullptr;
    QVector<Profile3> profiles;
    Profile3 currentProfile;
    QMenu *generatorMenu = nullptr;
    QMenu *searcherMenu = nullptr;

    void setupModels();

private slots:
    void updateProgress(const QVector<Frame> &frames, int progress);
    void generate();
    void search();
    void profilesIndexChanged(int index);
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void seedToTime();
    void copySeedToClipboard();
    void profileManager();
};

#endif // STATIONARY3_H
