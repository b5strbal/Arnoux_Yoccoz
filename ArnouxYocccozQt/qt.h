#ifndef QT_H
#define QT_H

#include <vector>
#include <QStringList>
#include <complex>
#include "intex/Mod1NumberIntExchange.h"


namespace balazs{
    class Permutation;
    class Mod1NumberIntExchange;
    class Mod1Number;
}


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
        string.append(QString::number(static_cast<double>(x)));
        string.append(" ");
    }
    string.chop(1);
    return string;
}

//template <>
//QString& operator<<(QString& string, const std::vector<balazs::Mod1Number>& vec){
//    for(auto &x : vec){
//        string.append(QString::number(static_cast<double>(x)));
//        string.append(" ");
//    }
//    string.chop(1);
//    return string;
//}


//template <>
//QString& operator<<(QString& string, const std::vector<balazs::Mod1NumberIntExchange>& vec){
//    for(auto &x : vec){
//        string.append(QString::number(static_cast<double>(x)));
//        string.append(" ");
//    }
//    string.chop(1);
//    return string;
//}


QString string(const std::complex<double>& cd);
QString string(const balazs::Permutation& perm);

#endif // QT_H
