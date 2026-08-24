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

#ifndef POKERADARMODEL_HPP
#define POKERADARMODEL_HPP

#include <Core/Gen4/States/PokeRadarState.hpp>
#include <Model/Gen4/IRNGProvider4.hpp>
#include <Model/TableModel.hpp>
#include <QString>
#include <array>

class PokeRadarModel4 : public TableModel<PokeRadarState>, public IRNGProvider4
{
    Q_OBJECT

public:
    PokeRadarModel4(QObject *parent, bool searcher = false);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    u8 getCall(int row) const override
    {
        return model[row].getCall();
    }

    u8 getChatot(int row) const override
    {
        return model[row].getChatot();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void setShowSearcherBattleAdvances(bool flag);
    void setShowStats(bool flag);

private:
    QString getCoordinates(const PokeRadarState &state, int type) const;
    QString getCoordinates(const std::array<PokeRadarPatch, 4> &patches, bool visible, int type) const;
    QString getResults(const PokeRadarState &state) const;
    QString getSearcherCoordinates(const PokeRadarState &state) const;
    QString getSkip(const PokeRadarState &state) const;
    QVariant getPokemonData(const PokeRadarState &state, int column) const;
    QVariant getPokemonHeader(int section) const;
    int mapGeneratorColumn(int column) const;

    bool searcher;
    bool showSearcherBattleAdvances;
    bool showStats;
};

#endif // POKERADARMODEL_HPP
