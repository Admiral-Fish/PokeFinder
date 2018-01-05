#ifndef RESEARCHERMODEL_HPP
#define RESEARCHERMODEL_HPP

#include <QAbstractTableModel>
#include <Util/ResearcherFrame.hpp>
#include <vector>

using namespace std;
typedef uint32_t u32;
typedef uint64_t u64;

class ResearcherModel : public QAbstractTableModel
{

private:
    vector<ResearcherFrame> model;
    bool flag;

public:
    ResearcherModel(QObject *parent, bool is64Bit);
    void SetModel(vector<ResearcherFrame>);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

};

#endif // RESEARCHERMODEL_HPP


