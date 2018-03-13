#ifndef PANDORA_HPP
#define PANDORA_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <libPokeFinder/RNG/LCRNG.hpp>
#include <libPokeFinder/Objects/Utilities.hpp>
#include <QStringList>
#include <QList>
#include <thread>

typedef uint32_t u32;

namespace Ui {
class Pandora;
}

class Pandora : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pandora(QWidget *parent = 0);
    ~Pandora();

signals:
    void updateGen3(QList<QStandardItem *>);

private slots:
    void on_pushButtonFindFRLGE_clicked();
    void updateGen3Model(QList<QStandardItem *> row);
    void on_checkBoxBattery_stateChanged(int arg1);
    void on_checkBoxPID_stateChanged(int arg1);
    void on_checkBoxTID_stateChanged(int arg1);
    void on_checkBoxSID_stateChanged(int arg1);
    void on_pushButtonFindRS_clicked();
    void on_radioButtonDateRS_toggled(bool checked);
    void on_radioButtonInitSeedRS_toggled(bool checked);
    void on_pushButtonFindXD_clicked();

private:
    Ui::Pandora *ui;

    bool isSearching = false;
    QStandardItemModel *gen3 = new QStandardItemModel(this);

    void setupModels();
    void searchFRLGE();
    void searchRS();
    void searchXDColo();
};

#endif // PANDORA_HPP
