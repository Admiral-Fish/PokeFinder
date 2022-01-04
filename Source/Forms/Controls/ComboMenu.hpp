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

#ifndef COMBOMENU_HPP
#define COMBOMENU_HPP

#include <QActionGroup>
#include <QToolButton>

class ComboMenu : public QToolButton
{
    Q_OBJECT
public:
    ComboMenu(QWidget *parent = nullptr);
    void addAction(const QString &actionText, int data, QMenu *menu = nullptr);
    void removeAction(const QString &name);
    void addMenu(const QString &menuText, const std::vector<std::string> &actions, const std::vector<int> &data = {});
    void addMenu(const QString &menuText, const std::vector<QString> &actions, const std::vector<int> &data = {});
    int getData() const;
    void clearSelection();

private:
    QActionGroup *actionGroup;
    QMenu *topMenu;

private slots:
    void actionChanged(QAction *action);
};

#endif // COMBOMENU_HPP
