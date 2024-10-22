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
#include <QCheckBox>
#include <QLineEdit>
#include <QListWidget>
#include <QMouseEvent>

CheckList::CheckList(QWidget *parent) : QComboBox(parent), lineEdit(new QLineEdit(this)), listWidget(new QListWidget(this))
{
    setModel(listWidget->model());
    setView(listWidget);
    setLineEdit(lineEdit);

    lineEdit->setReadOnly(true);
    lineEdit->installEventFilter(this);

    connect(this, &QComboBox::activated, this, &CheckList::itemPressed);
}

void CheckList::addItem(const QString &string, const QVariant &data)
{
    auto *checkBox = new QCheckBox(string, this);
    connect(checkBox, &QCheckBox::checkStateChanged, this, &CheckList::modelDataChanged);

    auto *item = new QListWidgetItem(listWidget);
    item->setData(Qt::UserRole, data);
    listWidget->setItemWidget(item, checkBox);

    // Adding items seems to empty the line edit. Reset to the text we want everytime.
    lineEdit->setText(tr("Any"));
}

void CheckList::addItem(const std::string &string, u16 data)
{
    addItem(QString::fromStdString(string), data);
}

void CheckList::addItems(const std::vector<std::string> &strings)
{
    if (!strings.empty())
    {
        clear();
        for (const auto &string : strings)
        {
            addItem(string, 0);
        }
    }
}

void CheckList::addItems(const std::vector<std::string> &strings, std::vector<u16> &data)
{
    if (!strings.empty())
    {
        clear();
        for (int i = 0; i < strings.size() && i < data.size(); i++)
        {
            addItem(strings[i], data[i]);
        }
    }
}

std::vector<bool> CheckList::getChecked() const
{
    std::vector<bool> result;
    if (checkState() == Qt::PartiallyChecked)
    {
        for (int i = 0; i < listWidget->count(); i++)
        {
            auto *item = listWidget->item(i);
            auto *checkBox = qobject_cast<QCheckBox *>(listWidget->itemWidget(item));
            result.emplace_back(checkBox->checkState() == Qt::Checked);
        }
    }
    else
    {
        result = std::vector<bool>(listWidget->count(), true);
    }
    return result;
}

std::vector<u16> CheckList::getCheckedData() const
{
    auto checked = getChecked();
    std::vector<u16> data;
    for (int i = 0; i < checked.size(); i++)
    {
        if (checked[i])
        {
            data.emplace_back(itemData(i).toUInt());
        }
    }
    return data;
}

void CheckList::hidePopup()
{
    int width = this->width();
    int height = listWidget->height();
    int x = QCursor::pos().x() - mapToGlobal(geometry().topLeft()).x() + geometry().x();
    int y = QCursor::pos().y() - mapToGlobal(geometry().topLeft()).y() + geometry().y();
    if (x < 0 || x > width || y < this->height() || y > height + this->height())
    {
        QComboBox::hidePopup();
    }
}

void CheckList::resetChecks()
{
    for (int i = 0; i < listWidget->count(); ++i)
    {
        auto *item = listWidget->item(i);
        auto *checkBox = qobject_cast<QCheckBox *>(listWidget->itemWidget(item));
        checkBox->setChecked(false);
    }
}

void CheckList::setChecks(const std::vector<bool> &flags)
{
    for (int i = 0; i < flags.size() && i < listWidget->count(); i++)
    {
        auto *item = listWidget->item(i);
        auto *checkBox = qobject_cast<QCheckBox *>(listWidget->itemWidget(item));
        checkBox->setChecked(flags[i]);
    }
}

void CheckList::setItemText(int index, const QString &text)
{
    auto *item = listWidget->item(index);
    auto *checkBox = qobject_cast<QCheckBox *>(listWidget->itemWidget(item));
    checkBox->setText(text);
}

Qt::CheckState CheckList::checkState() const
{
    int total = listWidget->count();
    int checked = 0;
    int unchecked = 0;

    for (int i = 0; i < total; i++)
    {
        auto *item = listWidget->item(i);
        auto *checkBox = qobject_cast<QCheckBox *>(listWidget->itemWidget(item));
        if (checkBox->isChecked())
        {
            checked++;
        }
        else
        {
            unchecked++;
        }
    }

    if (checked == total)
    {
        return Qt::Checked;
    }
    else if (unchecked == total)
    {
        return Qt::Unchecked;
    }
    else
    {
        return Qt::PartiallyChecked;
    }
}

bool CheckList::eventFilter(QObject *object, QEvent *event)
{
    if (object == lineEdit && event->type() == QEvent::MouseButtonRelease)
    {
        auto *mouse = reinterpret_cast<QMouseEvent *>(event);
        if (mouse->modifiers() == Qt::ControlModifier)
        {
            resetChecks();
        }
        else
        {
            showPopup();
        }
        return false;
    }
    return false;
}

void CheckList::keyPressEvent(QKeyEvent *event)
{
    // Do not handle key event
}

void CheckList::wheelEvent(QWheelEvent *event)
{
    // Do not handle wheel event
}

void CheckList::itemPressed(int index)
{
    auto *item = listWidget->item(index);
    auto *checkBox = qobject_cast<QCheckBox *>(listWidget->itemWidget(item));
    checkBox->setChecked(!checkBox->isChecked());
}

void CheckList::modelDataChanged()
{
    QString text;

    switch (checkState())
    {
    case Qt::Checked:
    case Qt::Unchecked:
        text = tr("Any");
        break;
    case Qt::PartiallyChecked:
        for (int i = 0; i < listWidget->count(); i++)
        {
            auto *item = listWidget->item(i);
            auto *checkBox = qobject_cast<QCheckBox *>(listWidget->itemWidget(item));

            if (checkBox->isChecked())
            {
                if (!text.isEmpty())
                {
                    text += ", ";
                }

                text += checkBox->text();
            }
        }
        break;
    }

    lineEdit->setText(text);
}
