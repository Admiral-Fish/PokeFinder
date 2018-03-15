/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#ifndef QCHECKLIST
#define QCHECKLIST

#include <QWidget>
#include <QComboBox>
#include <QStandardItemModel>
#include <QEvent>
#include <QLineEdit>
#include <QListView>
#include <vector>
#include <QStyledItemDelegate>

using std::vector;

class QCheckList : public QComboBox
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QStandardItemModel *model;

    void updateText();
    int globalCheckState();

private slots:
    void on_modelDataChanged();
    void on_itemPressed(const QModelIndex &index);

public:
    QCheckList(QWidget *parent = 0);
    ~QCheckList();
    void setup();
    vector<bool> getChecked();
    void uncheckAll();

    class QCheckListStyledItemDelegate : public QStyledItemDelegate
    {

    public:
        QCheckListStyledItemDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) {}
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    };

};

#endif // QCHECKLIST
