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

#include "PokeRadarModel.hpp"

#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <QStringList>
#include <algorithm>

PokeRadarModel4::PokeRadarModel4(QObject *parent, bool searcher) : TableModel(parent), searcher(searcher), showContinue(true), showStats(false)
{
}

int PokeRadarModel4::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : searcher ? 26 : 26;
}

QVariant PokeRadarModel4::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    const PokeRadarState &state = model[index.row()];
    int column = index.column();
    if (searcher)
    {
        if (column < 9)
        {
            switch (column)
            {
            case 0:
                return state.hasSearcherPokemon()
                    ? QString::number(state.getSearcherPokemon().getSeed(), 16).toUpper().rightJustified(8, '0')
                    : QStringLiteral("-");
            case 1:
                return state.hasSearcherPokemon() ? QVariant(state.getSearcherPokemon().getSeed() & 0xffff) : QVariant(QStringLiteral("-"));
            case 2:
                return state.hasSearcherPokemon() ? QVariant((state.getSearcherPokemon().getSeed() >> 16) & 0xff) : QVariant(QStringLiteral("-"));
            case 3:
                return state.getAdvances();
            case 4:
                return state.getPatchAdvances();
            case 5:
                return state.getDistance();
            case 6:
                return state.getChain();
            case 7:
                return getSkip(state);
            case 8:
                return getSearcherCoordinates(state);
            }
        }

        return getPokemonData(state, column - 9);
    }

    column = mapGeneratorColumn(column);
    if ((column == 1 || column == 2 || column > 7) && !state.hasPokemon())
    {
        return QStringLiteral("-");
    }

    switch (column)
    {
    case 0:
        return state.getAdvances();
    case 1:
        return state.getDisplayedBattleAdvances();
    case 2:
        return state.getDisplayedPatchAdvances();
    case 3:
        return QString::fromStdString(Utilities4::getChatot(state.getChatot()));
    case 4:
        return getSkip(state);
    case 5:
        return getCoordinates(state, 0);
    case 6:
        return getCoordinates(state, 1);
    case 7:
        return getCoordinates(state, 2);
    case 8:
        return getCoordinates(state, 3);
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
        return getPokemonData(state, column - 9);
    }

    return QVariant();
}

QVariant PokeRadarModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (searcher)
        {
            switch (section)
            {
            case 0:
                return tr("Seed");
            case 1:
                return tr("Delay");
            case 2:
                return tr("Hour");
            case 3:
                return tr("Advances");
            case 4:
                return tr("Patch Adv");
            case 5:
                return tr("Distance");
            case 6:
                return tr("Chain");
            case 7:
                return tr("Skip");
            case 8:
                return tr("Patch");
            default:
                return getPokemonHeader(section - 9);
            }
        }

        section = mapGeneratorColumn(section);
        switch (section)
        {
        case 0:
            return tr("Advances");
        case 1:
            return tr("Battle Adv");
        case 2:
            return tr("Patch Adv");
        case 3:
            return tr("Chatot");
        case 4:
            return tr("Skip");
        case 5:
            return tr("Regular");
        case 6:
            return tr("Strong");
        case 7:
            return tr("Continue");
        case 8:
            return tr("Shiny");
        case 9:
            return tr("Item");
        case 10:
            return tr("Slot");
        case 11:
            return tr("Level");
        case 12:
            return tr("PID");
        case 13:
            return tr("Shiny");
        case 14:
            return tr("Nature");
        case 15:
            return tr("Ability");
        case 16:
            return tr("HP");
        case 17:
            return tr("Atk");
        case 18:
            return tr("Def");
        case 19:
            return tr("SpA");
        case 20:
            return tr("SpD");
        case 21:
            return tr("Spe");
        case 22:
            return tr("Hidden");
        case 23:
            return tr("Power");
        case 24:
            return tr("Gender");
        case 25:
            return tr("Characteristic");
        }
    }

    return QVariant();
}

