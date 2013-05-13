#ifndef QT_H
#define QT_H

#include <vector>
#include <QStringList>
#include "intex/Mod1NumberIntExchange.h"

template <typename NumberType>
QStringList& operator<<(QStringList& stringList, const std::vector<NumberType>& vec){
    for(auto &x : vec){
        stringList << QString::number(x);
    }
    return stringList;
}

template <typename NumberType>
QString& operator<<(QString& string, const std::vector<NumberType>& vec){
    for(auto &x : vec){
        string.append(QString::number(x));
        string.append(" ");
    }
    string.chop(1);
    return string;
}

template <>
QString& operator<<(QString& string, const std::vector<balazs::Mod1Number>& vec){
    for(auto &x : vec){
        string.append(QString::number(static_cast<double>(x)));
        string.append(" ");
    }
    string.chop(1);
    return string;
}


template <>
QString& operator<<(QString& string, const std::vector<balazs::Mod1NumberIntExchange>& vec){
    for(auto &x : vec){
        string.append(QString::number(static_cast<double>(x)));
        string.append(" ");
    }
    string.chop(1);
    return string;
}



#endif // QT_H
