/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QWidget>

namespace Ui
{
    class Settings;
}

/**
 * @brief Provides an interface to view/modify application settings
 */
class Settings : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new Settings object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Settings(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Settings object
     */
    ~Settings() override;

private:
    Ui::Settings *ui;

private slots:
    /**
     * @brief Handles updating the path to the Gen5 IV bin file
     */
    void changeIVs();

    /**
     * @brief Handles updating the path to the profiles file
     */
    void changeProfiles();

    /**
     * @brief Updates the saved language settings
     *
     * @param index Language index
     */
    void languageIndexChanged(int index);

    /**
     * @brief Updates the saved style settings
     *
     * @param index Style index
     */
    void styleIndexChanged(int index);

    /**
     * @brief Updates the saved thread settings
     *
     * @param index Threads index
     */
    void threadsIndexChanged(int index);
};

#endif // SETTINGS_HPP
