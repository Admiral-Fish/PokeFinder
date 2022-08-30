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

#include "ComboMenu.hpp"
#include <QActionGroup>
#include <QMenu>

ComboMenu::ComboMenu(QWidget *parent) : QToolButton(parent), actionGroup(new QActionGroup(this)), topMenu(new QMenu(this))
{
    actionGroup->setExclusive(true);
    QToolButton::setMenu(topMenu);

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

bool ComboMenu::findAction(const QString &name)
{
    auto actions = actionGroup->actions();
    for (size_t i = 1; i < actions.size(); i++)
    {
        QAction *action = actions[i];
        if (action->text() == name)
        {
            return true;
        }
    }

    return false;
}

int ComboMenu::getData() const
{
    QAction *action = actionGroup->checkedAction();
    return action->data().toInt();
}

void ComboMenu::removeAction(const QString &name)
{
    auto actions = actionGroup->actions();
    for (size_t i = 1; i < actions.size(); i++)
    {
        QAction *action = actions[i];
        if (action->text() == name)
        {
            // Action we are removing is checked
            // Check the first action for no weird behavior
            if (action->isChecked())
            {
                actions[0]->setChecked(true);
                actionChanged(actions[0]);
            }

            topMenu->removeAction(action);
            actionGroup->removeAction(action);
            delete action;
            break;
        }
    }
}

void ComboMenu::actionChanged(QAction *action)
{
    auto *parent = qobject_cast<QMenu *>(action->parentWidget());
    if (parent != nullptr && !parent->title().isEmpty())
    {
        setText(QString("%1 :%2").arg(parent->title(), action->text()));
    }
    else
    {
        setText(action->text());
    }
}
