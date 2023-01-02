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

#ifndef PROFILEMANAGER8_HPP
#define PROFILEMANAGER8_HPP

#include <QWidget>

class ProfileModel8;

namespace Ui
{
    class ProfileManager8;
}

/**
 * @brief Provides interface to view/edit/delete existing profiles and create new ones
 */
class ProfileManager8 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new ProfileManager8 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    ProfileManager8(QWidget *parent = nullptr);

    /**
     * @brief Destroy the ProfileManager3 object
     */
    ~ProfileManager8() override;

private:
    Ui::ProfileManager8 *ui;

    ProfileModel8 *model;

private slots:
    /**
     * @brief Opens dialog to create a new profile
     */
    void create();

    /**
     * @brief Opens dialog to edit an existing profile
     */
    void edit();

    /**
     * @brief Opens dialog to confirm deletion of existing profile
     */
    void remove();
};

#endif // PROFILEMANAGER8_HPP
