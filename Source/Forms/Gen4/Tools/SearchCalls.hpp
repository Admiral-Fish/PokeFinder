/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <Core/Util/Global.hpp>
#include <QDialog>

class SeedTime;

namespace Ui
{
    class SearchCalls;
}

class SearchCalls : public QDialog
{
    Q_OBJECT
public:
    explicit SearchCalls(const std::vector<SeedTime> &model, QWidget *parent = nullptr);
    ~SearchCalls() override;
    std::vector<bool> possibleResults() const;

private:
    Ui::SearchCalls *ui;
    std::vector<SeedTime> data;
    std::vector<bool> possible;

private slots:
    void k();
    void e();
    void p();
    void callsTextChanged(const QString &val);
    void elm();
    void irwin();
};

#endif // SEARCHCALLS_HPP
