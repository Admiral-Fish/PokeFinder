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

#ifndef JIRACHIADVANCER_HPP
#define JIRACHIADVANCER_HPP

#include <Core/Util/Global.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <QWidget>

class QStandardItemModel;

namespace Ui
{
    class JirachiAdvancer;
}

class JirachiAdvancer : public QWidget
{
    Q_OBJECT
public:
    explicit JirachiAdvancer(QWidget *parent = nullptr);
    ~JirachiAdvancer() override;

private:
    Ui::JirachiAdvancer *ui;
    QStandardItemModel *model = nullptr;
    bool validateJirachi(u32 seed);
    bool validateMenu(u32 seed);
    void advanceMenu(XDRNG &rng, u32 &count);
    void advanceJirachi(XDRNG &rng, u32 &count);
    void advanceCutscene(XDRNG &rng, u32 &count);
    void advanceTitleScreen(XDRNG &rng, u32 &count);
    u32 calculateFrame(u32 currentSeed, u32 targetSeed);
    std::vector<u8> calculateActions(u32 currentSeed, u32 targetFrame, u32 bruteForce);
    void run();
    void minClicked(bool toggled);
};

#endif // JIRACHIADVANCER_HPP
