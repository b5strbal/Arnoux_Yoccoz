#ifndef FOLIATIONDATATABLEWIDGET_H
#define FOLIATIONDATATABLEWIDGET_H

#include <QTreeWidget>

namespace balazs{
    class Foliation;
}

class FoliationDataTableWidget : public QTreeWidget{
    Q_OBJECT

public:
    explicit FoliationDataTableWidget(const balazs::Foliation& foliation, QWidget *parent = 0);
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


#endif // FOLIATIONDATATABLEWIDGET_H
