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

#ifndef SEARCHCALLS_HPP
#define SEARCHCALLS_HPP

#include <Core/Global.hpp>
#include <QDialog>

class IndexFilterProxyModel;
class SeedTimeCalibrate4;
class SeedToTimeCalibrateModel4;
typedef QList<QModelIndex> QModelIndexList;

namespace Ui
{
    class SearchCalls;
}

/**
 * @brief Allows searching of Elm/Irwin calls to calibrate obtained seed
 */
class SearchCalls : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new SearchCalls object
     *
     * @param model Input model to search
     * @param parent Parent widget, which takes memory ownership
     */
    SearchCalls(SeedToTimeCalibrateModel4 *model, QWidget *parent = nullptr);

    /**
     * @brief Destroy the SearchCalls object
     */
    ~SearchCalls() override;

    /**
     * @brief Returns possible matching indexes based on input Elm/Irwin calls
     *
     * @return Vector of possible indexes
     */
    QModelIndexList getIndexes() const;

private:
    Ui::SearchCalls *ui;

    IndexFilterProxyModel *previewModel;
    SeedToTimeCalibrateModel4 *model;
    QModelIndexList indexes;

    /**
     * @brief Updates preview table with possible matching results
     */
    void updatePreview();

private slots:
    /**
     * @brief Updates possible matching calibration results
     *
     * @param text Current Elm/Irwin calls text
     */
    void callsTextChanged(const QString &text);

    /**
     * @brief Clears all Elm/Irwin calls from the string
     */
    void clear();

    /**
     * @brief Adds the Elm/Irwin E call to the string
     */
    void e();

    /**
     * @brief Updates display text to show the Elm E/K/P text
     */
    void elm();

    /**
     * @brief Updates display text to show the Irwin E/K/P text
     */
    void irwin();

    /**
     * @brief Adds the Elm/Irwin K call to the string
     */
    void k();

    /**
     * @brief Adds the Elm/Irwin P call to the string
     */
    void p();

    /**
     * @brief Removes the last Elm/Irwin call from the string
     */
    void remove();
};

#endif // SEARCHCALLS_HPP
