#ifndef TRANSVERSECURVESEARCHWIDGET_H
#define TRANSVERSECURVESEARCHWIDGET_H

#include <QWidget>
#include "../../scc/TransverseCurveDatabase.h"

class QTreeWidget;
class QLabel;
class QComboBox;
class QSpinBox;
class QPushButton;

namespace balazs{
    class TransverseCurve;
    class SepSegmentDatabase;
}

class TransverseCurveSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TransverseCurveSearchWidget(balazs::SepSegmentDatabase& ssDatabase, QWidget *parent = 0);
    
signals:
    void drawTransverseCurves(const balazs::TransverseCurve*);

public slots:
    

private:
    balazs::TransverseCurveDatabase tcDatabase;

    QLabel* modeLabel;
    QComboBox* modeComboBox;
    QLabel* maxInvoledSingularitiesLabel;
    QSpinBox* maxInvoledSingularitiesSpinBox;
    QLabel* maxDepthLabel;
    QSpinBox* maxDepthSpinBox;
    QPushButton* searchButton;

    QTreeWidget* tcTreeWidget;
};

#endif // TRANSVERSECURVESEARCHWIDGET_H
