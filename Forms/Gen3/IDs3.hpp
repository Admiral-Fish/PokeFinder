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

#ifndef IDS3_HPP
#define IDS3_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStringList>
#include <QList>
#include <PokeFinderCore/RNG/LCRNG.hpp>
#include <PokeFinderCore/Objects/Utilities.hpp>

using u32 = uint32_t;

namespace Ui
{
    class IDs3;
}

class IDs3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *event) override;

private:
    Ui::IDs3 *ui;
    QStandardItemModel *xdcolo = new QStandardItemModel(this);
    QStandardItemModel *frlge = new QStandardItemModel(this);
    QStandardItemModel *rs = new QStandardItemModel(this);

    void setupModels();

private slots:
    void on_pushButtonFindFRLGE_clicked();
    void on_checkBoxBattery_stateChanged(int arg1);
    void on_checkBoxPIDRS_stateChanged(int arg1);
    void on_checkBoxTIDRS_stateChanged(int arg1);
    void on_checkBoxSIDRS_stateChanged(int arg1);
    void on_pushButtonFindRS_clicked();
    void on_radioButtonDateRS_toggled(bool checked);
    void on_radioButtonInitSeedRS_toggled(bool checked);
    void on_pushButtonFindXD_clicked();
    void on_checkBoxPIDXD_stateChanged(int arg1);
    void on_checkBoxTIDXD_stateChanged(int arg1);
    void on_checkBoxSIDXD_stateChanged(int arg1);
    void on_checkBoxPIDFRLGE_stateChanged(int arg1);
    void on_checkBoxSIDFRLGE_stateChanged(int arg1);

public:
    explicit IDs3(QWidget *parent = nullptr);
    ~IDs3() override;

};

#endif // IDS3_HPP
