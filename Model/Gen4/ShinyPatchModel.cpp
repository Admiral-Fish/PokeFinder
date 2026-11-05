/*
 * This file is part of PokéFinder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ShinyPatchModel.hpp"

ShinyPatchModel::ShinyPatchModel(QObject *parent) : TableModel(parent)
{
}

int ShinyPatchModel::columnCount(const QModelIndex &parent) const
{
    return header.size();
}

QVariant ShinyPatchModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    const auto &row = model[index.row()];
    const auto &patch = row.patch;
    switch (index.column())
    {
    case 0:
        return row.advances;
    case 1:
        return QString::number(row.seed, 16).toUpper().rightJustified(8, '0');
    case 2:
        return patch.ring;
    case 3:
        return patch.x;
    case 4:
        return patch.y;
    case 5:
        return patch.active ? QVariant(patch.chainRoll) : QVariant("-");
    case 6:
        return patch.shinyCall ? QVariant(patch.shinyRoll) : QVariant("-");
    case 7:
        return patch.shinyCall ? QVariant::fromValue<qulonglong>(u64(row.advances) + patch.shinyCall) : QVariant("-");
    case 8:
        return !patch.active ? tr("Invalid tile") : patch.shiny ? tr("Shiny") : patch.continues ? tr("Continues") : tr("Chain breaks");
    }
    return QVariant();
}

QVariant ShinyPatchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section >= 0 && section < header.size())
    {
        return header[section];
    }
    return QVariant();
}
