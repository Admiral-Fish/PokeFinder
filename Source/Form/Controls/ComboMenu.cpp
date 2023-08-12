/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "ComboMenu.hpp"
#include <QMenu>

ComboMenu::ComboMenu(QWidget *parent) : QToolButton(parent), actionGroup(new QActionGroup(this)), topMenu(new QMenu(this))
{
    actionGroup->setExclusive(true);
    QToolButton::setMenu(topMenu);

    setPopupMode(QToolButton::InstantPopup);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(actionGroup, &QActionGroup::triggered, this, &ComboMenu::actionChanged);
}

void ComboMenu::addAction(const QString &actionText, int data, QMenu *menu)
{
    QAction *action = menu ? menu->addAction(actionText) : topMenu->addAction(actionText);
    action->setCheckable(true);
    action->setData(data);

    actionGroup->addAction(action);
    if (actionGroup->actions().size() == 1)
    {
        action->setChecked(true);
        actionChanged(action);
    }
}

void ComboMenu::addMenu(const QString &menuText, const std::vector<std::string> &actions, const std::vector<int> &data)
{
    QMenu *menu = topMenu->addMenu(menuText);
    for (size_t i = 0; i < actions.size(); i++)
    {
        int value = data.empty() ? i : data[i];
        addAction(QString::fromStdString(actions[i]), value, menu);
    }
}

void ComboMenu::addMenu(const QString &menuText, const std::vector<QString> &actions, const std::vector<int> &data)
{
    QMenu *menu = topMenu->addMenu(menuText);
    for (size_t i = 0; i < actions.size(); i++)
    {
        int value = data.empty() ? i : data[i];
        addAction(actions[i], value, menu);
    }
}

void ComboMenu::clearSelection()
{
    QAction *action = actionGroup->actions().constFirst();
    action->setChecked(true);
    actionChanged(action);
}

void ComboMenu::hideAction(const QVariant &data, bool hide)
{
    auto actions = actionGroup->actions();
    for (size_t i = 1; i < actions.size(); i++)
    {
        QAction *action = actions[i];
        if (action->data() == data)
        {
            action->setVisible(!hide);
            if (action->isChecked())
            {
                clearSelection();
            }
        }
    }

    // If all of a menus action are invisible then we also want to hide the menu
    for (QObject *object : topMenu->children())
    {
        auto *menu = qobject_cast<QMenu *>(object);
        if (menu)
        {
            auto menuActions = menu->actions();
            bool visible = std::all_of(menuActions.begin(), menuActions.end(), [](const QAction *action) { return action->isVisible(); });
            menu->menuAction()->setVisible(visible);
        }
    }
}

void ComboMenu::actionChanged(QAction *action)
{
    auto *parent = qobject_cast<QMenu *>(action->parent());
    if (parent != nullptr && !parent->title().isEmpty())
    {
        setText(QString("%1 :%2").arg(parent->title(), action->text()));
    }
    else
    {
        setText(action->text());
    }
}
