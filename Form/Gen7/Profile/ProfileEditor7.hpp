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

#ifndef PROFILEEDITOR7_HPP
#define PROFILEEDITOR7_HPP

#include <QDialog>

namespace Ui
{
    class ProfileEditor7;
}

class Profile7;

/**
 * @brief Provides dialog to view/edit fields of a profile
 */
class ProfileEditor7 final : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new ProfileEditor7 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    ProfileEditor7(QWidget *parent = nullptr);

    /**
     * @brief Construct a new ProfileEditor7 object
     *
     * @param profile Existing profile to populate the dialog
     * @param parent Parent widget, which takes memory ownership
     */
    ProfileEditor7(const Profile7 &profile, QWidget *parent = nullptr);

    /**
     * @brief Destroy the ProfileEditor8 object
     */
    ~ProfileEditor7() override;

    /**
     * @brief Creates finalized profile based on input fields
     *
     * @return Profile information
     */
    Profile7 getProfile();

private:
    Ui::ProfileEditor7 *ui;

private slots:
    /**
     * @brief Validates that a profile name exists before allowing the dialog to be closed
     */
    void okay();
};

#endif // PROFILEEDITOR7_HPP
