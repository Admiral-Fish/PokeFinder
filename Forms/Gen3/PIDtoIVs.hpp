#ifndef PIDTOIVS_HPP
#define PIDTOIVS_HPP

#include <QMainWindow>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QList>
#include <libPokeFinder/RNG/RNGCache.hpp>
#include <libPokeFinder/Objects/Method.hpp>
#include <QDebug>

typedef uint32_t u32;

using std::vector;
namespace Ui {
class PIDtoIVs;
}

class PIDtoIVs : public QMainWindow
{
    Q_OBJECT

public:
    explicit PIDtoIVs(QWidget *parent = 0);
    ~PIDtoIVs();

private slots:
    void on_pushButtonGenerate_clicked();

private:
    Ui::PIDtoIVs *ui;

    QStandardItemModel *m = new QStandardItemModel(this);
    char low8[0x10000];
    bool flags[0x10000];
    RNGCache *cache;

    void setupModels();
    void calcFromPID(u32 pid);
    void calcMethod124(u32 pid);
    void calcMethodXD(u32 pid);
    QString calcIVs1(u32 iv1);
    QString calcIVs2(u32 iv1);
    QString calcIVs4(u32 iv1);
    QString calcIVsXD(u32 iv1, u32 iv2);
    void addSeed(u32 seed, u32 iv1);
    void addSeedGC(u32 seed, u32 iv1, u32 iv2);
    u32 forward(u32 seed);
    u32 reverse(u32 seed);
    u32 reverseXD(u32 seed);

};

#endif // PIDTOIVS_HPP
