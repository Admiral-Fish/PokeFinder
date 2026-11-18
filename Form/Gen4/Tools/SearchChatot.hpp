/*
 * This file is part of PokeFinder
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SEARCHCHATOT_HPP
#define SEARCHCHATOT_HPP

#include <Core/Global.hpp>
#include <QAbstractItemModel>
#include <QWidget>
#include <functional>
#include <vector>

class QLineEdit;
class QSpinBox;
class SeedToTimeCalibrateModel4;

/**
 * @brief Provides Chatot pitch controls for Gen 4 seed calibration
 */
class SearchChatot final : public QWidget
{
public:
    using ResultsCallback = std::function<void(const QModelIndexList &, int)>;

    SearchChatot(SeedToTimeCalibrateModel4 *model, ResultsCallback callback, QWidget *parent = nullptr);

    void clear();

    void remove();

    void search();

private:
    SeedToTimeCalibrateModel4 *model;
    ResultsCallback callback;
    QLineEdit *lineEdit;
    QSpinBox *startingAdvance;
    QSpinBox *maxAdvance;
    std::vector<u8> tokens;
    bool sequenceValid = true;

    void appendToken(const QString &text, u8 token);

    bool parseTokens(const QString &text);
};

#endif // SEARCHCHATOT_HPP
