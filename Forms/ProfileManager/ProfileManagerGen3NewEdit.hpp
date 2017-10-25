#ifndef PROFILEMANAGERGEN3NEWEDIT_HPP
#define PROFILEMANAGERGEN3NEWEDIT_HPP

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QObject>
#include <QStandardItemModel>
#include <QList>

namespace Ui {
class ProfileManagerGen3NewEdit;
}

class ProfileManagerGen3NewEdit : public QDialog
{
    Q_OBJECT

signals:
    void newProfile(QString, int, int, uint32_t, uint32_t, bool);

public:
    explicit ProfileManagerGen3NewEdit(QWidget *parent = 0);
    explicit ProfileManagerGen3NewEdit(QString profileName, int version, int language, uint32_t tid, uint32_t sid, bool deadBattery, QWidget *parent = 0);
    ~ProfileManagerGen3NewEdit();

private slots:
    void on_pushButtonAccept_clicked();

    void on_pushButtonCancel_clicked();

    void on_comboBoxVersion_currentIndexChanged(int index);

private:
    Ui::ProfileManagerGen3NewEdit *ui;
};

#endif // PROFILEMANAGERGEN3NEWEDIT_HPP