QVariant PokeRadarModel4::getPokemonData(const PokeRadarState &state, int column) const
{
    bool isSearcherState = searcher && state.hasSearcherPokemon();
    if (!state.hasPokemon())
    {
        return QStringLiteral("-");
    }

    switch (column)
    {
    case 0:
        return QString::fromStdString(Translator::getItem(isSearcherState ? state.getSearcherPokemon().getItem() : state.getPokemon().getItem()));
    case 1:
        return QString("%1: %2")
            .arg(isSearcherState ? state.getSearcherPokemon().getEncounterSlot() : state.getPokemon().getEncounterSlot())
            .arg(QString::fromStdString(Translator::getSpecie(isSearcherState ? state.getSearcherPokemon().getSpecie() : state.getPokemon().getSpecie(),
                                                              isSearcherState ? state.getSearcherPokemon().getForm() : state.getPokemon().getForm())));
    case 2:
        return isSearcherState ? state.getSearcherPokemon().getLevel() : state.getPokemon().getLevel();
    case 3:
        return QString::number(isSearcherState ? state.getSearcherPokemon().getPID() : state.getPokemon().getPID(), 16).toUpper().rightJustified(8, '0');
    case 4:
    {
        u8 shiny = isSearcherState ? state.getSearcherPokemon().getShiny() : state.getPokemon().getShiny();
        return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
    }
    case 5:
        return QString::fromStdString(Translator::getNature(isSearcherState ? state.getSearcherPokemon().getNature() : state.getPokemon().getNature()));
    case 6:
        return QString("%1: %2")
            .arg(isSearcherState ? state.getSearcherPokemon().getAbility() : state.getPokemon().getAbility())
            .arg(QString::fromStdString(Translator::getAbility(isSearcherState ? state.getSearcherPokemon().getAbilityIndex()
                                                                               : state.getPokemon().getAbilityIndex())));
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        return showStats ? (isSearcherState ? state.getSearcherPokemon().getStat(column - 7) : state.getPokemon().getStat(column - 7))
                         : (isSearcherState ? state.getSearcherPokemon().getIV(column - 7) : state.getPokemon().getIV(column - 7));
    case 13:
        return QString::fromStdString(
            Translator::getHiddenPower(isSearcherState ? state.getSearcherPokemon().getHiddenPower() : state.getPokemon().getHiddenPower()));
    case 14:
        return isSearcherState ? state.getSearcherPokemon().getHiddenPowerStrength() : state.getPokemon().getHiddenPowerStrength();
    case 15:
        return QString::fromStdString(Translator::getGender(isSearcherState ? state.getSearcherPokemon().getGender() : state.getPokemon().getGender()));
    case 16:
        return QString::fromStdString(Translator::getCharacteristic(isSearcherState ? state.getSearcherPokemon().getCharacteristic()
                                                                                   : state.getPokemon().getCharacteristic(),
                                                                    CharacteristicGeneration::Gen4));
    }

    return QVariant();
}

QVariant PokeRadarModel4::getPokemonHeader(int section) const
{
    switch (section)
    {
    case 0:
        return tr("Item");
    case 1:
        return tr("Slot");
    case 2:
        return tr("Level");
    case 3:
        return tr("PID");
    case 4:
        return tr("Shiny");
    case 5:
        return tr("Nature");
    case 6:
        return tr("Ability");
    case 7:
        return tr("HP");
    case 8:
        return tr("Atk");
    case 9:
        return tr("Def");
    case 10:
        return tr("SpA");
    case 11:
        return tr("SpD");
    case 12:
        return tr("Spe");
    case 13:
        return tr("Hidden");
    case 14:
        return tr("Power");
    case 15:
        return tr("Gender");
    case 16:
        return tr("Characteristic");
    }

    return QVariant();
}

int PokeRadarModel4::mapGeneratorColumn(int column) const
{
    return column;
}

QString PokeRadarModel4::getSkip(const PokeRadarState &state) const
{
    auto format = [](u8 skip) { return skip == 0 ? QStringLiteral("-") : QString("+%1").arg(skip); };
    if (state.getNoGraceSkip() == state.getGraceSkip())
    {
        return format(state.getNoGraceSkip());
    }

    return QStringLiteral("%1 / %2").arg(format(state.getGraceSkip()), format(state.getNoGraceSkip()));
}

QString PokeRadarModel4::getCoordinates(const PokeRadarState &state, int type) const
{
    if (!state.getPatchesVisible())
    {
        return QStringLiteral("-");
    }

    std::vector<PokeRadarPatch> patches;
    for (const auto &patch : state.getPatches())
    {
        if (!patch.active)
        {
            continue;
        }

        bool include = type == 3 ? patch.shiny : type == 2 ? patch.continueChain : patch.strong == (type == 1);
        if (include)
        {
            patches.emplace_back(patch);
        }
    }

    std::ranges::sort(patches, {}, [](const PokeRadarPatch &patch) { return std::pair { patch.y, patch.x }; });

    QStringList coordinates;
    for (const auto &patch : patches)
    {
        coordinates.append(QString("%1%2").arg(QChar('A' + patch.x)).arg(patch.y));
    }

    return coordinates.empty() ? QStringLiteral("-") : coordinates.join(QStringLiteral(", "));
}

QString PokeRadarModel4::getSearcherCoordinates(const PokeRadarState &state) const
{
    if (!state.getPatchesVisible() || !state.hasDisplayPatchType())
    {
        return QStringLiteral("-");
    }

    std::vector<PokeRadarPatch> patches;
    for (const auto &patch : state.getPatches())
    {
        if (patch.active && patch.strong == state.getDisplayPatchStrong() && patch.shiny == state.getDisplayPatchShiny())
        {
            patches.emplace_back(patch);
        }
    }

    std::ranges::sort(patches, {}, [](const PokeRadarPatch &patch) { return std::pair { patch.y, patch.x }; });

    QStringList coordinates;
    for (const auto &patch : patches)
    {
        coordinates.append(QString("%1%2").arg(QChar('A' + patch.x)).arg(patch.y));
    }

    return coordinates.empty() ? QStringLiteral("-") : coordinates.join(QStringLiteral(", "));
}

void PokeRadarModel4::setShowStats(bool flag)
{
    showStats = flag;
    if (rowCount() == 0)
    {
        return;
    }

    int first = 16;
    emit dataChanged(index(0, first), index(rowCount() - 1, first + 5), { Qt::DisplayRole });
}

void PokeRadarModel4::setShowContinue(bool flag)
{
    showContinue = flag;
}
