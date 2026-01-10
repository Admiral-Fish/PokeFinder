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

#ifndef TABWIDGET_HPP
#define TABWIDGET_HPP

#include <QTabWidget>

class TabWidget : public QTabWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the filters should be transferred
     */
    void transferFilters(int);

    /**
     * @brief Emits that the settings should be transferred
     */
    void transferSettings(int);

public:
    /**
     * @brief Construct a new TabWidget object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    TabWidget(QWidget *parent = nullptr);

    /**
     * @brief Handles when the context menu is requested
     *
     * @param event Contains context menu event information
     */
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // TABWIDGET_HPP
