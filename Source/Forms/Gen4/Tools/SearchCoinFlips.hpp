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

#ifndef SEARCHCOINFLIPS_HPP
#define SEARCHCOINFLIPS_HPP

#include <QDialog>

class SeedTime;

namespace Ui
{
    class SearchCoinFlips;
}

class SearchCoinFlips : public QDialog
{
    Q_OBJECT
public:
    explicit SearchCoinFlips(const std::vector<SeedTime> &model, QWidget *parent = nullptr);
    ~SearchCoinFlips() override;
    std::vector<bool> possibleResults() const;

private:
    Ui::SearchCoinFlips *ui;
    std::vector<SeedTime> data;
    std::vector<bool> possible;

private slots:
    void heads();
    void tails();
    void flipsTextChanged(const QString &val);
};

#endif // SEARCHCOINFLIPS_HPP
