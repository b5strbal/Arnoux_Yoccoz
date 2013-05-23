#include "TransverseCurveTreeWidgetItem.h"
#include "../../math/Permutation.h"
#include "../../intex/Mod1NumberIntExchange.h"
//#include "../../scc/TransverseCurveData.h"

TransverseCurveTreeWidgetItem::TransverseCurveTreeWidgetItem(balazs::TransverseCurveData &&tcData, QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent), m_transverseCurveData(std::move(tcData))
{
    setText(1, QString::number(static_cast<double>(m_transverseCurveData.length())));  // length

    QString perm = "(";
    for(std::size_t i = 0; i < m_transverseCurveData.singularityPermutation().size(); i++){
        perm.append(QString::number(m_transverseCurveData.singularityPermutation()[i]));
        perm.append(" ");
    }
    perm.chop(1);
    perm.append(")");

    setText(2, perm);

    for(std::size_t i = 0; i < m_transverseCurveData.singularityPermutation().size(); i++){
        QTreeWidgetItem* item = new QTreeWidgetItem(this);
        item->setText(3, QString::number(static_cast<double>(m_transverseCurveData.lengths()[i])));
    }
}

const balazs::TransverseCurveData &TransverseCurveTreeWidgetItem::transverseCurveData() const
{
    return m_transverseCurveData;
}
