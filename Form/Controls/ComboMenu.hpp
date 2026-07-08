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

#ifndef COMBOMENU_HPP
#define COMBOMENU_HPP

#include <Core/Global.hpp>
#include <QActionGroup>
#include <QToolButton>
#include <vector>

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
     */
    void addMenu(const QString &menuText, const std::vector<std::string> &actions);

    /**
     * @brief Adds a new sub menu and set of actions
     *
     * @param menuText Display text for the menu
     * @param actions Collection of actions to add
     */
    void addMenu(const QString &menuText, const std::vector<std::pair<QString, int>> &actions);

    /**
     * @brief Defaults selected action to first one of the menu
     */
    void clearSelection();

    /**
     * @brief Gets checked action data
     *
     * @return Checked action data
     */
    std::vector<int> getCheckedData() const;

    /**
     * @brief Sets checked action data
     *
     * @param data Action data values to check
     */
    void setCheckedData(const std::vector<int> &data);

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
        static_assert(std::is_same<u8, std::underlying_type_t<Enum>>::value, "Unsupported enum base type");

        QAction *action = actionGroup->checkedAction();
        if constexpr (std::is_same<u8, std::underlying_type_t<Enum>>::value)
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

    /**
     * @brief Sets whether multiple actions can be selected
     *
     * @param flag Whether multiple actions can be selected
     */
    void setMultiSelect(bool flag);

private:
    bool eventFilter(QObject *object, QEvent *event) override;

    QActionGroup *actionGroup;
    QMenu *topMenu;
    bool multiSelect;

    /**
     * @brief Updates display text for multi-select mode
     */
    void updateMultiSelectText();

    /**
     * @brief Adds multi-select toggling behavior to a sub menu
     *
     * @param menu Sub menu to configure
     */
    void configureSubMenu(QMenu *menu);

private slots:
    /**
     * @brief Updates display text of the \ref ComboMenu
     *
     * @param action Action that is being selected
     */
    void actionChanged(QAction *action);
};

#endif // COMBOMENU_HPP
