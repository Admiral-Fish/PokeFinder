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

#ifndef STATIC4_HPP
#define STATIC4_HPP

#include <Core/Gen4/Profile4.hpp>
#include <QWidget>

class StaticGeneratorModel4;
class StaticSearcherModel4;
class QMenu;

namespace Ui
{
    class Static4;
}

class Static4 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    explicit Static4(QWidget *parent = nullptr);
    ~Static4() override;
    void updateProfiles();

private:
    Ui::Static4 *ui;
    StaticGeneratorModel4 *generatorModel;
    StaticSearcherModel4 *searcherModel;
    std::vector<Profile4> profiles;
    Profile4 currentProfile;
    QMenu *generatorMenu;
    QMenu *searcherMenu;

    void setupModels();

private slots:
    void generate();
    void search();
    void profileIndexChanged(int index);
    void seedToTime();
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void profileManager();
};

#endif // STATIC4_HPP
