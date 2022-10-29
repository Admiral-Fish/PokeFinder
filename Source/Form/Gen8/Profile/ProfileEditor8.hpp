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

#ifndef PROFILEEDITOR8_HPP
#define PROFILEEDITOR8_HPP

#include <QDialog>

namespace Ui
{
    class ProfileEditor8;
}

class Profile8;

/**
 * @brief Provides dialog to view/edit fields of a profile
 */
class ProfileEditor8 : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new ProfileEditor8 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    explicit ProfileEditor8(QWidget *parent = nullptr);

    /**
     * @brief Construct a new ProfileEditor8 object
     *
     * @param profile Existing profile to populate the dialog
     * @param parent Parent widget, which takes memory ownership
     */
    explicit ProfileEditor8(const Profile8 &profile, QWidget *parent = nullptr);

    /**
     * @brief Destroy the ProfileEditor8 object
     */
    ~ProfileEditor8() override;

    /**
     * @brief Creates finalized profile based on input fields
     *
     * @return Profile information
     */
    Profile8 getProfile();

private:
    Ui::ProfileEditor8 *ui;

private slots:
    /**
     * @brief Validates that a profile name exists before allowing the dialog to be closed
     */
    void okay();
};

#endif // PROFILEEDITOR8_HPP
