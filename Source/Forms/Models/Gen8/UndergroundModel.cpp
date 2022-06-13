#include "UndergroundModel.hpp"
#include <Core/Util/Translator.hpp>

UndergroundModel::UndergroundModel(QObject *parent) : TableModel<UgState>(parent)
{
}

int UndergroundModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 14;
}

QVariant UndergroundModel::data(const QModelIndex &index, int role) const
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
        {
            u8 item = state.getItem();
            return item >= 80 ? "No" : item < 60 ? "5%" : "50%";
        }
        case 2:
            return QString::fromStdString(*Translator::getSpecies(state.getSpecies()));
        case 3:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 4:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 5:
            return QString::fromStdString(*Translator::getNature(state.getNature()));
        case 6:
        {
            u8 ability = state.getAbility();
            if (ability == 0 || ability == 1)
            {
                return ability;
            }
            return "H";
        }
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            return state.getIV(static_cast<u8>(column - 7));
        case 13:
            return QString::fromStdString(*Translator::getGender(state.getGender()));
        }
    }

    return QVariant();
}

QVariant UndergroundModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
