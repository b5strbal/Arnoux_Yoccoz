#include "TransverseCurveTreeWidgetItem.h"
#include "../../math/Permutation.h"
#include "../../intex/Mod1NumberIntExchange.h"
//#include "../../scc/TransverseCurveData.h"

TransverseCurveTreeWidgetItem::TransverseCurveTreeWidgetItem(const balazs::TransverseCurve& tc, QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent), m_transverseCurve(tc)
{

    // length
    setText(1, QString::number(static_cast<double>(m_transverseCurve.length())));



    // singularity permutation
//    QString string = "(";
//    for(std::size_t i = 0; i < m_transverseCurveData.singularityPermutation().size(); i++){
//        string.append(QString::number(m_transverseCurveData.singularityPermutation()[i]));
//        string.append(" ");
//    }
//    string.chop(1);
//    string.append(")");

//    setText(2, string);



//    // new lengths
//    for(std::size_t i = 0; i < m_transverseCurveData.singularityPermutation().size(); i++){
//        QTreeWidgetItem* item = new QTreeWidgetItem(this);
//        item->setText(3, QString::number(static_cast<double>(m_transverseCurveData.lengths()[i])));
//    }


//    // new permutation
//    string = "(";
//    for(std::size_t i = 0; i < m_transverseCurveData.intervalPermutation().size(); i++){
//        string.append(QString::number(m_transverseCurveData.intervalPermutation()[i]));
//        string.append(" ");
//    }
//    string.chop(1);
//    string.append(")");

//    setText(4, string);


//    // new twist
//    setText(5, QString::number(static_cast<double>(m_transverseCurveData.twist())));




//    // normalized lengths
//    for(std::size_t i = 0; i < m_transverseCurveData.singularityPermutation().size(); i++){
//        child(i)->setText(6, QString::number(static_cast<double>(m_transverseCurveData.normalizedLengths()[i])));
//    }


//    // normalized twist
//    setText(7, QString::number(static_cast<double>(m_transverseCurveData.normalizedTwist())));

}

const balazs::TransverseCurve &TransverseCurveTreeWidgetItem::transverseCurve() const
{
    return m_transverseCurve;
}
