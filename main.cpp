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

    return a.exec();
}
