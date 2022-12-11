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

#ifndef HIDDENGROTTO_HPP
#define HIDDENGROTTO_HPP

#include <QWidget>

class HiddenGrottoArea;
class HiddenGrottoGeneratorModel5;
class HiddenGrottoSearcherModel5;
class Profile5;

namespace Ui
{
    class HiddenGrotto;
}

/**
 * @brief Provides settings and filters to RNG hidden grotto in Gen 5
 */
class HiddenGrotto : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new HiddenGrotto object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    HiddenGrotto(QWidget *parent = nullptr);

    /**
     * @brief Destroy the HiddenGrotto object
     */
    ~HiddenGrotto() override;

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
    Ui::HiddenGrotto *ui;

    std::vector<HiddenGrottoArea> encounter;
    HiddenGrottoGeneratorModel5 *generatorModel;
    HiddenGrottoSearcherModel5 *searcherModel;
    Profile5 *currentProfile;
    std::vector<Profile5> profiles;

private slots:
    /**
     * @brief Generates grotto encounters from a starting seed
     */
    void generate();

    /**
     * @brief Updates the grotto listed
     *
     * @param index Location index
     */
    void generatorLocationIndexChanged(int index);

    /**
     * @brief Updates generator filter with selected pokemon and item
     */
    void generatorUpdateFilter();

    /**
     * @brief Searches grotto encounters from date range
     */
    void search();

    /**
     * @brief Updates the grotto listed
     *
     * @param index Location index
     */
    void searcherLocationIndexChanged(int index);

    /**
     * @brief Updates searcher filter with selected pokemon and item
     */
    void searcherUpdateFilter();

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

#endif // HIDDENGROTTO_HPP
