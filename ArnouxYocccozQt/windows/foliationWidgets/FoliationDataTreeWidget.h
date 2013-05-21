#ifndef FOLIATIONDATATREEWIDGET_H
#define FOLIATIONDATATREEWIDGET_H

#include <QTreeWidget>

namespace balazs{
    class Foliation;
}

class FoliationDataTreeWidget : public QTreeWidget{
    Q_OBJECT

public:
    explicit FoliationDataTreeWidget(const balazs::Foliation& foliation, QWidget *parent = 0);
    QSize sizeHint() const { return QSize(300, 300); }
    void update();
protected:

private:
    const balazs::Foliation& m_foliation;

    QTreeWidgetItem* eulerCharItem;
    QTreeWidgetItem* genusItem;
    QTreeWidgetItem* singListAbDiffItem;
    QTreeWidgetItem* singListProngsItem;
    QTreeWidgetItem* lengthsItem;
    QTreeWidgetItem* topDivPointsItem;
    QTreeWidgetItem* bottomDivPointsItem;
};


#endif // FOLIATIONDATATREEWIDGET_H
