#ifndef UNDERGROUNDMODEL_HPP
#define UNDERGROUNDMODEL_HPP

#include <Core/Gen8/States/UgState.hpp>
#include <Forms/Models/TableModel.hpp>

class UndergroundModel : public TableModel<UgState>
{
    Q_OBJECT
public:
    explicit UndergroundModel(QObject *parent);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header = { tr("Advances"), tr("Item"), tr("Species"), tr("PID"), tr("Shiny"), tr("Nature"), tr("Ability"),
                           tr("HP"),       tr("Atk"),  tr("Def"),     tr("SpA"), tr("SpD"),   tr("Spe"),    tr("Gender") };
};

#endif // UNDERGROUNDMODEL_HPP
