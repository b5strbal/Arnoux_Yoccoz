#include "qt.h"
#include "intex/Mod1NumberIntExchange.h"
#include "math/Permutation.h"

QString string(const std::complex<double>& cd){
    QString s;
    s.append(QString::number(cd.real()));
    if(cd.imag() == 0){
        return s;
    }
    if(cd.imag() > 0){
        s.append(" + ");
        s.append(QString::number(cd.imag()));
    } else {
        s.append(" - ");
        s.append(QString::number(-cd.imag()));
    }
    s.append("i");

    return s;
}

QString string(const balazs::Permutation& perm){
    QString s = "(";
    for(std::size_t i = 0; i < perm.size(); i++){
        s.append(QString::number(perm[i]));
        s.append(" ");
    }
    s.chop(1);
    s.append(")");
    return s;
}
