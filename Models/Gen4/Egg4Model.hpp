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

#ifndef EGG4MODEL_HPP
#define EGG4MODEL_HPP

#include <QAbstractTableModel>
#include <QVector>
#include <PokeFinderCore/Gen4/Frame4.hpp>

class Egg4GeneratorModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    QVector<Frame4> model;
    Method method;

public:
    Egg4GeneratorModel(QObject *parent, Method method);
    void setModel(const QVector<Frame4> &frames);
    void clear();
    void setMethod(Method method);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

};

class Egg4SearcherModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    QVector<Frame4> model;
    Method method;

public:
    Egg4SearcherModel(QObject *parent, Method method);
    void setModel(const QVector<Frame4> &frames);
    void addItems(const QVector<Frame4> &frames);
    void clear();
    void setMethod(Method method);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

};

#endif // EGG4MODEL_HPP
