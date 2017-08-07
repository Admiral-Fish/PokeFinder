#include "mainwindow.h"
#include <QApplication>
#include "Objects/NatureLock.hpp"
#include "Objects/Utilities.hpp"
#include "Objects/LCRNG.hpp"
#include "Objects/SFMT.hpp"
#include "Objects/TinyMT.hpp"
#include <iostream>
#include <boost/date_time/gregorian/gregorian_types.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    uint32_t state[4] = { 0x78A495AE, 0x60127F96, 0x0D6F15E9, 0x1969DE6C };
    TinyMT tiny = TinyMT(state);

    for (int i = 0; i < 10; i++)
        std::cout << std::hex << tiny.Nextuint() << std::endl;

    return a.exec();
}
