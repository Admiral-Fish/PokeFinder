#ifndef HIDDENGROTTOMODEL_HPP
#define HIDDENGROTTOMODEL_HPP

#include <Core/Gen5/States/SearcherState5.hpp>
#include <Core/Gen5/States/HiddenGrottoState.hpp>
#include <Forms/Models/TableModel.hpp>

class HiddenGrottoGeneratorModel5 : public TableModel<HiddenGrottoState>
{
    Q_OBJECT
public:
    explicit HiddenGrottoGeneratorModel5(QObject *parent);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header = {  tr("Advances"), tr("Chatot"),   tr("PID"),  tr("Nature"),   tr("Ability"),  tr("Gender")};
};

class HiddenGrottoSearcherModel5 : public TableModel<SearcherState5<HiddenGrottoState>>
{
    Q_OBJECT
public:
    explicit HiddenGrottoSearcherModel5(QObject *parent);
    void sort(int column, Qt::SortOrder order) override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header = {  tr("Seed"),  tr("Group"),    tr("Slot"),     tr("Advances"), tr("Gender"),   tr("Date/Time"),
                            tr("Timer0"),   tr("Buttons")};
};

#endif // HIDDENGROTTOMODEL_HPP
