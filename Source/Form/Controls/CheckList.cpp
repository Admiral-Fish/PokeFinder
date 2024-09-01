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

#include "CheckList.hpp"
#include <QApplication>
#include <QLineEdit>
#include <QListView>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <algorithm>

CheckList::CheckList(QWidget *parent) : QComboBox(parent)
{
    addItem(tr("Any"));
    qobject_cast<QListView *>(view())->setRowHidden(0, true);

    setToolTip(tr("Click holding ctrl to reset"));

    connect(qobject_cast<QListView *>(view()), &QAbstractItemView::pressed, this, &CheckList::itemPressed);
    connect(model(), &QAbstractItemModel::dataChanged, this, &CheckList::modelDataChanged);
    connect(this, &CheckList::currentIndexChanged, this, [this] {
        if (currentIndex() != 0)
        {
            setCurrentIndex(0);
        }
    });
}

std::vector<bool> CheckList::getChecked() const
{
    std::vector<bool> result;

    auto *m = qobject_cast<QStandardItemModel *>(model());
    if (checkState() == Qt::PartiallyChecked)
    {
        for (int i = 1; i < m->rowCount(); i++)
        {
            result.emplace_back(m->item(i)->checkState() == Qt::Checked);
        }
    }
    else
    {
        result = std::vector<bool>(m->rowCount() - 1, true);
    }
    return result;
}

std::vector<u16> CheckList::getCheckedData() const
{
    auto checked = getChecked();
    std::vector<u16> data;
    for (int i = 1; i < checked.size(); i++)
    {
        if (checked[i])
        {
            data.emplace_back(itemData(i).toUInt());
        }
    }
    return data;
}

void CheckList::resetChecks()
{
    auto *m = qobject_cast<QStandardItemModel *>(model());
    for (auto i = 1; i < m->rowCount(); i++)
    {
        m->item(i)->setCheckState(Qt::Unchecked);
    }
}

void CheckList::setChecks(const std::vector<bool> &flags)
{
    auto *m = qobject_cast<QStandardItemModel *>(model());
    for (size_t i = 1; i < flags.size() && i < m->rowCount(); i++)
    {
        m->item(i)->setCheckState(flags[i] ? Qt::Checked : Qt::Unchecked);
    }
}

void CheckList::setup(const std::vector<std::string> &items)
{
    if (!items.empty())
    {
        auto *m = qobject_cast<QStandardItemModel *>(model());
        m->removeRows(1, m->rowCount() - 1);

        for (const auto &item : items)
        {
            addItem(QString::fromStdString(item));
        }
    }

    setupChecks();
}

void CheckList::setup(const std::vector<std::string> &items, const std::vector<u16> &data)
{
    assert(items.size() == data.size());
    if (!items.empty())
    {
        auto *m = qobject_cast<QStandardItemModel *>(model());
        m->removeRows(1, m->rowCount() - 1);

        for (int i = 0; i < items.size(); i++)
        {
            addItem(QString::fromStdString(items[i]), data[i]);
        }
    }

    setupChecks();
}

Qt::CheckState CheckList::checkState() const
{
    auto *m = qobject_cast<QStandardItemModel *>(model());

    int total = m->rowCount() - 1;
    int checked = 0;
    int unchecked = 0;

    for (int i = 1; i < total + 1; i++)
    {
        if (m->item(i)->checkState() == Qt::Checked)
        {
            checked++;
        }
        else if (m->item(i)->checkState() == Qt::Unchecked)
        {
            unchecked++;
        }
    }

    return checked == total ? Qt::Checked : unchecked == total ? Qt::Unchecked : Qt::PartiallyChecked;
}

void CheckList::mousePressEvent(QMouseEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        resetChecks();
    }
    else
    {
        QComboBox::mousePressEvent(event);
    }
}

void CheckList::setupChecks()
{
    auto *m = qobject_cast<QStandardItemModel *>(model());

    auto font = fontMetrics();
    int width = 0;
    for (int i = 1; i < m->rowCount(); i++)
    {
        QStandardItem *item = m->item(i);
        item->setCheckState(Qt::Unchecked);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

        width = std::max(width, font.size(0, item->text()).width());
    }

    width += 2 * font.size(0, " ").width() + iconSize().width() + QApplication::style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    width *= 1.25f;

    view()->setMinimumWidth(width);
}

void CheckList::modelDataChanged()
{
    QString text;

    auto *m = qobject_cast<QStandardItemModel *>(model());
    switch (checkState())
    {
    case Qt::Checked:
    case Qt::Unchecked:
        text = tr("Any");
        break;
    case Qt::PartiallyChecked:
        for (int i = 1; i < m->rowCount(); i++)
        {
            if (m->item(i)->checkState() == Qt::Checked)
            {
                if (!text.isEmpty())
                {
                    text += ", ";
                }

                text += m->item(i)->text();
            }
        }
        break;
    }

    setItemText(0, text);
}

void CheckList::itemPressed(const QModelIndex &index)
{
    auto *m = qobject_cast<QStandardItemModel *>(model());
    QStandardItem *item = m->itemFromIndex(index);
    item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
}
