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

#ifndef IVCACHEFINDER_HPP
#define IVCACHEFINDER_HPP

#include <QWidget>

namespace Ui
{
    class IVCacheFinder;
}

/**
 * @brief Provides settings to generate a IV seed cache
 */
class IVCacheFinder: public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new IVCacheFinder object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    IVCacheFinder(QWidget *parent = nullptr);

    /**
     * @brief Destroy the IVCacheFinder object
     */
    ~IVCacheFinder() override;

private:
    Ui::IVCacheFinder *ui;

private slots:
    /**
     * @brief Generates the cache file for the selected profile
     */
    void search();

    /**
     * @brief Prompts user to select a file to output the cache to
     */
    void updateOutputFile();
};

#endif // IVCACHEFINDER_HPP
