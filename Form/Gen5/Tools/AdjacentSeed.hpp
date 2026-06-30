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

#ifndef ADJACENTSEED_HPP
#define ADJACENTSEED_HPP

#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Tools/AdjacentSeedCalculator.hpp>
#include <QWidget>
#include <vector>

class AdjacentSeedModel;
class DateTime;
class QStandardItemModel;

enum class Button : u16;

namespace Ui
{
    class AdjacentSeed;
}

class AdjacentSeed : public QWidget
{
    Q_OBJECT
signals:
    void profilesModified(int);

public:
    AdjacentSeed(QWidget *parent = nullptr);
    AdjacentSeed(const DateTime &dateTime, Buttons buttons, AdjacentSeedMethod method = AdjacentSeedMethod::Standard,
                     QWidget *parent = nullptr);
    AdjacentSeed(const DateTime &dateTime, Buttons buttons, const Profile5 &profile,
                     AdjacentSeedMethod method = AdjacentSeedMethod::Standard, QWidget *parent = nullptr);
    ~AdjacentSeed() override;

    bool hasProfiles() const;

public slots:
    void updateProfiles();

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    Ui::AdjacentSeed *ui;
    AdjacentSeedModel *model;
    QStandardItemModel *keypressModel;
    Profile5 *currentProfile;
    Buttons currentButtons;
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

#endif // ADJACENTSEED_HPP
