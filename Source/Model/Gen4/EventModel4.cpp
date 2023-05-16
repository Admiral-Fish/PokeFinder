#include "EventModel4.hpp"
#include <Core/Util/Translator.hpp>

EventGeneratorModel4::EventGeneratorModel4(QObject *parent) : TableModel(parent), showStats(false)
{
}

int EventGeneratorModel4::columnCount(const QModelIndex &parent) const
{
    return 9;
}

QVariant EventGeneratorModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return showStats ? state.getStat(column - 1) : state.getIV(column - 1);
        case 7:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 8:
            return state.getHiddenPowerStrength();
        }
    }

    return QVariant();
}

QVariant EventGeneratorModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void EventGeneratorModel4::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 1), index(rowCount(), 6), { Qt::DisplayRole });
}

EventSearcherModel4::EventSearcherModel4(QObject *parent) : TableModel(parent), showStats(false)
{
}

int EventSearcherModel4::columnCount(const QModelIndex &parent) const
{
    return 10;
}

QVariant EventSearcherModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        switch (column)
        {
        case 0:
            return QString::number(state.getSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
            return state.getAdvances();
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            return showStats ? state.getStat(column - 2) : state.getIV(column - 2);
        case 8:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 9:
            return state.getHiddenPowerStrength();
        }
    }

    return QVariant();
}

QVariant EventSearcherModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void EventSearcherModel4::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const SearcherState4 &state1, const SearcherState4 &state2) {
                return flag ? state1.getSeed() < state2.getSeed() : state1.getSeed() > state2.getSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const SearcherState4 &state1, const SearcherState4 &state2) {
                return flag ? state1.getAdvances() < state2.getAdvances() : state1.getAdvances() > state2.getAdvances();
            });
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            std::sort(model.begin(), model.end(), [flag, column](const SearcherState4 &state1, const SearcherState4 &state2) {
                return flag ? state1.getIV(column - 6) < state2.getIV(column - 6) : state1.getIV(column - 6) > state2.getIV(column - 6);
            });
            break;
        case 8:
            std::sort(model.begin(), model.end(), [flag](const SearcherState4 &state1, const SearcherState4 &state2) {
                return flag ? state1.getHiddenPower() < state2.getHiddenPower() : state1.getHiddenPower() > state2.getHiddenPower();
            });
            break;
        case 9:
            std::sort(model.begin(), model.end(), [flag](const SearcherState4 &state1, const SearcherState4 &state2) {
                return flag ? state1.getHiddenPowerStrength() < state2.getHiddenPowerStrength()
                            : state1.getHiddenPowerStrength() > state2.getHiddenPowerStrength();
            });
            break;
        }
    }
}

void EventSearcherModel4::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 2), index(rowCount(), 7), { Qt::DisplayRole });
}
