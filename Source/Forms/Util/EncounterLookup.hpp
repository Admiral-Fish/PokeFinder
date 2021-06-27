/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef ENCOUNTERLOOKUP_HPP
#define ENCOUNTERLOOKUP_HPP

#include <Core/Util/Global.hpp>
#include <QWidget>
#include <set>

class QStandardItemModel;
enum Encounter : u8;
enum Game : u16;

namespace Ui
{
    class EncounterLookup;
}

class EncounterLookup : public QWidget
{
    Q_OBJECT
public:
    explicit EncounterLookup(QWidget *parent = nullptr);
    ~EncounterLookup() override;

private:
    Ui::EncounterLookup *ui;
    QStandardItemModel *model = nullptr;

    void setupModels();
    std::set<std::pair<u8, QString>> getEncounters3(Game game, u16 specie);
    std::set<std::pair<u8, QString>> getEncounters4(Game game, u16 specie);
    QString getEncounterString(Encounter type);

private slots:
    void find();
    void gameIndexChanged(int index);
};

#endif // ENCOUNTERLOOKUP_HPP
