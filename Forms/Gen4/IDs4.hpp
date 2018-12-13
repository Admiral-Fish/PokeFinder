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

#ifndef IDS4_HPP
#define IDS4_HPP

#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <thread>
#include <Objects/Utilities.hpp>
#include <RNG/LCRNG.hpp>

namespace Ui
{
    class IDs4;
}

class IDs4 : public QMainWindow
{
    Q_OBJECT

signals:
    void updateProgress();

public:
    explicit IDs4(QWidget *parent = nullptr);
    ~IDs4() override;

private:
    Ui::IDs4 *ui;
    QStandardItemModel *model = new QStandardItemModel(this);
    bool isSearching = false;
    bool cancel = false;
    int progress;

    void setupModels();
    void searchPID();
    void searchTIDSID();
    void updateSearch();

private slots:
    void on_pushButtonSearchShinyPID_clicked();
    void on_pushButtonSearchTIDSID_clicked();
    void on_pushButtonSearchSeedFinder_clicked();
    void on_checkBoxSearchTIDShinyPID_toggled(bool checked);
    void on_checkBoxInfiniteSearchShinyPID_toggled(bool checked);
    void on_checkBoxSearchSID_toggled(bool checked);
    void on_checkBoxInfiniteSearchTIDSID_toggled(bool checked);
    void updateProgressBar();

};

#endif // IDS4_HPP
