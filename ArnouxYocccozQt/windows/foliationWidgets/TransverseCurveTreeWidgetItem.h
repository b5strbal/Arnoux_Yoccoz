#ifndef TRANSVERSECURVETREEWIDGETITEM_H
#define TRANSVERSECURVETREEWIDGETITEM_H

#include <QTreeWidgetItem>

#include "../../scc/TransverseCurve.h"

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
    
};

#endif // TRANSVERSECURVETREEWIDGETITEM_H
