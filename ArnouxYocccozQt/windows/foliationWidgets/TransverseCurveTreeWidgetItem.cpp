#include "TransverseCurveTreeWidgetItem.h"
#include "fol/Foliation.h"
#include "SmallFoliationTreeWidgetItem.h"
//#include "../../intex/Mod1NumberIntExchange.h"


TransverseCurveTreeWidgetItem::TransverseCurveTreeWidgetItem(const balazs::TransverseCurve& tc, QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent), m_transverseCurve(tc)
{

    // length
    setText(0, QString::number(static_cast<double>(m_transverseCurve.length())));
}

const balazs::TransverseCurve &TransverseCurveTreeWidgetItem::transverseCurve() const
{
    return m_transverseCurve;
}


void TransverseCurveTreeWidgetItem::generateCandidates()
{
    for(bool flippedOver : {false, true}){
        for(bool orientationReversing : {false, true}){
            for(std::size_t i = 0; i < m_transverseCurve.foliation().numIntervals(); i++){
                new SmallFoliationTreeWidgetItem(m_transverseCurve, i, flippedOver, orientationReversing, this);
            }
        }
    }
}
