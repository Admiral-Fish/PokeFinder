#include "HiddenGrottoModel.hpp"
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>

HiddenGrottoGeneratorModel5::HiddenGrottoGeneratorModel5(QObject *parent) : TableModel<HiddenGrottoState>(parent)
{
}

int HiddenGrottoGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    return 6;
}

QVariant HiddenGrottoGeneratorModel5::data(const QModelIndex &index, int role) const
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
            return QString::fromStdString(Utilities::getChatot64(state.getSeed()));
        case 2:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 3:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 4:
        {
            u8 ability = state.getAbility();
            if (ability == 0 || ability == 1)
            {
                return ability;
            }
            return "H";
        }
        case 5:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        }
    }
    return QVariant();
}

QVariant HiddenGrottoGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

HiddenGrottoSearcherModel5::HiddenGrottoSearcherModel5(QObject *parent) : TableModel<SearcherState5<HiddenGrottoState>>(parent)
{
}

void HiddenGrottoSearcherModel5::sort(int column, Qt::SortOrder order)
{
    if(!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch(column)
        {
            case 0:
                std::sort(model.begin(), model.end(), [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                    return flag ? state1.getInitialSeed() < state2.getInitialSeed() : state1.getInitialSeed() > state2.getInitialSeed();
                });
                break;
            case 1:
                std::sort(model.begin(), model.end(), [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                    return flag ? state1.getState().getGroup() < state2.getState().getGroup()
                                : state1.getState().getGroup() > state2.getState().getGroup();
                });
                break;
            case 2:
                std::sort(model.begin(), model.end(), [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                    return flag ? state1.getState().getSlot() < state2.getState().getSlot()
                                : state1.getState().getSlot() > state2.getState().getSlot();
                });
                break;
            case 3:
                std::sort(model.begin(), model.end(), [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                    return flag ? state1.getState().getAdvances() < state2.getState().getAdvances()
                                : state1.getState().getAdvances() > state2.getState().getAdvances();
                });
                break;
            case 4:
                std::sort(model.begin(), model.end(), [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                    return flag ? state1.getState().getGender() < state2.getState().getGender()
                                : state1.getState().getGender() > state2.getState().getGender();
                });
                break;
            case 5:
                std::sort(model.begin(), model.end(), [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                    return flag ? state1.getDateTime() < state2.getDateTime()
                                : state1.getDateTime() > state2.getDateTime();
                });
                break;
            case 6:
                std::sort(model.begin(), model.end(), [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                    return flag ? state1.getTimer0() < state2.getTimer0()
                                : state1.getTimer0() > state2.getTimer0();
                });
                break;
            case 7:
                std::sort(model.begin(), model.end(), [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                    return flag ? state1.getButtons() < state2.getButtons()
                                : state1.getButtons() > state2.getButtons();
                });
                break;
        }
    }
}

int HiddenGrottoSearcherModel5::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 8;
}

QVariant HiddenGrottoSearcherModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &display = model[index.row()];
        const auto &state = display.getState();
        int column = index.column();
        switch(column)
        {
        case 0:
            return QString::number(display.getInitialSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
            return state.getGroup();
        case 2:
            return state.getSlot();
        case 3:
            return state.getAdvances();
        case 4:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 5:
            return QString::fromStdString(display.getDateTime().toString());
        case 6:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 7:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }
    return QVariant();
}

QVariant HiddenGrottoSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
