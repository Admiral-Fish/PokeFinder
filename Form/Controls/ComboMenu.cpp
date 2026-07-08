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

#include "ComboMenu.hpp"
#include <QEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QStringList>
#include <algorithm>

ComboMenu::ComboMenu(QWidget *parent) : QToolButton(parent), actionGroup(new QActionGroup(this)), topMenu(new QMenu(this)), multiSelect(false)
{
    actionGroup->setExclusive(true);
    QToolButton::setMenu(topMenu);
    topMenu->installEventFilter(this);

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
    if (multiSelect)
    {
        action->setChecked(true);
        updateMultiSelectText();
    }
    else if (actionGroup->actions().size() == 1)
    {
        action->setChecked(true);
        actionChanged(action);
    }
}

void ComboMenu::addMenu(const QString &menuText, const std::vector<std::string> &actions)
{
    std::vector<int> indices(actions.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::ranges::sort(indices, [&actions](int i, int j) { return actions[i] < actions[j]; });

    QMenu *menu = topMenu->addMenu(menuText);
    configureSubMenu(menu);
    for (int i : indices)
    {
        addAction(QString::fromStdString(actions[i]), i, menu);
    }
}

void ComboMenu::addMenu(const QString &menuText, const std::vector<std::pair<QString, int>> &actions)
{
    QMenu *menu = topMenu->addMenu(menuText);
    configureSubMenu(menu);
    for (const auto &action : actions)
    {
        addAction(action.first, action.second, menu);
    }
}

void ComboMenu::clearSelection()
{
    QAction *action = actionGroup->actions().constFirst();
    action->setChecked(true);
    actionChanged(action);
}

std::vector<int> ComboMenu::getCheckedData() const
{
    auto actions = actionGroup->actions();
    std::vector<int> data;
    if (!multiSelect)
    {
        data.emplace_back(actionGroup->checkedAction()->data().toInt());
        return data;
    }

    int checked = std::ranges::count_if(actions, [](const QAction *action) { return action->isChecked(); });
    bool all = checked == 0 || checked == actions.size();
    for (const auto *action : actions)
    {
        if (all || action->isChecked())
        {
            data.emplace_back(action->data().toInt());
        }
    }

    return data;
}

void ComboMenu::setCheckedData(const std::vector<int> &data)
{
    for (auto *action : actionGroup->actions())
    {
        bool checked = std::ranges::contains(data, action->data().toInt());
        action->setChecked(checked);
    }

    if (multiSelect)
    {
        updateMultiSelectText();
    }
    else
    {
        QAction *action = actionGroup->checkedAction();
        if (action != nullptr)
        {
            actionChanged(action);
        }
    }
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
            if (action->isChecked() && hide)
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
            bool visible = std::ranges::any_of(menuActions, [](const QAction *action) { return action->isVisible(); });
            menu->menuAction()->setVisible(visible);
        }
    }

    if (multiSelect)
    {
        updateMultiSelectText();
    }
}

void ComboMenu::setMultiSelect(bool flag)
{
    multiSelect = flag;
    actionGroup->setExclusive(!flag);

    if (multiSelect)
    {
        for (QObject *object : topMenu->children())
        {
            auto *menu = qobject_cast<QMenu *>(object);
            if (menu != nullptr)
            {
                configureSubMenu(menu);
            }
        }

        updateMultiSelectText();
    }
}

bool ComboMenu::eventFilter(QObject *object, QEvent *event)
{
    auto *menu = qobject_cast<QMenu *>(object);
    if (multiSelect && menu != nullptr && event->type() == QEvent::MouseButtonRelease)
    {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);
        QAction *action = menu->actionAt(mouseEvent->pos());
        if (action == nullptr)
        {
            return QToolButton::eventFilter(object, event);
        }

        if (action->menu() != nullptr)
        {
            auto menuActions = action->menu()->actions();
            bool checked = !std::ranges::all_of(menuActions, [](const QAction *menuAction) {
                return !menuAction->isVisible() || menuAction->isChecked();
            });
            for (auto *menuAction : menuActions)
            {
                if (menuAction->isVisible())
                {
                    menuAction->setChecked(checked);
                }
            }
            updateMultiSelectText();
            return true;
        }

        if (action->isCheckable())
        {
            action->setChecked(!action->isChecked());
            updateMultiSelectText();
            return true;
        }
    }

    return QToolButton::eventFilter(object, event);
}

void ComboMenu::actionChanged(QAction *action)
{
    if (multiSelect)
    {
        updateMultiSelectText();
        return;
    }

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

void ComboMenu::updateMultiSelectText()
{
    auto actions = actionGroup->actions();
    int checked = std::ranges::count_if(actions, [](const QAction *action) { return action->isChecked(); });

    for (QObject *object : topMenu->children())
    {
        auto *menu = qobject_cast<QMenu *>(object);
        if (menu != nullptr)
        {
            auto menuActions = menu->actions();
            bool hasVisible = std::ranges::any_of(menuActions, [](const QAction *action) { return action->isVisible(); });
            bool allChecked = hasVisible && std::ranges::all_of(menuActions, [](const QAction *action) {
                return !action->isVisible() || action->isChecked();
            });
            menu->menuAction()->setChecked(allChecked);
        }
    }

    if (checked == 0 || checked == actions.size())
    {
        setText(tr("Any"));
        return;
    }

    QStringList text;
    for (const auto *action : actions)
    {
        if (action->isChecked())
        {
            auto *parent = qobject_cast<QMenu *>(action->parent());
            if (parent != nullptr && !parent->title().isEmpty())
            {
                text.emplace_back(QString("%1: %2").arg(parent->title(), action->text()));
            }
            else
            {
                text.emplace_back(action->text());
            }
        }
    }

    setText(text.join(", "));
}

void ComboMenu::configureSubMenu(QMenu *menu)
{
    if (!multiSelect)
    {
        return;
    }

    if (menu->property("comboMenuMultiSelect").toBool())
    {
        return;
    }

    menu->setProperty("comboMenuMultiSelect", true);
    menu->installEventFilter(this);
    menu->menuAction()->setCheckable(true);
    connect(menu->menuAction(), &QAction::triggered, this, [=](bool checked) {
        for (auto *action : menu->actions())
        {
            if (action->isVisible())
            {
                action->setChecked(checked);
            }
        }
        updateMultiSelectText();
    });
}
