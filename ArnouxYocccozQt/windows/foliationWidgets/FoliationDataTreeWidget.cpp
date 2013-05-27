#include "FoliationDataTreeWidget.h"
#include "../../fol/Foliation.h"
#include "../../qt.h"

FoliationDataTreeWidget::FoliationDataTreeWidget(const balazs::Foliation &foliation, QWidget *parent) :
    QTreeWidget(parent),
    m_foliation(foliation)
{
    setColumnCount(2);
    QFont font;
    font.setPixelSize(10);
    setFont(font);

    QStringList list;
    list << "Property" << "Value";
    setHeaderLabels(list);

    eulerCharItem = new QTreeWidgetItem(this);
    eulerCharItem->setText(0, tr("Euler characteristic"));

    genusItem = new QTreeWidgetItem(this);
    genusItem->setText(0, tr("Genus"));

    singListAbDiffItem = new QTreeWidgetItem(this);
    singListAbDiffItem->setText(0, tr("Singularity type (Abelian differential)"));

    singListProngsItem = new QTreeWidgetItem(this);
    singListProngsItem->setText(0, tr("Singularity type (number of prongs)"));

    lengthsItem = new QTreeWidgetItem(this);
    lengthsItem->setText(0, tr("Interval lengths"));
    lengthsItem->setText(1, tr(""));

    permutationItem = new QTreeWidgetItem(this);
    permutationItem->setText(0, tr("Permutation"));

    twistItem = new QTreeWidgetItem(this);
    twistItem->setText(0, tr("Twist"));

    topDivPointsItem = new QTreeWidgetItem(this);
    topDivPointsItem->setText(0, tr("Top division points"));
    topDivPointsItem->setText(1, tr(""));

    bottomDivPointsItem = new QTreeWidgetItem(this);
    bottomDivPointsItem->setText(0, tr("Bottom division points"));
    bottomDivPointsItem->setText(1, tr(""));

    setMinimumWidth(300);
    update();
}


void FoliationDataTreeWidget::update()
{
    eulerCharItem->setText(1, tr("%1").arg(m_foliation.eulerChar()));
    genusItem->setText(1, tr("%1").arg(m_foliation.genus()));

    QString s;
    s << m_foliation.singularityTypeOfAbelianDiff();
    singListAbDiffItem->setText(1, tr("(%1)").arg(s));

    s.clear();
    s << m_foliation.singularityTypeProngs();
    singListProngsItem->setText(1, tr("(%1)").arg(s));

    permutationItem->setText(1, string(m_foliation.intExchange().permutationWithMinimalTwist()));

    twistItem->setText(1, QString::number(static_cast<double>(m_foliation.intExchange().twist())));

    QTreeWidgetItem* childItem;

    lengthsItem->takeChildren();
    for(auto &x : m_foliation.intExchange().lengths()){
        childItem = new QTreeWidgetItem(lengthsItem);
        childItem->setText(1, QString::number(static_cast<double>(x)));
    }

    topDivPointsItem->takeChildren();
    for(auto &x : m_foliation.topDivPoints()){
        childItem = new QTreeWidgetItem(topDivPointsItem);
        childItem->setText(1, QString::number(static_cast<double>(x)));
    }

    bottomDivPointsItem->takeChildren();
    for(auto &x : m_foliation.bottomDivPoints()){
        childItem = new QTreeWidgetItem(bottomDivPointsItem);
        childItem->setText(1, QString::number(static_cast<double>(x)));
    }


    resizeColumnToContents(0);
}
