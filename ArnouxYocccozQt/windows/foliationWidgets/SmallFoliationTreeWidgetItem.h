#ifndef SMALLFOLIATIONTREEWIDGETITEM_H
#define SMALLFOLIATIONTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include "../../scc/SmallFoliation.h"

namespace balazs{
    class TransverseCurve;
}

class SmallFoliationTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit SmallFoliationTreeWidgetItem(const balazs::TransverseCurve& tc,
                                          std::size_t referenceZeroIndex,
                                          bool flippedOver,
                                          bool orientationReversing,
                                          QTreeWidgetItem *parent = 0);

    
private:
    balazs::SmallFoliation m_smallFoliation;

};

#endif // SMALLFOLIATIONTREEWIDGETITEM_H
