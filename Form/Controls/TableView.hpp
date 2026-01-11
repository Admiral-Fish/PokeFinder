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

#ifndef TABLEVIEW_HPP
#define TABLEVIEW_HPP

#include <QTableView>

/**
 * @brief Provides automatic horizontal header sizing and action handling
 * The TableView class automatically resizes the horizontal header to use all available space then allows the user to manually resize each
 * column. It also provides a context menu with default actions to output the data to txt/csv. This menu can be provided additional
 * developer defined actions.
 */
class TableView : public QTableView
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new TableView object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    TableView(QWidget *parent = nullptr);

    /**
     * @brief Sets the model and connects signal to auto resize when rows are added
     *
     * @param model Model for the table view to present
     */
    void setModel(QAbstractItemModel *model) override;

private:
    /**
     * @brief Handles when the context menu is requested. Only displays if the model isn't empty.
     *
     * @param event Contains context menu event information
     */
    void contextMenuEvent(QContextMenuEvent *event) override;

    /**
     * @brief Handles ctr+c key inputs to copy selection to clipboard
     *
     * @param event Contains keyboard event information
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief Handles double click mouse event to copy selection to clipboard
     *
     * @param event Contains mouse event information
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief Outputs model data to file
     *
     * @param csv Whether the output format is txt or csv
     */
    void outputModel(bool csv = false) const;

    /**
     * @brief Handles window resize event to resize the header
     *
     * @param event Contains resize event information
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief Sets the current selected tableview region to the clipboard
     */
    void setSelectionToClipBoard();
};

#endif // TABLEVIEW_HPP
