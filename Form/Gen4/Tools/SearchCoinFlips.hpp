/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

class IndexFilterProxyModel;
class SeedToTimeCalibrateModel4;
typedef QList<QModelIndex> QModelIndexList;

namespace Ui
{
    class SearchCoinFlips;
}

/**
 * @brief Allows searching of coin flips to calibrate obtained seed
 */
class SearchCoinFlips : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new SearchCoinFlips object
     *
     * @param model Input model to search
     * @param parent Parent widget, which takes memory ownership
     */
    SearchCoinFlips(SeedToTimeCalibrateModel4 *model, QWidget *parent = nullptr);

    /**
     * @brief Destroy the SearchCoinFlips object
     */
    ~SearchCoinFlips() override;

    /**
     * @brief Returns possible matching indexes based on input coin flips
     *
     * @return Vector of possible indexes
     */
    QModelIndexList getIndexes() const;

private:
    Ui::SearchCoinFlips *ui;

    IndexFilterProxyModel *previewModel;
    SeedToTimeCalibrateModel4 *model;
    QModelIndexList indexes;

    /**
     * @brief Updates preview table with possible matching results
     */
    void updatePreview();

private slots:
    /**
     * @brief Clears all coin flips from the string
     */
    void clear();

    /**
     * @brief Updates possible matching calibration results
     *
     * @param text Current coin flip text
     */
    void flipsTextChanged(const QString &text);

    /**
     * @brief Updates display text to show the heads text
     */
    void heads();

    /**
     * @brief Removes the last coin flip from the string
     */
    void remove();

    /**
     * @brief Updates display text to show the tails text
     */
    void tails();
};

#endif // SEARCHCOINFLIPS_HPP
