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

#ifndef RESEARCHER_HPP
#define RESEARCHER_HPP

#include <Core/Global.hpp>
#include <QWidget>

class ResearcherState;
class ResearcherModel;
enum class Custom : u8;

namespace Ui
{
    class Researcher;
}

/**
 * @brief Allows generation from PRNG states from different RNGs and performing custom calculations/searches on them.
 */
class Researcher : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new Researcher object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Researcher(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Researcher object
     */
    ~Researcher() override;

private:
    Ui::Researcher *ui;

    ResearcherModel *model;

    /**
     * @brief Gets the \p custom value from the current \p state or \p states
     *
     * @param custom Custom value to grab
     * @param state Current researcher state
     * @param states Vector of researcher states
     *
     * @return Calculated custom value
     */
    u64 getCustom(Custom custom, const ResearcherState &state, const std::vector<ResearcherState> &states);

    /**
     * @brief Returns which hex checkboxes are checked
     *
     * @return Vector of true/false for whether the hex checkbox is checked
     */
    std::array<bool, 10> getHexCheck();

private slots:
    /**
     * @brief Generates the PRNG states for the selected RNG
     */
    void generate();

    /**
     * @brief Searches for the next occurence of the specified value from where the current index in the tableview is
     */
    void next();

    /**
     * @brief Searches for the first occurence of the specified value
     */
    void search();

    /**
     * @brief Updates which customs are show based on the RNG being 32bit or 64bit
     *
     * @param index RNG index
     */
    void rngSelectionIndexChanged(int index);
};

#endif // RESEARCHER_HPP
