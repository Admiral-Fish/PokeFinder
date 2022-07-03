/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef STATIC3_H
#define STATIC3_H

#include <QWidget>

class Profile3;
class StaticGeneratorModel3;
class StaticSearcherModel3;
class QMenu;

namespace Ui
{
    class Static3;
}

class Static3 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Static3(QWidget *parent = nullptr);
    ~Static3() override;
    void updateProfiles();

private:
    Ui::Static3 *ui;
    StaticGeneratorModel3 *generatorModel;
    StaticSearcherModel3 *searcherModel;
    std::vector<Profile3> profiles;
    Profile3 *currentProfile;
    QMenu *generatorMenu;
    QMenu *searcherMenu;

    void setupModels();

private slots:
    void generate();
    void search();
    void profilesIndexChanged(int index);
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void seedToTime();
    void profileManager();
};

#endif // STATIC3_H
