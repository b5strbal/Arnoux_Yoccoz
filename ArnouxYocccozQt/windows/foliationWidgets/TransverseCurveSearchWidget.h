#ifndef TRANSVERSECURVESEARCHWIDGET_H
#define TRANSVERSECURVESEARCHWIDGET_H

#include <QWidget>
#include <QMap>
#include "../../scc/TransverseCurveDatabase.h"
#include <mutex>
#include <QMessageBox>

class QTreeWidget;
class QTreeWidgetItem;
class QLabel;
class QComboBox;
class QSpinBox;
class QPushButton;

namespace balazs{
    class TransverseCurveData;
    class SepSegmentDatabase;
}

class TransverseCurveSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TransverseCurveSearchWidget(balazs::SepSegmentDatabase& ssDatabase, QWidget *parent = 0);
signals:
    void drawTransverseCurve(const balazs::TransverseCurveData*);

public slots:
    
private slots:
    void startSearch();
    void stopSearching();
    void finishedSearching();
    void setEstimatedTimeLabel(int nothing = 0);
    void onCurrentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem *);

private:
    void createMaps();
    double estimatedTime();
    void displayResults();

    bool stopSearch;
    std::mutex mutex;

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
    QLabel* estimatedTimeLabel;
    QPushButton* searchButton;
    QPushButton* stopButton;

    QTreeWidget* tcTreeWidget;
//    QTreeWidgetItem* shiftToLeftItem;
//    QTreeWidgetItem* shiftToRightItem;
//    QTreeWidgetItem* wrapOnSingularitiesItem;
//    QTreeWidgetItem* shiftToLeftRP2Item;
//    QTreeWidgetItem* shiftToRightRP2Item;
};





class TransverseCurveSearch : public QObject{
    Q_OBJECT
public:
    TransverseCurveSearch(balazs::TransverseCurveDatabase& tcDatabase,
                          int maxdepth,
                          int maxInvolvedSingularities,
                          const bool& stopSearch,
                          std::mutex& mutex,
                          QObject* parent = 0)
        : QObject(parent),
          m_tcDatabase(tcDatabase),
          m_maxdepth(maxdepth),
          m_maxInvolvedSingularities(maxInvolvedSingularities),
          m_stopSearch(stopSearch),
          m_mutex(mutex)
    {}

public slots:
    void start(){
        m_tcDatabase.generateTransverseCurves(m_maxdepth, m_maxInvolvedSingularities, m_stopSearch, m_mutex);

        emit(finished());
    }


signals:
    void finished();

private:
    balazs::TransverseCurveDatabase& m_tcDatabase;
    int m_maxdepth;
    int m_maxInvolvedSingularities;

    const bool& m_stopSearch;
    std::mutex& m_mutex;
};

#endif // TRANSVERSECURVESEARCHWIDGET_H
