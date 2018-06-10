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

#ifndef RESEARCHERMODEL_HPP
#define RESEARCHERMODEL_HPP

#include <QAbstractTableModel>
#include <Util/ResearcherFrame.hpp>
#include <vector>
#include <cstdint>

using std::vector;

class ResearcherModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    vector<ResearcherFrame> model;
    bool flag;
    vector<bool> hex;

    static inline u64 get64Bit(ResearcherFrame f) { return f.full64; }
    static inline u64 get32BitHigh(ResearcherFrame f) { return f.high32(); }
    static inline u64 get32BitLow(ResearcherFrame f) { return f.low32(); }
    static inline u64 get32(ResearcherFrame f) { return f.full32; }
    static inline u64 get16BitHigh(ResearcherFrame f) { return f.high16(); }
    static inline u64 get16BitLow(ResearcherFrame f) { return f.low16(); }

public:
    ResearcherModel(QObject *parent, bool is64Bit);
    void setModel(vector<ResearcherFrame>);
    void clear();
    void setFlag(bool is64Bit);
    void setHex(vector<bool> hex);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex search(QString string, u64 result, int row);

};

#endif // RESEARCHERMODEL_HPP
