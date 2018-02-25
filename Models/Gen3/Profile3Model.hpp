#ifndef PROFILE3MODEL_HPP
#define PROFILE3MODEL_HPP


#include <QAbstractTableModel>
#include <libPokeFinder/Gen3/Profile3.hpp>

class Profile3Model : public QAbstractTableModel
{

    Q_OBJECT

private:
    vector<Profile3> model;

public:
    Profile3Model(QObject *parent);
    void setModel(vector<Profile3> profiles);
    void addItem(Profile3 profile);
    void updateProfile(Profile3 profile, int row);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Profile3 getProfile(int index);
    void removeProfile(int index);

};

#endif // PROFILE3MODEL_HPP
