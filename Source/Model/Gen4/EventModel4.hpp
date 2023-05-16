#ifndef EVENTMODEL4_HPP
#define EVENTMODEL4_HPP

#include <Core/Gen4/States/State4.hpp>
#include <Core/Parents/States/State.hpp>
#include <Model/TableModel.hpp>

class EventGeneratorModel4 : public TableModel<GeneratorState>
{
    Q_OBJECT
public:
    EventGeneratorModel4(QObject *parent);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void setShowStats(bool flag);

private:
    QStringList header = { tr("Advances"), tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power") };
    bool showStats;
};

class EventSearcherModel4 : public TableModel<SearcherState4>
{
    Q_OBJECT
public:
    EventSearcherModel4(QObject *parent);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation, int role) const override;

    void sort(int column, Qt::SortOrder order) override;

public slots:
    void setShowStats(bool flag);

private:
    QStringList header
        = { tr("Seed"), tr("Advances"), tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power") };
    bool showStats;
};

#endif // EVENTMODEL4_HPP
