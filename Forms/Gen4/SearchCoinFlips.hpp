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

#ifndef SEARCHCOINFLIPS_HPP
#define SEARCHCOINFLIPS_HPP

#include <QDialog>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <Util/DateTime.hpp>
#include <PokeFinderCore/Objects/Utilities.hpp>


namespace Ui
{
    class SearchCoinFlips;
}

class SearchCoinFlips : public QDialog
{
    Q_OBJECT

private:
    Ui::SearchCoinFlips *ui;
    QVector<DateTime> data;
    QVector<bool> possible;

private slots:
    void on_pushButtonHeads_clicked();
    void on_pushButtonTails_clicked();
    void on_lineEditFlips_textChanged(const QString &arg1);
    void on_pushButtonOkay_clicked();
    void on_pushButtonCancel_clicked();

public:
    explicit SearchCoinFlips(const QVector<DateTime> &model, QWidget *parent = nullptr);
    ~SearchCoinFlips() override;
    QVector<bool> possibleResults();
};

#endif // SEARCHCOINFLIPS_HPP
