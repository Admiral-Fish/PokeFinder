/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#ifndef PROFILEMANAGER3NEWEDIT_HPP
#define PROFILEMANAGER3NEWEDIT_HPP

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QObject>
#include <QStandardItemModel>
#include <QList>
#include <PokeFinderCore/Gen3/Profile3.hpp>

typedef uint32_t u32;

namespace Ui
{
    class ProfileManager3NewEdit;
}

class ProfileManager3NewEdit : public QDialog
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *);

signals:
    void newProfile(Profile3);
    void editProfile(Profile3, Profile3);

private:
    Ui::ProfileManager3NewEdit *ui;
    bool isEditing = false;
    Profile3 original;

    void setupModels();

private slots:
    void on_pushButtonAccept_clicked();
    void on_pushButtonCancel_clicked();
    void on_comboBoxVersion_currentIndexChanged(int index);

public:
    explicit ProfileManager3NewEdit(QWidget *parent = 0);
    explicit ProfileManager3NewEdit( Profile3 profile, QWidget *parent = 0);
    ~ProfileManager3NewEdit();

};

#endif // PROFILEMANAGER3NEWEDIT_HPP
