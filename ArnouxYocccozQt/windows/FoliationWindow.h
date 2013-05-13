#ifndef FOLIATIONWINDOW_H
#define FOLIATIONWINDOW_H

#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>

class FoliationDrawingArea;
class QGroupBox;
class QCheckBox;
class QLabel;
class QSpinBox;
class FoliationDataTreeWidget;

namespace balazs{
    class Foliation;
}


class FoliationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FoliationWindow(balazs::Foliation* foliation, QWidget *parent = 0);
    ~FoliationWindow();
signals:
    
private slots:
    void rotateFoliationLeft();
    void rotateFoliationRight();
    void reverseFoliation();
    void flopOverFoliation();

private:
    balazs::Foliation* pFoliation;

    FoliationDrawingArea* drawingArea;

    QGroupBox* drawingOptionsGroupBox;
    QCheckBox* permutationLabelsCheckBox;
    QCheckBox* lengthsLabelsCheckBox;
    QCheckBox* fillingCheckBox;
    QLabel* permFontSizeLabel;
    QSpinBox* permFontSizeSpinBox;
    QLabel* lengthsFontSizeLabel;
    QSpinBox* lengthsFontSizeSpinBox;

    FoliationDataTreeWidget* m_foliationDataTable;

    QPushButton* rotateRightButton;
    QPushButton* rotateLeftButton;
    QPushButton* reverseButton;
    QPushButton* flipOverButton;
};









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







#endif // FOLIATIONWINDOW_H
