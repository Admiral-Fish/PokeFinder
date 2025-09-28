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
#include <QStandardItemModel>
#include <Model/SortFilterProxyModel.hpp>

/**
 * @brief Provides a proxy to sort strings but sorting numbers numerically
 */
class CheckListProxyModel : public SortFilterProxyModel
{
public:
    /**
     * @brief Construct a new ComboBoxProxyModel object
     *
     * @param parent Parent object, which takes memory ownership
     * @param model Source model to be processed by proxy
     */
    CheckListProxyModel(QObject *parent, QAbstractItemModel *model) : SortFilterProxyModel(parent, model)
    {
    }

    /**
     * @brief Compares two items in the model
     * 
     * @param source_left First item to compare
     * @param source_right Second item to compare
     * 
     * @return true First item is less than second item
     * @return false First item is not less than second item
     */
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override
    {
        QString left = source_left.data().toString();
        QString right = source_right.data().toString();

        bool valid;
        int leftNum = left.toInt(&valid);
        if (valid)
        {
            int rightNum = right.toInt(&valid);
            if (valid)
            {
                return leftNum < rightNum;
            }
        }

        return left < right;
    }
};

CheckList::CheckList(QWidget *parent) : QComboBox(parent)
{
    lineEdit = new QLineEdit(this);
    lineEdit->setReadOnly(true);
    lineEdit->installEventFilter(this);

    model = new QStandardItemModel(this);
    proxyModel = new CheckListProxyModel(this, model);

    setModel(proxyModel);
    setLineEdit(lineEdit);

    connect(model, &QStandardItemModel::itemChanged, this, &CheckList::modelDataChanged);
}

void CheckList::addItem(const QString &string, const QVariant &data)
{
    auto *item = new QStandardItem(string);
    item->setData(data);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);

    model->appendRow(item);

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

        proxyModel->sort(0);
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

        proxyModel->sort(0);
    }
}

std::vector<bool> CheckList::getChecked() const
{
    std::vector<bool> result;
    if (checkState() == Qt::PartiallyChecked)
    {
        for (int i = 0; i < model->rowCount(); i++)
        {
            auto *item = model->item(i);
            result.emplace_back(item->checkState() == Qt::Checked);
        }
    }
    else
    {
        result = std::vector<bool>(model->rowCount(), true);
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
            data.emplace_back(model->item(i)->data().toUInt());
        }
    }
    return data;
}

void CheckList::hidePopup()
{
    int width = this->width();
    int height = this->view()->height();
    int x = QCursor::pos().x() - mapToGlobal(geometry().topLeft()).x() + geometry().x();
    int y = QCursor::pos().y() - mapToGlobal(geometry().topLeft()).y() + geometry().y();
    if (x < 0 || x > width || y < this->height() || y > height + this->height())
    {
        QComboBox::hidePopup();
    }
}

void CheckList::resetChecks()
{
    for (int i = 0; i < model->rowCount(); i++)
    {
        auto *item = model->item(i);
        item->setCheckState(Qt::Unchecked);
    }
}

void CheckList::setChecks(const std::vector<bool> &flags)
{
    for (int i = 0; i < flags.size() && i < model->rowCount(); i++)
    {
        auto *item = model->item(i);
        item->setCheckState(flags[i] ? Qt::Checked : Qt::Unchecked);
    }
}

Qt::CheckState CheckList::checkState() const
{
    int total = model->rowCount();
    int checked = 0;
    int unchecked = 0;

    for (int i = 0; i < total; i++)
    {
        auto *item = model->item(i);
        if (item->checkState() == Qt::Checked)
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
        for (int i = 0; i < proxyModel->rowCount(); i++)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i, 0));
            auto *item = model->item(index.row());
            if (item->checkState() == Qt::Checked)
            {
                if (!text.isEmpty())
                {
                    text += ", ";
                }

                text += item->text();
            }
        }
        break;
    }

    lineEdit->setText(text);
}
