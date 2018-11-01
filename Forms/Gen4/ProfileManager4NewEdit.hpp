/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef PROFILEMANAGER4NEWEDIT_HPP
#define PROFILEMANAGER4NEWEDIT_HPP

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <PokeFinderCore/Gen4/Profile4.hpp>

using u32 = uint32_t;

namespace Ui
{
    class ProfileManager4NewEdit;
}

class ProfileManager4NewEdit : public QDialog
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *event) override;

signals:
    void newProfile(Profile4);
    void editProfile(Profile4, Profile4);

private:
    Ui::ProfileManager4NewEdit *ui;
    bool isEditing = false;
    Profile4 original;
    Profile4 fresh;

    void setupModels();

private slots:
    void on_pushButtonAccept_clicked();
    void on_pushButtonCancel_clicked();
    void on_comboBoxVersion_currentIndexChanged(int index);

public:
    explicit ProfileManager4NewEdit(QWidget *parent = nullptr);
    explicit ProfileManager4NewEdit(Profile4 profile, QWidget *parent = nullptr);
    ~ProfileManager4NewEdit() override;
    Profile4 getNewProfile();
    Profile4 getOriginal();

};

#endif // PROFILEMANAGER4NEWEDIT_HPP
