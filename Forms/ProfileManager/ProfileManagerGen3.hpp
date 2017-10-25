#ifndef PROFILEMANAGERGEN3_HPP
#define PROFILEMANAGERGEN3_HPP

#include <QWidget>
#include <Forms/ProfileManager/ProfileManagerGen3NewEdit.hpp>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QObject>
#include <libPokeFinder/Gen3/ProfileGen3.hpp>

namespace Ui {
class ProfileManagerGen3;
}

class ProfileManagerGen3 : public QDialog
{
    Q_OBJECT

signals:
    void updateProfiles();

public:
    explicit ProfileManagerGen3(QWidget *parent = 0);
    ~ProfileManagerGen3();

private slots:
    void on_pushButtonNew_clicked();

    void on_pushButtonOk_clicked();

    void registerProfile(QString profileName, int version, int language, uint32_t tid, uint32_t sid, bool deadBattery);

    void on_pushButtonEdit_clicked();

    void on_pushButtonDelete_clicked();

private:
    Ui::ProfileManagerGen3 *ui;

    void updateTable(std::vector<QList<QStandardItem *>> rows);
};

#endif // PROFILEMANAGERGEN3_HPP
