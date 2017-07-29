#include "mainwindow.h"
#include <QApplication>
#include "Objects/NatureLock.hpp"
#include "Objects/Utilities.hpp"
#include "Objects/LCRNG.hpp"
#include <iostream>
#include <boost/date_time/gregorian/gregorian_types.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    LCRNG64 rng = LCRNG64(0);
    for (int i = 0; i < 10; i++)
        std::cout << std::hex << rng.prev32Bit() << std::endl;

    return a.exec();
}
