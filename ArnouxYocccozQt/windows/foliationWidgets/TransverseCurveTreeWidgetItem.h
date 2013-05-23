#ifndef TRANSVERSECURVETREEWIDGETITEM_H
#define TRANSVERSECURVETREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include "../../scc/TransverseCurveData.h"

namespace balazs{
    class TransverseCurveData;
}

class TransverseCurveTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit TransverseCurveTreeWidgetItem(balazs::TransverseCurveData&& tcData, QTreeWidgetItem *parent = 0);
    const balazs::TransverseCurveData& transverseCurveData() const;
signals:
    
public slots:

private:
    balazs::TransverseCurveData m_transverseCurveData;
    
};

#endif // TRANSVERSECURVETREEWIDGETITEM_H
