#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QHeaderView>
#include <cstdint>
#include <vector>
#include "Core/Objects/LCRNG.hpp"
#include "Core/Gen3/FrameGen3.hpp"
#include "Core/Gen3/GeneratorGen3.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_generate_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
