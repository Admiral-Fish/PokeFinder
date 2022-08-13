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

#ifndef IVFILTER_HPP
#define IVFILTER_HPP

#include <Core/Global.hpp>
#include <QWidget>

namespace Ui
{
    class IVFilter;
}

/**
 * @brief Provides selection of filters for IVs
 */
class IVFilter : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new IVFilter object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    explicit IVFilter(QWidget *parent = nullptr);

    /**
     * @brief Destroy the IVFilter object
     */
    ~IVFilter() override;

    /**
     * @brief Gets lower bound IVs to filter by
     *
     * @return Array of minimum IVs
     */
    std::array<u8, 6> getLower() const;

    /**
     * @brief Gets upper bound IVs to filter by
     *
     * @return Array of maximum IVs
     */
    std::array<u8, 6> getUpper() const;

private:
    Ui::IVFilter *ui;

private slots:
    /**
     * @brief Updates min/max HP based on control keys selected
     *
     * @param type Control keys
     */
    void changeCompareHP(int type);

    /**
     * @brief Updates min/max Atk based on control keys selected
     *
     * @param type Control keys
     */
    void changeCompareAtk(int type);

    /**
     * @brief Updates min/max Def based on control keys selected
     *
     * @param type Control keys
     */
    void changeCompareDef(int type);

    /**
     * @brief Updates min/max SpA based on control keys selected
     *
     * @param type Control keys
     */
    void changeCompareSpA(int type);

    /**
     * @brief Updates min/max SpD based on control keys selected
     *
     * @param type Control keys
     */
    void changeCompareSpD(int type);

    /**
     * @brief Updates min/max Spe based on control keys selected
     *
     * @param type Control keys
     */
    void changeCompareSpe(int type);

    /**
     * @brief Opens IV calculator to determine IV minimum/maximum
     */
    void openIVCalculator();

    /**
     * @brief Updates min/max IV values based upon calculation from IV Calculator
     *
     * @param ivs Possible IV ranges
     */
    void updateIVs(const std::array<std::vector<u8>, 6> &ivs);
};

#endif // IVFILTER_HPP
