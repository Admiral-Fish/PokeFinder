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

#include "SeedtoTime4Model.hpp"

SeedtoTime4Model::SeedtoTime4Model(QObject *parent, bool flag, Game version) : QAbstractTableModel(parent)
{
    calibrate = flag;
    this->version = version;
}

void SeedtoTime4Model::setModel(vector<DateTime> times)
{
    if (times.empty())
        return;
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + times.size() - 1);
    model.insert(model.end(), times.begin(), times.end());
    emit endInsertRows();
}

void SeedtoTime4Model::clear()
{
    if (model.empty())
        return;
    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    model.clear();
    model.shrink_to_fit();
    emit endRemoveRows();
}

DateTime SeedtoTime4Model::getData(int row)
{
    return model[row];
}

vector<DateTime> SeedtoTime4Model::getData()
{
    return model;
}

void SeedtoTime4Model::setFlags(bool flag, Game version)
{
    calibrate = flag;
    this->version = version;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int SeedtoTime4Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return (int)model.size();
}

int SeedtoTime4Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    if (calibrate)
        return (version == HeartGold || version == SoulSilver) ? 6 : 5;
    else
        return 3;
}

QVariant SeedtoTime4Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int column = index.column();
        DateTime frame = model[row];

        if (calibrate)
        {
            switch (column)
            {
                case 0:
                    return QString::number(frame.getSeed(), 16).toUpper().toUpper().rightJustified(8, '0');
                case 1:
                    return frame.getDate();
                case 2:
                    return frame.getTime();
                case 3:
                    return frame.getDelay();
                case 4:
                    return (version == HeartGold || version == SoulSilver) ? Utilities::elmCalls(frame.getSeed(), 15) : Utilities::coinFlips(frame.getSeed(), 15);
                case 5:
                    return "Roamer"; // TODO
            }
        }
        else
        {
            switch (column)
            {
                case 0:
                    return frame.getDate();
                case 1:
                    return frame.getTime();
                case 2:
                    return frame.getDelay();
            }
        }
    }
    return QVariant();
}

QVariant SeedtoTime4Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (calibrate)
            {
                switch (section)
                {
                    case 0:
                        return tr("Seed");
                    case 1:
                        return tr("Date");
                    case 2:
                        return tr("Time");
                    case 3:
                        return tr("Delay");
                    case 4:
                        return (version == HeartGold || version == SoulSilver) ? tr("Elm calls") : tr("Coin flips");
                    case 5 :
                        return tr("Roamer locations");
                }
            }
            else
            {
                switch (section)
                {
                    case 0:
                        return tr("Date");
                    case 1:
                        return tr("Time");
                    case 2:
                        return tr("Delay");
                }
            }
        }
    }
    return QVariant();
}
