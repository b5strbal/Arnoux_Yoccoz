#include "SmallFoliationTreeWidgetItem.h"
#include "../../math/Permutation.h"
#include "../../scc/SmallFoliation.h"
#include "../../../Eigen/Eigenvalues"
#include <sstream>
#include "qt.h"



SmallFoliationTreeWidgetItem::SmallFoliationTreeWidgetItem(const balazs::SmallFoliation& sf,
                                                           QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent),
    m_smallFoliation(sf)
{
    setBackground(0, QBrush(Qt::green));

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
    setText(3, string(m_smallFoliation.permutation()));

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


    // transpose eigenvalues and eigenvectors
    matrix.transposeInPlace();
    Eigen::EigenSolver<Eigen::MatrixXd> es(matrix);
    for(int i = 0; i < matrix.rows(); i++){
        setText(6 + i + matrix.rows(), string(es.eigenvalues()[i]));
        setForeground(6 + i + matrix.rows(), QBrush(Qt::blue));

        for(int j = 0; j < matrix.rows(); j++){
            child(j)->setText(6 + i + matrix.rows(), string(es.eigenvectors().col(i)[j]));
        }
    }
    matrix.transposeInPlace();



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
