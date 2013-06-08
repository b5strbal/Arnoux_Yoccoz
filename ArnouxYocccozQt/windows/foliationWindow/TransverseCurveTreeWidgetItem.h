#ifndef TRANSVERSECURVETREEWIDGETITEM_H
#define TRANSVERSECURVETREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <list>

#include "../../scc/TransverseCurve.h"
#include "scc/SmallFoliation.h"

//namespace balazs{
//    class SmallFoliation;
//}

class TransverseCurveTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit TransverseCurveTreeWidgetItem(const balazs::TransverseCurve& tc, QTreeWidgetItem *parent = 0);
    const balazs::TransverseCurve& transverseCurve() const;
    void generateCandidates();
signals:
    
public slots:

private:
    const balazs::TransverseCurve& m_transverseCurve;
    std::list<balazs::SmallFoliation> m_smallFoliations;
};

#endif // TRANSVERSECURVETREEWIDGETITEM_H
