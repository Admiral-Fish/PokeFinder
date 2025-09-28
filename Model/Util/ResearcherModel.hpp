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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef RESEARCHERMODEL_HPP
#define RESEARCHERMODEL_HPP

#include <Core/Parents/States/ResearcherState.hpp>
#include <Model/TableModel.hpp>
#include <array>

/**
 * @brief Enum to encompass researcher computations
 */
enum class Custom : u8
{
    None,
    Full64Bit,
    Full32Bit,
    High32Bit,
    Low32Bit,
    High16Bit,
    Low16Bit,
    PreviousFull64Bit,
    PreviousFull32Bit,
    PreviousHigh32Bit,
    PreviousLow32Bit,
    PreviousHigh16Bit,
    PreviousLow16Bit,
    Custom1,
    Custom2,
    Custom3,
    Custom4,
    Custom5,
    Custom6,
    Custom7,
    Custom8,
    Custom9,
    Custom10,
    Previous1,
    Previous2,
    Previous3,
    Previous4,
    Previous5,
    Previous6,
    Previous7,
    Previous8,
    Previous9,
    Previous10
};

/**
 * @brief Converts enum to number
 *
 * @param game Input game
 *
 * @return Converted number
 */
constexpr u8 toInt(Custom custom)
{
    return static_cast<u8>(custom);
}

/**
 * @brief Provides a table model implementation to show PRNG information for Researcher
 */
class ResearcherModel : public TableModel<ResearcherState>
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new ResearcherModel object
     *
     * @param parent Parent object, which takes memory ownership
     * @param flag Whether or not the model is in 64bit or 32bit mode
     */
    ResearcherModel(QObject *parent, bool flag);

    /**
     * @brief Returns the number of columns in the model
     *
     * @param parent Unused parent index
     *
     * @return Number of columns
     */
    int columnCount(const QModelIndex &index = QModelIndex()) const override;

    /**
     * @brief Returns data at the \p index with \p role
     *
     * @param index Row/column index
     * @param role Model data role
     *
     * @return Data at index
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Returns header text at the \p section, \p orientation, and \p role
     *
     * @param section Column index
     * @param orientation Header position
     * @param role Model data role
     *
     * @return Header text at column
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    /**
     * @brief Search for the column that has the given data
     *
     * @param custom Determine which function pointer to search with
     * @param result Number to search
     * @param row Row index to search
     *
     * @return Index of found data
     */
    QModelIndex search(Custom custom, u64 result, int row);

    /**
     * @brief Sets the 64bit RNG flag
     *
     * @param flag True if 64bit RNG, false otherwise
     */
    void setFlag(bool flag);

    /**
     * @brief Sets which customs should be displayed in hexadecimal
     *
     * @param hex Vector of true/false for which customs to display as hexadecimal
     */
    void setHex(const std::array<bool, 10> &hex);

private:
    bool flag;
    std::array<bool, 10> hex;

    QStringList header = { tr("Advances"),  tr("64Bit"),    tr("32Bit High"), tr("32Bit Low"), tr("32Bit"),    tr("16Bit High"),
                           tr("16Bit Low"), tr("Custom 1"), tr("Custom 2"),   tr("Custom 3"),  tr("Custom 4"), tr("Custom 5"),
                           tr("Custom 6"),  tr("Custom 7"), tr("Custom 8"),   tr("Custom 9"),  tr("Custom 10") };

    /**
     * @brief Adjusts column based on the 64bit RNG flag
     *
     * @param column Original column
     *
     * @return Adjusted column
     */
    int getColumn(int column) const;
};

#endif // RESEARCHERMODEL_HPP
