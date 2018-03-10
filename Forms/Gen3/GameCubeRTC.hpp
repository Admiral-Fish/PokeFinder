#ifndef GAMECUBERTC_HPP
#define GAMECUBERTC_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <thread>
#include <QList>
#include <libPokeFinder/RNG/LCRNG.hpp>
#include <QDateTime>
#include <QDate>
#include <QTime>

typedef uint32_t u32;

namespace Ui {
class GameCubeRTC;
}

class GameCubeRTC : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameCubeRTC(QWidget *parent = 0);
    ~GameCubeRTC();

signals:
    void updateView(QList<QStandardItem *>);

private slots:
    void on_pushButtonSearch_clicked();
    void updateTableView(QList<QStandardItem *> row);

private:
    Ui::GameCubeRTC *ui;
    bool isSearching = false;
    QStandardItemModel *m = new QStandardItemModel(this);
    QDateTime date = QDateTime(QDate(2000, 1, 1), QTime(0, 0));

    void setupModels();
    void calcRTC();
};

#endif // GAMECUBERTC_HPP
