/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef ADJACENTSEEDTOOL_HPP
#define ADJACENTSEEDTOOL_HPP

#include <Core/Enum/Buttons.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Tools/AdjacentSeedCalculator.hpp>
#include <Core/Util/DateTime.hpp>
#include <QWidget>
#include <vector>

class AdjacentSeedModel5;
class QModelIndex;
class QStandardItemModel;

namespace Ui
{
    class AdjacentSeedTool;
}

class AdjacentSeedTool : public QWidget
{
    Q_OBJECT
signals:
    void profilesModified(int);

public:
    AdjacentSeedTool(QWidget *parent = nullptr);
    AdjacentSeedTool(const DateTime &dateTime, Buttons buttons, AdjacentSeedMethod method = AdjacentSeedMethod::Standard,
                     QWidget *parent = nullptr);
    AdjacentSeedTool(const DateTime &dateTime, Buttons buttons, const Profile5 &profile,
                     AdjacentSeedMethod method = AdjacentSeedMethod::Standard, QWidget *parent = nullptr);
    ~AdjacentSeedTool() override;

    bool hasProfiles() const;

public slots:
    void updateProfiles();

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    Ui::AdjacentSeedTool *ui;
    AdjacentSeedModel5 *model;
    QStandardItemModel *keypressModel;
    Profile5 *currentProfile = nullptr;
    Buttons currentButtons = Buttons::None;
    std::vector<Profile5> profiles;

    Buttons getSelectedButtons() const;
    void setInitialSearch(const DateTime &dateTime, Buttons buttons);
    void setMethod(AdjacentSeedMethod method);
    void setProfile(const Profile5 &profile);
    void setSelectedButtons(Buttons buttons);

private slots:
    void generate();
    void keypressIndexPressed(const QModelIndex &index);
    void openIVCalculator();
    void profileIndexChanged(int index);
    void profileManager();
    void updateKeypressText();
    void updatePreview();
};

#endif // ADJACENTSEEDTOOL_HPP
