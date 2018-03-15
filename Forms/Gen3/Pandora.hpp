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

#ifndef PANDORA_HPP
#define PANDORA_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <libPokeFinder/RNG/LCRNG.hpp>
#include <libPokeFinder/Objects/Utilities.hpp>
#include <QStringList>
#include <QList>
#include <thread>

typedef uint32_t u32;

namespace Ui {
class Pandora;
}

class Pandora : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

private:
    Ui::Pandora *ui;
    QStandardItemModel *xdcolo = new QStandardItemModel(this);
    QStandardItemModel *frlge = new QStandardItemModel(this);
    QStandardItemModel *rs = new QStandardItemModel(this);

    void setupModels();

private slots:
    void on_pushButtonFindFRLGE_clicked();
    void on_checkBoxBattery_stateChanged(int arg1);
    void on_checkBoxPID_stateChanged(int arg1);
    void on_checkBoxTID_stateChanged(int arg1);
    void on_checkBoxSID_stateChanged(int arg1);
    void on_pushButtonFindRS_clicked();
    void on_radioButtonDateRS_toggled(bool checked);
    void on_radioButtonInitSeedRS_toggled(bool checked);
    void on_pushButtonFindXD_clicked();

public:
    explicit Pandora(QWidget *parent = 0);
    ~Pandora();

};

#endif // PANDORA_HPP
