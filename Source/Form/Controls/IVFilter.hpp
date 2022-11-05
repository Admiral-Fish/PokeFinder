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

class QSpinBox;

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
signals:
    /**
     * @brief Emits that show stats setting has been changed
     */
    void showStatsChanged(bool);

public:
    /**
     * @brief Construct a new IVFilter object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    IVFilter(QWidget *parent = nullptr);

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
    Ui::IVFilter *ui;

private slots:
    /**
     * @brief Updates min/max values based on control keys selected
     *
     * @param minBox Spinbox that has the minimum value
     * @param maxBox Spinbox that has the maximum value
     * @param type Control keys
     */
    void changeCompare(QSpinBox *minBox, QSpinBox *maxBox, int type);

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

#endif // IVFILTER_HPP
