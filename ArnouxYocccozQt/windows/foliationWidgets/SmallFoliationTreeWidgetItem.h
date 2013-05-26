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
    explicit SmallFoliationTreeWidgetItem(const balazs::SmallFoliation& sf,
                                          QTreeWidgetItem *parent = 0);

    
private:
    const balazs::SmallFoliation& m_smallFoliation;
};

#endif // SMALLFOLIATIONTREEWIDGETITEM_H
