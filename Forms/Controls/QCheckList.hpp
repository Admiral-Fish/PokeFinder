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

#ifndef QCHECKLIST
#define QCHECKLIST

#include <QWidget>
#include <QComboBox>
#include <QStandardItemModel>
#include <QEvent>
#include <QLineEdit>
#include <QListView>
#include <QVector>
#include <QStyledItemDelegate>

class QCheckList : public QComboBox
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QStandardItemModel *model;

    void updateText();
    int globalCheckState();

private slots:
    void on_modelDataChanged();
    void on_itemPressed(const QModelIndex &index);

public:
    QCheckList(QWidget *parent = nullptr);
    ~QCheckList() override;
    void setup();
    QVector<bool> getChecked();
    void uncheckAll();
    void setChecks(QVector<bool> flags);

};

#endif // QCHECKLIST
