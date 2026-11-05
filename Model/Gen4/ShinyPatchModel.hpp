/*
 * This file is part of PokéFinder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef SHINYPATCHMODEL_HPP
#define SHINYPATCHMODEL_HPP

#include <Core/Gen4/Tools/ShinyPatchCalculator.hpp>
#include <Model/TableModel.hpp>

struct ShinyPatchRow
{
    u32 advances;
    u32 seed;
    ShinyPatchCalculator::Patch patch;
};

/** @brief Displays radar activation advances and the outcome of each selected patch. */
class ShinyPatchModel final : public TableModel<ShinyPatchRow>
{
    Q_OBJECT
public:
    explicit ShinyPatchModel(QObject *parent = nullptr);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header = { tr("Advances"), tr("Seed"),       tr("Ring"),       tr("X (right)"),
                           tr("Y (down)"), tr("Chain roll"), tr("Shiny roll"), tr("Shiny check advance"),
                           tr("Result") };
};

#endif // SHINYPATCHMODEL_HPP
