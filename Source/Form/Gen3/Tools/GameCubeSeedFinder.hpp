/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef GAMECUBESEEDFINDER_HPP
#define GAMECUBESEEDFINDER_HPP

#include <Core/Global.hpp>
#include <QWidget>

namespace Ui
{
    class GameCubeSeedFinder;
}

/**
 * @brief Provides settings to determine current PRNG state in GameCube games
 */
class GameCubeSeedFinder : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new GameCubeSeedFinder object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    GameCubeSeedFinder(QWidget *parent = nullptr);

    /**
     * @brief Destroy the GameCubeSeedFinder object
     */
    ~GameCubeSeedFinder() override;

private:
    Ui::GameCubeSeedFinder *ui;

    QString coloPath;
    QString galesPath;
    std::vector<u32> coloSeeds;
    std::vector<u32> galeSeeds;
    u8 coloRound;
    u8 galesRound;

    /**
     * @brief Shows user with potentional candidate \p seeds
     *
     * @param seeds Candidate PRNG states
     */
    void updateChannel(const std::vector<u32> &seeds);

    /**
     * @brief Updates the progres bar of the Channel search
     *
     * @param progress Current progress
     */
    void updateChannelProgress(int progress);

    /**
     * @brief Updates potentional candidate seeds
     *
     * @param seeds Candidate PRNG states
     */
    void updateColo(const std::vector<u32> &seeds);

    /**
     * @brief Updates the progres bar of the Colo search
     *
     * @param progress Current progress
     */
    void updateColoProgress(int progress);

    /**
     * @brief Updates potentional candidate seeds
     *
     * @param seeds Candidate PRNG states
     */
    void updateGales(const std::vector<u32> &seeds);

    /**
     * @brief Updates the progres bar of the Gales search
     *
     * @param progress Current progress
     */
    void updateGalesProgress(int progress);

private slots:
    /**
     * @brief Adds the selected Channel pattern
     */
    void channelAdd();

    /**
     * @brief Removes the last Channel pattern
     */
    void channelRemove();

    /**
     * @brief Searches for Channel candidate PRNG states
     */
    void channelSearch();

    /**
     * @brief Clears candidate PRNG states and resets for searching from round 1
     */
    void coloReset();

    /**
     * @brief Searches for Colo candidate PRNG states
     */
    void coloSearch();

    /**
     * @brief Clears candidate PRNG states and resets for searching from round 1
     */
    void galesReset();

    /**
     * @brief Searches for Gales candidate PRNG states
     */
    void galesSearch();
};

#endif // GAMECUBESEEDFINDER_HPP
