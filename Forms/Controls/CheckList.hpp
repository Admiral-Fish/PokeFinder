/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef CHECKLIST
#define CHECKLIST

#include <QComboBox>

class QStandardItemModel;

class CheckList : public QComboBox
{
    Q_OBJECT
public:
    explicit CheckList(QWidget *parent = nullptr);
    void setup(const QStringList &items = QStringList());
    QVector<bool> getChecked() const;
    void setChecks(QVector<bool> flags);

public slots:
    void resetChecks();

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QStandardItemModel *model;

    void updateText();
    int checkState() const;

private slots:
    void modelDataChanged();
    void itemPressed(const QModelIndex &index);
};

#endif // CHECKLIST
