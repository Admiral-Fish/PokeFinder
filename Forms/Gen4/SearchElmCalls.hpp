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

#ifndef SEARCHELMCALLS_HPP
#define SEARCHELMCALLS_HPP

#include <QDialog>
#include <Util/DateTime.hpp>
#include <PokeFinderCore/Objects/Utilities.hpp>

using std::vector;

namespace Ui
{
    class SearchElmCalls;
}

class SearchElmCalls : public QDialog
{
    Q_OBJECT

private:
    Ui::SearchElmCalls *ui;
    vector<DateTime> data;
    vector<bool> possible;

private slots:
    void on_pushButtonK_clicked();
    void on_pushButtonE_clicked();
    void on_pushButtonP_clicked();
    void on_lineEditCalls_textChanged(const QString &arg1);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_radioButtonElm_clicked();
    void on_radioButtonIrwin_clicked();

public:
    explicit SearchElmCalls(vector<DateTime> model, QWidget *parent = 0);
    ~SearchElmCalls();
    vector<bool> possibleResults();

};

#endif // SEARCHELMCALLS_HPP
