/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
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

#ifndef RESEARCHERMODEL_HPP
#define RESEARCHERMODEL_HPP

#include <QAbstractTableModel>
#include <Util/ResearcherFrame.hpp>
#include <vector>
#include <cstdint>

using namespace std;

class ResearcherModel : public QAbstractTableModel
{

private:
    vector<ResearcherFrame> model;
    bool flag;
    static inline u64 Get64Bit(ResearcherFrame f) { return f.Full64; }
    static inline u64 Get32BitHigh(ResearcherFrame f) { return f.High32(); }
    static inline u64 Get32BitLow(ResearcherFrame f) { return f.Low32(); }
    static inline u64 Get32(ResearcherFrame f) { return f.Full32; }
    static inline u64 Get16BitHigh(ResearcherFrame f) { return f.High16(); }
    static inline u64 Get16BitLow(ResearcherFrame f) { return f.Low16(); }

public:
    ResearcherModel(QObject *parent, bool is64Bit);
    void SetModel(vector<ResearcherFrame>);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex Search(QString string, u64 result, int row);

};

#endif // RESEARCHERMODEL_HPP
