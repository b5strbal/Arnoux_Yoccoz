#include "SmallFoliationTreeWidgetItem.h"
#include "../../math/Permutation.h"
#include "../../scc/SmallFoliation.h"
#include "../../../Eigen/Eigenvalues"
#include <sstream>

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


SmallFoliationTreeWidgetItem::SmallFoliationTreeWidgetItem(const balazs::TransverseCurve& tc,
                                                           std::size_t referenceZeroIndex,
                                                           bool flippedOver,
                                                           bool orientationReversing,
                                                           QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent),
    m_smallFoliation(tc, referenceZeroIndex, flippedOver, orientationReversing)
{
    std::size_t size = m_smallFoliation.lengths().size();
    for(std::size_t i = 0; i < size + 1; i++){
        new QTreeWidgetItem(this);
    }

    // lengths and twist
    for(std::size_t i = 0; i < size; i++){
        child(i)->setText(1, QString::number(static_cast<double>(m_smallFoliation.lengths()[i])));
    }
    child(size)->setText(1, QString::number(static_cast<double>(m_smallFoliation.twist())));

    // normalized lengths and twist
    for(std::size_t i = 0; i < size; i++){
        child(i)->setText(2, QString::number(static_cast<double>(m_smallFoliation.normalizedLengths()[i])));
    }
    child(size)->setText(2, QString::number(static_cast<double>(m_smallFoliation.normalizedTwist())));


    // permutation
    {
        QString s = "(";
        for(std::size_t i = 0; i < size; i++){
            s.append(QString::number(m_smallFoliation.permutation()[i]));
            s.append(" ");
        }
        s.chop(1);
        s.append(")");

        setText(3, s);
    }

    // transition matrix
    auto matrix = m_smallFoliation.transitionMatrix();
    for(int i = 0; i < matrix.rows(); i++){
        QString s;
        for(int j = 0; j < matrix.cols(); j++){
            s.append(QString::number(matrix(i, j)));
            s.append(" ");
        }
        s.chop(1);
        child(i)->setText(4, s);
    }

    QMap<balazs::SmallFoliation::WhatIsWrong, Qt::GlobalColor> color;
    color[balazs::SmallFoliation::Permutation_Does_Not_Match] = Qt::red;
    color[balazs::SmallFoliation::No_Appropriate_EigenValue] = Qt::magenta;
    color[balazs::SmallFoliation::No_Positive_EigenVector] = Qt::yellow;
    color[balazs::SmallFoliation::Nothing] = Qt::green;

    balazs::SmallFoliation::WhatIsWrong problem = m_smallFoliation.isGoodCandidate();
    setBackground(0, QBrush(color[problem]));
    if(problem != balazs::SmallFoliation::Nothing) return;




    // inverse matrix
    matrix = matrix.inverse();
    for(int i = 0; i < matrix.rows(); i++){
        QString s;
        for(int j = 0; j < matrix.cols(); j++){
            s.append(QString::number(matrix(i, j)));
            s.append(" ");
        }
        s.chop(1);
        child(i)->setText(5, s);
    }

    // eigenvalues and eigenvectors
    for(int i = 0; i < matrix.rows(); i++){
        setText(6 + i, string(m_smallFoliation.eigenvalues()[i]));
        setForeground(6 + i, QBrush(Qt::blue));

        for(int j = 0; j < matrix.rows(); j++){
            child(j)->setText(6 + i, string(m_smallFoliation.eigenvectors().col(i)[j]));
        }
    }





    // strip heights
//    for(std::size_t i = 0; i < m_smallFoliation.normalizedLengths().size(); i++){
//        QString s;
//        for(std::size_t j = 0; j < m_smallFoliation.normalizedLengths().size(); j++){
//            s.append(QString::number(sf.stripHeights().at(balazs::HDirection::Right)[i][j]));
//            s.append(" ");
//        }
//        s.chop(1);
//        item->setText(1, s);
//    }

    // strip heights
//    for(std::size_t i = 0; i < sf.normalizedLengths().size(); i++){
//        s.clear();
//        for(std::size_t j = 0; j < sf.normalizedLengths().size(); j++){
//            s.append(QString::number(sf.stripHeights().at(balazs::HDirection::Left)[i][j]));
//            s.append(" ");
//        }
//        s.chop(1);
//        child(i)->setText(4, s);
//    }




}
