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

#ifndef IDSFILTER_HPP
#define IDSFILTER_HPP

#include <Core/Parents/Filters/IDFilter.hpp>
#include <QWidget>

namespace Ui
{
    class IDsFilter;
}

/**
 * @brief Provides selection of filters for TID/SID RNG
 */
class IDsFilter : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Creates a new IDsFilter
     * @param parent Parent widget, which takes memory ownership
     */
    explicit IDsFilter(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~IDsFilter();

    /**
     * @brief Gets settings to filter by
     * @return Filter information
     */
    IDFilter getFilter() const;

    /**
     * @brief Enables of disables showing of the Display TID filter settings
     * @param flag True if enabled, false otherwise
     */
    void enableDisplayTID(bool flag);

private:
    Ui::IDsFilter *ui;

private slots:
    /**
     * @brief Validates text allowed for TID, SID, TID/SID, or Display TID
     */
    void textEditIDsTextChanged();

    /**
     * @brief Validates text allowed for TSV
     */
    void textEditTSVTextChanged();
};

#endif // IDSFILTER_HPP
