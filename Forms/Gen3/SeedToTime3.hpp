#ifndef SEEDTOTIME3_HPP
#define SEEDTOTIME3_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDateTime>
#include <QList>
#include <QStandardItem>
#include <QMessageBox>
#include <libPokeFinder/RNG/LCRNG.hpp>

typedef uint32_t u32;

namespace Ui {
class SeedToTime3;
}

class SeedToTime3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

public:
    explicit SeedToTime3(QWidget *parent = 0);
    ~SeedToTime3();

private slots:
    void on_pushButtonFind_clicked();

private:
    Ui::SeedToTime3 *ui;
    QStandardItemModel *m = new QStandardItemModel(this);

    void setupModels();
    u32 originSeed(u32 seed);
    void seedToTime(u32 seed, u32 year);
};

#endif // SEEDTOTIME3_HPP
