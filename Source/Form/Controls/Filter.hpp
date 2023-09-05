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

#ifndef FILTER_HPP
#define FILTER_HPP

#include <Core/Global.hpp>
#include <QWidget>

enum class Controls : u16;
class QSpinBox;

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
signals:
    /**
     * @brief Emits that show stats setting has been changed
     */
    void showStatsChanged(bool);

    /**
     * @brief Emits that IV filter has been changed
     */
    void ivsChanged();

public:
    /**
     * @brief Construct a new Filter object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Filter(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Filter object
     */
    ~Filter() override;

    /**
     * @brief Disables specified controls
     *
     * @param control Controls value
     */
    void disableControls(Controls control);

    /**
     * @brief Enables showing the hidden ability
     */
    void enableHiddenAbility();

    /**
     * @brief Gets ability to filter by
     *
     * @return Ability value
     */
    u8 getAbility() const;

    /**
     * @brief Checks if filters should be ignored
     *
     * @return true Filters are disabled
     * @return false Filters are not disabled
     */
    bool getDisableFilters() const;

    /**
     * @brief Gets encounter slots to filter by
     *
     * @return Array of encounter slots
     */
    std::array<bool, 12> getEncounterSlots() const;

    /**
     * @brief Constructs filter from the UI settings
     *
     * @tparam FilterType Filter class type
     * @tparam wild Whether filter is for wild encounters
     *
     * @return Filter object
     */
    template <class FilterType, bool wild = false>
    FilterType getFilter() const
    {
        if constexpr (wild)
        {
            return FilterType(getGender(), getAbility(), getShiny(), getDisableFilters(), getMinIVs(), getMaxIVs(), getNatures(),
                              getHiddenPowers(), getEncounterSlots());
        }
        else
        {
            return FilterType(getGender(), getAbility(), getShiny(), getDisableFilters(), getMinIVs(), getMaxIVs(), getNatures(),
                              getHiddenPowers());
        }
    }

    /**
     * @brief Gets gender to filter by
     *
     * @return Gender value
     */
    u8 getGender() const;

    /**
     * @brief Gets hidden powers to filter by
     *
     * @return Array of hidden powers
     */
    std::array<bool, 16> getHiddenPowers() const;

    /**
     * @brief Gets upper bound IVs to filter by
     *
     * @return Array of maximum IVs
     */
    std::array<u8, 6> getMaxIVs() const;

    /**
     * @brief Gets lower bound IVs to filter by
     *
     * @return Array of minimum IVs
     */
    std::array<u8, 6> getMinIVs() const;

    /**
     * @brief Get natures to filter by
     *
     * @return Array of natures
     */
    std::array<bool, 25> getNatures() const;

    /**
     * @brief Gets shiny status to filter by
     *
     * @return Shiny value
     */
    u8 getShiny() const;

    /**
     * @brief Unchecks all encounter slots
     */
    void resetEncounterSlots() const;

    /**
     * @brief Sets encounter slots that are available
     *
     * @param max Number of encounter slots
     */
    void setEncounterSlots(u8 max) const;

    /**
     * @brief Sets encounter slots that are checked and not checked
     *
     * @param encounterSlots Vector of encounter slots
     */
    void toggleEncounterSlots(const std::vector<bool> &encounterSlots) const;

protected:
    /**
     * @brief Shows the combo box model when clicked
     *
     * @param object Object that is part of triggered event
     * @param event Contains information about the triggered event
     *
     * @return true Model should be shown
     * @return false Model should not be shown
     */
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    Ui::Filter *ui;

private slots:
    /**
     * @brief Opens IV calculator to determine IV minimum/maximum
     */
    void openIVCalculator() const;

    /**
     * @brief Updates min/max IV values based upon calculation from IV Calculator
     *
     * @param ivs Possible IV ranges
     */
    void updateIVs(const std::array<std::vector<u8>, 6> &ivs);
};

#endif // FILTER_HPP
