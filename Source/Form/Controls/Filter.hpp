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

#ifndef FILTER_HPP
#define FILTER_HPP

#include <Core/Global.hpp>
#include <QWidget>

enum class Controls : u16;

namespace Ui
{
    class Filter;
}

/**
 * @brief Provides settings to filter results on
 */
class Filter : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Creates a new Filter
     * @param parent Parent widget, which takes memory ownership
     */
    explicit Filter(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~Filter() override;

    /**
     * @brief Gets lower bound IVs to filter by
     * @return Array of minimum IVs
     */
    std::array<u8, 6> getMinIVs() const;

    /**
     * @brief Gets upper bound IVs to filter by
     * @return Array of maximum IVs
     */
    std::array<u8, 6> getMaxIVs() const;

    /**
     * @brief Gets ability to filter by
     * @return Ability value
     */
    u8 getAbility() const;

    /**
     * @brief Gets gender to filter by
     * @return Gender value
     */
    u8 getGender() const;

    /**
     * @brief Gets encounter slots to filter by
     * @return Vector of encounter slots
     */
    std::vector<bool> getEncounterSlots();

    /**
     * @brief Sets encounter slots that are available
     * @param encounterSlots Vector of encounter slots
     */
    void setEncounterSlots(const std::vector<std::string> &encounterSlots) const;

    /**
     * @brief Sets encounter slots that are checked and not checked
     * @param encounterSlots Vector of encounter slots
     */
    void toggleEncounterSlots(const std::vector<bool> &encounterSlots) const;

    /**
     * @brief Unchecks all encounter slots
     */
    void resetEncounterSlots() const;

    /**
     * @brief Gets hidden powers to filter by
     * @return Vector of hidden powers
     */
    std::vector<bool> getHiddenPowers();

    /**
     * @brief Get natures to filter by
     * @return Vector of natures
     */
    std::vector<bool> getNatures();

    /**
     * @brief Gets shiny status to filter by
     * @return Shiny value
     */
    u8 getShiny() const;

    /**
     * @brief Gets if delay should be added
     * @return True if delay is enabled, false otherwise
     */
    bool useDelay() const;

    /**
     * @brief Gets delay to add
     * @return Delay value
     */
    u32 getDelay() const;

    /**
     * @brief Checks if filters should be ignored
     * @return True if filters should be disabled, false otherwise
     */
    bool getDisableFilters() const;

    /**
     * @brief Enables specified controls
     * @param control Controls value
     */
    void enableControls(Controls control);

    /**
     * @brief Disables specified controls
     * @param control Controls value
     */
    void disableControls(Controls control);

    /**
     * @brief Enables showing the hidden ability
     */
    void enableHiddenAbility();

private:
    Ui::Filter *ui;
};

#endif // FILTER_HPP
