#ifndef TRANSVERSECURVETREEWIDGETITEM_H
#define TRANSVERSECURVETREEWIDGETITEM_H

#include <QTreeWidgetItem>

namespace balazs{
    class TransverseCurve;
}

class TransverseCurveTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit TransverseCurveTreeWidgetItem(const balazs::TransverseCurve& tc, QTreeWidgetItem *parent = 0);
    const balazs::TransverseCurve& transverseCurve() const { return m_transverseCurve; }
signals:
    
public slots:

private:
    const balazs::TransverseCurve& m_transverseCurve;
    
};

#endif // TRANSVERSECURVETREEWIDGETITEM_H
