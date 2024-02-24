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

#ifndef DREAMRADAR_HPP
#define DREAMRADAR_HPP

#include <QWidget>

class ComboBox;
class DreamRadarGeneratorModel5;
class DreamRadarSearcherModel5;
class DreamRadarTemplate;
class Profile5;

namespace Ui
{
    class DreamRadar;
}

/**
 * @brief Provides settings and filters to RNG dream radar encounters in Gen 5 games
 */
class DreamRadar : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new DreamRadar object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    DreamRadar(QWidget *parent = nullptr);

    /**
     * @brief Destroy the DreamRadar object
     */
    ~DreamRadar() override;

    /**
     * @brief Determines if any profiles exist
     *
     * @return true At least 1 profile exists
     * @return false 0 profiles exist
     */
    bool hasProfiles() const;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::DreamRadar *ui;

    DreamRadarGeneratorModel5 *generatorModel;
    DreamRadarSearcherModel5 *searcherModel;
    Profile5 *currentProfile;
    std::vector<Profile5> profiles;

    /**
     * @brief Loads UI settings for dream radar templates
     *
     * @return Dream radar templates
     */
    std::vector<DreamRadarTemplate> getGeneratorSettings() const;

    /**
     * @brief Loads UI settings for dream radar templates
     *
     * @return Dream radar templates
     */
    std::vector<DreamRadarTemplate> getSearcherSettings() const;

    /**
     * @brief Updates available genders for the select Pokemon
     *
     * @param comboBoxSpecie Pokemon specie combo box
     * @param comboBoxGender Pokemon gender combo box
     */
    void updateGenders(ComboBox *comboBoxSpecie, ComboBox *comboBoxGender);

private slots:
    /**
     * @brief Generates static encounters from a starting seed
     */
    void generate();

    /**
     * @brief Searches static encounters from date range
     */
    void search();

    /**
     * @brief Updates displayed information for a profile
     *
     * @param index Profile index
     */
    void profileIndexChanged(int index);

    /**
     * @brief Opens the profile manager
     */
    void profileManager();
};

#endif // DREAMRADAR_HPP
