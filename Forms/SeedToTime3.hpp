#ifndef SEEDTOTIME3_HPP
#define SEEDTOTIME3_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QList>
#include <QStandardItem>

namespace Ui {
class SeedToTime3;
}

class SeedToTime3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit SeedToTime3(QWidget *parent = 0);
    ~SeedToTime3();

private slots:
    void on_pushButtonFind_clicked();

private:
    Ui::SeedToTime3 *ui;
    QStandardItemModel *m;
    QDateTime start;
    QDateTime addTime;

    void setupModels();
    uint32_t originSeed(uint32_t seed);
    uint32_t reverse(uint32_t seed);
    void seedToTime(uint32_t seed, uint32_t year);
};

#endif // SEEDTOTIME3_HPP
