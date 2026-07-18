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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef ADVANCEFINDER_HPP
#define ADVANCEFINDER_HPP

#include <Core/Global.hpp>
#include <QDialog>
#include <vector>

class QAbstractItemModel;
class IndexFilterProxyModel;
class IRNGProvider4;
class IRNGProvider5;
class QTableView;

namespace Ui
{
    class AdvanceFinder;
}

/**
 * @brief Provides a dialog to find advances based on Chatot pitch or save needle sequences
 */
class AdvanceFinder : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new AdvanceFinder object
     *
     * @param generatorModel The generator model to search through
     * @param parent Parent widget, which takes memory ownership
     */
    AdvanceFinder(QAbstractItemModel *generatorModel, QTableView *sourceTableView, QWidget *parent = nullptr);

    /**
     * @brief Destroy the AdvanceFinder object
     */
    ~AdvanceFinder() override;

private:
    Ui::AdvanceFinder *ui;

    IndexFilterProxyModel *model;
    IRNGProvider4 *callChatot;
    IRNGProvider5 *chatotNeedle;
    QTableView *sourceTableView;
    std::vector<u8> tokens;

    /**
     * @brief Adds a token to the search sequence
     *
     * @param text Token text to append
     * @param token Token value to append
     */
    void appendToken(const QPushButton *button, u8 token);

    /**
     * @brief Clears the search sequence
     */
    void clearTokens();

    /**
     * @brief Updates display text to show the Elm E/K/P text
     */
    void elm();

    /**
     * @brief Updates display text to show the Irwin E/K/P text
     */
    void irwin();

    /**
     * @brief Removes the last token from the search sequence
     */
    void removeToken();

    /**
     * @brief Stores current sequence text for the active mode
     *
     * @param text Current sequence text
     */
    void sequenceTextChanged(const QString &text);

    /**
     * @brief Selects and scrolls to the currently previewed source advance
     */
    void jumpToAdvance();

    /**
     * @brief Searches for advances matching the current sequence
     */
    void search();

private slots:
    /**
     * @brief Handles search mode toggle
     */
    void onModeChanged();
};

#endif // ADVANCEFINDER_HPP
