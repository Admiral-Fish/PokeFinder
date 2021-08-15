#ifndef HIDDENGROTTOMODEL_HPP
#define HIDDENGROTTOMODEL_HPP

#include <Core/Gen5/States/HiddenGrottoState.hpp>
#include <Core/Gen5/States/SearcherState5.hpp>
#include <Forms/Models/TableModel.hpp>

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
    QStringList header
        = { tr("Seed"), tr("Group"), tr("Slot"), tr("Advances"), tr("Gender"), tr("Date/Time"), tr("Timer0"), tr("Buttons") };
};

#endif // HIDDENGROTTOMODEL_HPP
