#ifndef IVTOPID_SID_SEED_HPP
#define IVTOPID_SID_SEED_HPP

#include <QMainWindow>
#include <libPokeFinder/Objects/Nature.hpp>
#include <QStandardItemModel>
#include <QStandardItem>
#include <Forms/IVFilter.hpp>
#include <libPokeFinder/RNG/LCRNG.hpp>
#include <QList>

typedef uint32_t u32;

namespace Ui {
class IVtoPID_SID_SEED;
}

class IVtoPID_SID_SEED : public QMainWindow
{
    Q_OBJECT

public:
    explicit IVtoPID_SID_SEED(QWidget *parent = 0);
    ~IVtoPID_SID_SEED();

private slots:
    void on_pushButtonFind_clicked();

private:
    Ui::IVtoPID_SID_SEED *ui;

    QStandardItemModel *m = new QStandardItemModel();

    void setupModels();
    void getSeeds(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe, u32 nature, u32 tid);
};

#endif // IVTOPID_SID_SEED_HPP
