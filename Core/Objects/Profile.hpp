#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <QString>
#include <QDomDocument>
#include <QApplication>
#include <QFile>
#include <QTextStream>

class Profile
{
public:

    QString profileName;
    QString version;
    QString language;
    bool valid;

};

#endif // PROFILE_HPP
