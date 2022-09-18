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

#include <Core/Global.hpp>
#include <QActionGroup>
#include <QToolButton>

/**
 * @brief Provides exclusion item selection via menu selection where sub menus can act as combo boxes
 */
class ComboMenu : public QToolButton
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new ComboMenu object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    ComboMenu(QWidget *parent = nullptr);

    /**
     * @brief Adds new action to either the main menu or a provided sub menu
     *
     * @param actionText Display text for the action
     * @param data Data to be associated with the action
     * @param menu Sub menu for action to be added to if provided
     */
    void addAction(const QString &actionText, int data, QMenu *menu = nullptr);

    /**
     * @brief Adds a new sub menu and set of actions
     *
     * @param menuText Display text for the menu
     * @param actions Collection of actions to add
     * @param data Collection of data for the added actions
     */
    void addMenu(const QString &menuText, const std::vector<std::string> &actions, const std::vector<int> &data = {});

    /**
     * @brief Adds a new sub menu and set of actions
     *
     * @param menuText Display text for the menu
     * @param actions Collection of actions to add
     * @param data Collection of data for the added actions
     */
    void addMenu(const QString &menuText, const std::vector<QString> &actions, const std::vector<int> &data = {});

    /**
     * @brief Defaults selected action to first one of the menu
     */
    void clearSelection();

    /**
     * @brief Gets current selected index data as templated enum type
     *
     * @tparam Enum Enum type to convert data too
     *
     * @return Current data as enum type
     */
    template <typename Enum>
    constexpr Enum getEnum() const
    {
        static_assert(std::is_same<u8, typename std::underlying_type<Enum>::type>::value, "Unsupported enum base type");

        QAction *action = actionGroup->checkedAction();
        if constexpr (std::is_same<u8, typename std::underlying_type<Enum>::type>::value)
        {
            return static_cast<Enum>(action->data().toUInt());
        }
        else
        {
            return static_cast<Enum>(0);
        }
    }

    /**
     * @brief Sets the visibility of the action
     *
     * @param data Data to search action by
     * @param hide Whether or not to hide the action
     */
    void hideAction(const QVariant &data, bool hide);

private:
    QActionGroup *actionGroup;
    QMenu *topMenu;

private slots:
    /**
     * @brief Updates display text of the \ref ComboMenu
     *
     * @param action Action that is being selected
     */
    void actionChanged(QAction *action);
};

#endif // COMBOMENU_HPP
