#ifndef SEPSEGMENTSEARCHWIDGET_H
#define SEPSEGMENTSEARCHWIDGET_H

#include <QWidget>
#include "../../scc/SepSegmentDatabase.h"


class QLabel;
class QSpinBox;
class QPushButton;
class QTableWidget;
namespace balazs{
    class Foliation;
}

class SepSegmentSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SepSegmentSearchWidget(const balazs::Foliation& foliation, QWidget *parent = 0);
    
signals:
    void drawSepSegment(const balazs::SeparatrixSegment*);
public slots:

private slots:
    void fillOutTable();
    void drawSepSegment(int row, int column);

private:    


    QLabel* depthLabel;
    QSpinBox* depthSpinBox;
    QPushButton* searchButton;

    QTableWidget* resultTable;

    balazs::SepSegmentDatabase sepSegmentDatabase;
};

#endif // SEPSEGMENTSEARCHWIDGET_H
