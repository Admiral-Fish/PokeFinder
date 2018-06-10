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

#ifndef SEARCHCALLS_HPP
#define SEARCHCALLS_HPP

#include <QDialog>
#include <Util/DateTime.hpp>
#include <PokeFinderCore/Objects/Utilities.hpp>
#include <PokeFinderCore/Gen4/HGSSRoamer.hpp>

using std::vector;

namespace Ui
{
    class SearchCalls;
}

class SearchCalls : public QDialog
{
    Q_OBJECT

private:
    Ui::SearchCalls *ui;
    vector<DateTime> data;
    vector<bool> possible;
    vector<bool> roamers;
    vector<u32> routes;

private slots:
    void on_pushButtonK_clicked();
    void on_pushButtonE_clicked();
    void on_pushButtonP_clicked();
    void on_lineEditCalls_textChanged(const QString &arg1);
    void on_radioButtonElm_clicked();
    void on_radioButtonIrwin_clicked();
    void on_pushButtonOkay_clicked();
    void on_pushButtonCancel_clicked();

public:
    explicit SearchCalls(vector<DateTime> model, vector<bool> roamers, vector<u32> routes, QWidget *parent = 0);
    ~SearchCalls();
    vector<bool> possibleResults();

};

#endif // SEARCHCALLS_HPP
