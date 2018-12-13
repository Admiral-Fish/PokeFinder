/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

CheckList::CheckList(QWidget *parent) : QComboBox(parent)
{
    model = new QStandardItemModel();
    setModel(model);

    setEditable(true);
    lineEdit()->setReadOnly(true);
    lineEdit()->installEventFilter(this);

    connect(lineEdit(), &QLineEdit::selectionChanged, lineEdit(), &QLineEdit::deselect);
    connect(dynamic_cast<QListView *>(view()), &QAbstractItemView::pressed, this, &CheckList::itemPressed);
    connect(model, &QAbstractItemModel::dataChanged, this, &CheckList::modelDataChanged);
}

CheckList::~CheckList()
{
    delete model;
}

void CheckList::setup()
{
    for (int i = 0; i < model->rowCount(); i++)
    {
        QStandardItem *item = model->item(i);
        item->setCheckState(Qt::Unchecked);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }
}

QVector<bool> CheckList::getChecked()
{
    QVector<bool> result;

    if (globalCheckState() == Qt::Unchecked || globalCheckState() == Qt::Checked)
    {
        for (auto i = 0; i < model->rowCount(); i++)
        {
            result.append(true);
        }
    }
    else
    {
        for (auto i = 0; i < model->rowCount(); i++)
        {
            result.append(model->item(i)->checkState() == Qt::Checked);
        }
    }
    return result;
}

void CheckList::uncheckAll()
{
    for (auto i = 0; i < model->rowCount(); i++)
    {
        model->item(i)->setCheckState(Qt::Unchecked);
    }
}

void CheckList::setChecks(QVector<bool> flags)
{
    for (auto i = 0; i < model->rowCount(); i++)
    {
        model->item(i)->setCheckState(flags[i] ? Qt::Checked : Qt::Unchecked);
    }
}

bool CheckList::eventFilter(QObject *object, QEvent *event)
{
    if (object == lineEdit() && event->type() == QEvent::MouseButtonPress)
    {
        showPopup();
        return true;
    }

    return false;
}

void CheckList::updateText()
{
    QString text;

    switch (globalCheckState())
    {
        case Qt::Checked:
            text = tr("Any");
            break;
        case Qt::Unchecked:
            text = tr("Any");
            break;
        case Qt::PartiallyChecked:
            for (int i = 0; i < model->rowCount(); i++)
            {
                if (model->item(i)->checkState() == Qt::Checked)
                {
                    if (!text.isEmpty())
                    {
                        text += ", ";
                    }

                    text += model->item(i)->text();
                }
            }
            break;
        default:
            text = tr("Any");
    }

    lineEdit()->setText(text);
}

int CheckList::globalCheckState()
{
    int total = model->rowCount(), checked = 0, unchecked = 0;

    for (int i = 0; i < total; i++)
    {
        if (model->item(i)->checkState() == Qt::Checked)
        {
            checked++;
        }
        else if (model->item(i)->checkState() == Qt::Unchecked)
        {
            unchecked++;
        }
    }

    return checked == total ? Qt::Checked : unchecked == total ? Qt::Unchecked : Qt::PartiallyChecked;
}

void CheckList::modelDataChanged()
{
    updateText();
}

void CheckList::itemPressed(const QModelIndex &index)
{
    QStandardItem *item = model->itemFromIndex(index);
    item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
}
