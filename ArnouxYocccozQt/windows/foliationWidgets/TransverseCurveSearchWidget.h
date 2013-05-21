#ifndef TRANSVERSECURVESEARCHWIDGET_H
#define TRANSVERSECURVESEARCHWIDGET_H

#include <QWidget>
#include <QMap>
#include "../../scc/TransverseCurveDatabase.h"

class QTreeWidget;
class QTreeWidgetItem;
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
    
private slots:
    void onSearchButtonClicked();

private:
    void createMaps();

    balazs::SepSegmentDatabase& sepSegmentDatabase;

    static QString shiftToLeftSideString;
    static QString shiftToRightSideString;
    static QString wrapOnSingularitiesString;
    static QString shiftToLeftSideRP2String;
    static QString shiftToRightSideRP2String;

    QStringList relevantStrings;

    std::map<QString, std::unique_ptr<balazs::TransverseCurveDatabase>> tcDatabaseMap;
    std::map<QString, QTreeWidgetItem*> topWidgetItemMap;
   // QMap<QString, std::shared_ptr<balazs::SSCMode>> sscModeMap;

    QLabel* modeLabel;
    QComboBox* modeComboBox;
    QLabel* maxInvoledSingularitiesLabel;
    QSpinBox* maxInvoledSingularitiesSpinBox;
    QLabel* maxDepthLabel;
    QSpinBox* maxDepthSpinBox;
    QPushButton* searchButton;

    QTreeWidget* tcTreeWidget;
//    QTreeWidgetItem* shiftToLeftItem;
//    QTreeWidgetItem* shiftToRightItem;
//    QTreeWidgetItem* wrapOnSingularitiesItem;
//    QTreeWidgetItem* shiftToLeftRP2Item;
//    QTreeWidgetItem* shiftToRightRP2Item;
};

#endif // TRANSVERSECURVESEARCHWIDGET_H
