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

#ifndef PROFILEMANAGER3_HPP
#define PROFILEMANAGER3_HPP

#include <QMainWindow>
#include <Forms/Gen3/ProfileManager3NewEdit.hpp>
#include <PokeFinderCore/Gen3/Profile3.hpp>
#include <Models/Gen3/Profile3Model.hpp>

using u32 = uint32_t;

namespace Ui
{
    class ProfileManager3;
}

class ProfileManager3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *event) override;

signals:
    void updateProfiles();

private:
    Ui::ProfileManager3 *ui;
    Profile3Model *model = new Profile3Model(this);

    void setupModels();

private slots:
    void on_pushButtonNew_clicked();
    void on_pushButtonOk_clicked();
    void on_pushButtonEdit_clicked();
    void on_pushButtonDelete_clicked();

public:
    explicit ProfileManager3(QWidget *parent = nullptr);
    ~ProfileManager3() override;

};

#endif // PROFILEMANAGER3_HPP
