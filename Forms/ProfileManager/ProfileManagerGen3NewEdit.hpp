/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
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

#ifndef PROFILEMANAGERGEN3NEWEDIT_HPP
#define PROFILEMANAGERGEN3NEWEDIT_HPP

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QObject>
#include <QStandardItemModel>
#include <QList>

namespace Ui {
class ProfileManagerGen3NewEdit;
}

class ProfileManagerGen3NewEdit : public QDialog
{
    Q_OBJECT

signals:
    void newProfile(QString, int, int, uint32_t, uint32_t, bool);

public:
    explicit ProfileManagerGen3NewEdit(QWidget *parent = 0);
    explicit ProfileManagerGen3NewEdit(QString profileName, int version, int language, uint32_t tid, uint32_t sid, bool deadBattery, QWidget *parent = 0);
    ~ProfileManagerGen3NewEdit();

private slots:
    void on_pushButtonAccept_clicked();

    void on_pushButtonCancel_clicked();

    void on_comboBoxVersion_currentIndexChanged(int index);

private:
    Ui::ProfileManagerGen3NewEdit *ui;
};

#endif // PROFILEMANAGERGEN3NEWEDIT_HPP
