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

#ifndef HIDDENGROTTO_HPP
#define HIDDENGROTTO_HPP

#include <Core/Gen5/Profile5.hpp>
#include <QWidget>

class HiddenGrottoGeneratorModel5;
class HiddenGrottoSearcherModel5;
class QMenu;

namespace Ui
{
    class HiddenGrotto;
}

class HiddenGrotto : public QWidget
{
    Q_OBJECT

signals:
    void alertProfiles(int);

public:
    explicit HiddenGrotto(QWidget *parent = nullptr);
    ~HiddenGrotto() override;
    void updateProfiles();
    bool hasProfiles() const;

private:
    Ui::HiddenGrotto *ui;
    HiddenGrottoGeneratorModel5 *generatorModel;
    HiddenGrottoSearcherModel5 *searcherModel;
    std::vector<Profile5> profiles;
    Profile5 currentProfile;
    QMenu *generatorMenu;
    QMenu *searcherMenu;

    void setupModels();

private slots:
    void generate();
    void search();
    void profileIndexChanged(int index);
    void calculateInitialAdvances();
    void tableViewGeneratorContextMenu(QPoint pos);
    void tableViewSearcherContextMenu(QPoint pos);
    void profileManager();
};

#endif // HIDDENGROTTO_HPP
