#include "TransverseCurveTreeWidgetItem.h"
#include "../../scc/TransverseCurve.h"

TransverseCurveTreeWidgetItem::TransverseCurveTreeWidgetItem(const balazs::TransverseCurve &tc, QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent), m_transverseCurve(tc)
{
    setText(1, QString::number(static_cast<double>(tc.length())));  // length
}
