#ifndef UNDERGROUNDMODEL_HPP
#define UNDERGROUNDMODEL_HPP

#include <Core/Gen8/States/UndergroundState.hpp>
#include <Model/TableModel.hpp>

class UndergroundModel : public TableModel<UndergroundState>
{
public:
    UndergroundModel(QObject *parent);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void setShowStats(bool flag);

private:
    QStringList header = { tr("Advances"), tr("Item"), tr("Level"), tr("PID"), tr("Shiny"), tr("Nature"), tr("Ability"), tr("HP"),
                           tr("Atk"),      tr("Def"),  tr("SpA"),   tr("SpD"), tr("Spe"),   tr("Hidden"), tr("Gender") };
    bool showStats;
};

#endif // UNDERGROUNDMODEL_HPP
