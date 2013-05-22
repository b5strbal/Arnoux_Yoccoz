#include "TransverseCurveSearchWidget.h"
#include "../../scc/TransverseCurve.h"
#include "../../scc/TransverseCurveDatabase.h"
#include "../../fol/FoliationFromRP2.h"
#include "../../scc/SSCMode.h"
#include "../../scc/SepSegmentDatabase.h"
#include "TransverseCurveTreeWidgetItem.h"

#include <cassert>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QMessageBox>
#include <QThread>
#include <cmath>

QString TransverseCurveSearchWidget::shiftToLeftSideString = tr("Shift leaf segments to left");
QString TransverseCurveSearchWidget::shiftToRightSideString = tr("Shift leaf segments to right");
QString TransverseCurveSearchWidget::wrapOnSingularitiesString = tr("Wrap leaf segments onto singularities");
QString TransverseCurveSearchWidget::shiftToLeftSideRP2String = tr("Shift leaf segments to left (lifts of curves from RP2)");
QString TransverseCurveSearchWidget::shiftToRightSideRP2String = tr("Shift leaf segments to right (lifts of curves from RP2)");




TransverseCurveSearchWidget::TransverseCurveSearchWidget(balazs::SepSegmentDatabase &ssDatabase, QWidget *parent) :
    QWidget(parent),
    sepSegmentDatabase(ssDatabase)
{
    tcTreeWidget = new QTreeWidget;
    tcTreeWidget->setColumnCount(4);
    QStringList list;
    list << tr("Mode") << tr("Length");
    tcTreeWidget->setHeaderLabels(list);

    connect(tcTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

    createMaps();

    modeLabel = new QLabel(tr("Mode:"));
    modeComboBox = new QComboBox;

    foreach(const QString& s, relevantStrings){
        modeComboBox->addItem(s);
    }

    maxInvoledSingularitiesLabel = new QLabel(tr("Max number of involved singularities:"));
    maxInvoledSingularitiesSpinBox = new QSpinBox;
    maxInvoledSingularitiesSpinBox->setRange(1, sepSegmentDatabase.foliation().numIntervals());

    maxDepthLabel = new QLabel(tr("Max depth:"));
    maxDepthSpinBox = new QSpinBox;
    maxDepthSpinBox->setRange(1, INT_MAX);

    estimatedTimeLabel = new QLabel;
    estimatedTimeLabel->setStyleSheet("QLabel { color : red; }");

    connect(modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEstimatedTimeLabel(int)));
    connect(maxInvoledSingularitiesSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setEstimatedTimeLabel(int)));
    connect(maxDepthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setEstimatedTimeLabel(int)));
    setEstimatedTimeLabel();

    searchButton = new QPushButton(tr("Search"));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(startSearch()));

    stopButton = new QPushButton(tr("Stop"));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stopSearching()));


    QHBoxLayout* lineLayout = new QHBoxLayout;
    lineLayout->addWidget(modeLabel);
    lineLayout->addWidget(modeComboBox);
    lineLayout->addWidget(maxInvoledSingularitiesLabel);
    lineLayout->addWidget(maxInvoledSingularitiesSpinBox);
    lineLayout->addWidget(maxDepthLabel);
    lineLayout->addWidget(maxDepthSpinBox);
    lineLayout->addStretch(1);


    QHBoxLayout* lineLayout2 = new QHBoxLayout;
    lineLayout2->addStretch(1);
    lineLayout2->addWidget(estimatedTimeLabel);
    lineLayout2->addWidget(searchButton);
    lineLayout2->addWidget(stopButton);
    lineLayout2->addStretch(1);



    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(lineLayout);
    mainLayout->addLayout(lineLayout2);
    mainLayout->addWidget(tcTreeWidget);

    setLayout(mainLayout);
}



void TransverseCurveSearchWidget::createMaps()
{
    relevantStrings << shiftToLeftSideString << shiftToRightSideString << wrapOnSingularitiesString;
    if(balazs::comesFromRP2(sepSegmentDatabase.foliation())){
        relevantStrings << shiftToLeftSideRP2String << shiftToRightSideRP2String;
    }

    foreach(const QString& s, relevantStrings){
        topWidgetItemMap[s] = new QTreeWidgetItem(tcTreeWidget);
        topWidgetItemMap.at(s)->setText(0, s);

        balazs::SSCMode* pSSCMode;
        if(s == shiftToLeftSideString){
            pSSCMode = new balazs::SSCModeShiftToSameSide(sepSegmentDatabase.foliation(), balazs::HDirection::Left);
        } else if(s == shiftToRightSideString){
            pSSCMode = new balazs::SSCModeShiftToSameSide(sepSegmentDatabase.foliation(), balazs::HDirection::Right);
        } else if(s == wrapOnSingularitiesString){
            pSSCMode = new balazs::SSCModeSingWrap(sepSegmentDatabase.foliation());
        } else if(s == shiftToLeftSideRP2String){
            pSSCMode = new balazs::SSCModeShiftToSameSideFromRP2(sepSegmentDatabase.foliation(), balazs::HDirection::Left);
        } else if(s == shiftToRightSideRP2String){
            pSSCMode = new balazs::SSCModeShiftToSameSideFromRP2(sepSegmentDatabase.foliation(), balazs::HDirection::Right);
        } else {
            assert(false);
        }

        balazs::TransverseCurveDatabase* p = new balazs::TransverseCurveDatabase(sepSegmentDatabase,
                std::shared_ptr<balazs::SSCMode>(pSSCMode));
        tcDatabaseMap[s] = std::unique_ptr<balazs::TransverseCurveDatabase>(p);
    }
    tcTreeWidget->resizeColumnToContents(0);
}



double TransverseCurveSearchWidget::estimatedTime()
{
    double retval = 0;
    const QString &s = modeComboBox->currentText();
    if(s == shiftToLeftSideString || s == shiftToRightSideString){
        for(int k = 1; k <= maxInvoledSingularitiesSpinBox->value(); k++){
            retval += balazs::choose(sepSegmentDatabase.foliation().numIntervals(), k) *
                    pow(maxDepthSpinBox->value(), 2 * k);
        }
    } else if(s == wrapOnSingularitiesString){
        for(int k = 1; k <= maxInvoledSingularitiesSpinBox->value(); k++){
            retval += balazs::choose(sepSegmentDatabase.foliation().numIntervals(), k) *
                    pow(maxDepthSpinBox->value(), k);
        }
    } else if(s == shiftToLeftSideRP2String || s == shiftToRightSideRP2String){
        for(int k = 1; k <= maxInvoledSingularitiesSpinBox->value()/2; k++){
            retval += balazs::choose(sepSegmentDatabase.foliation().numIntervals()/2, k) *
                    pow(maxDepthSpinBox->value(), 2 * k);
        }
    }
    return retval / 40000;
}



void TransverseCurveSearchWidget::displayResults()
{
    const QString &s = modeComboBox->currentText();
    foreach(QTreeWidgetItem* item, topWidgetItemMap[s]->takeChildren()){
        topWidgetItemMap[s]->removeChild(item);
        delete item;
    }

    for(const auto& tc : *tcDatabaseMap[s]){
        new TransverseCurveTreeWidgetItem(tc, topWidgetItemMap[s]);
    }
    topWidgetItemMap[s]->setExpanded(true);
}












void TransverseCurveSearchWidget::startSearch()
{
    QThread* thread = new QThread;
    const QString &s = modeComboBox->currentText();
    TransverseCurveSearch* tcSearch = new TransverseCurveSearch(*tcDatabaseMap.at(s),
                                                                maxDepthSpinBox->value(),
                                                                maxInvoledSingularitiesSpinBox->value(),
                                                                stopSearch, mutex);

    stopSearch = false;

    connect(thread, SIGNAL(started()), tcSearch, SLOT(start()));
    connect(tcSearch, SIGNAL(finished()), this, SLOT(finishedSearching()));
    connect(tcSearch, SIGNAL(finished()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), tcSearch, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    stopButton->setEnabled(true);
    maxDepthSpinBox->setEnabled(false);
    maxInvoledSingularitiesSpinBox->setEnabled(false);
    modeComboBox->setEnabled(false);
    searchButton->setEnabled(false);

    tcSearch->moveToThread(thread);

    thread->start();

}


void TransverseCurveSearchWidget::stopSearching()
{
    std::lock_guard<std::mutex> lk(mutex);
    stopSearch = true;
}

void TransverseCurveSearchWidget::finishedSearching()
{
    stopButton->setEnabled(false);
    maxDepthSpinBox->setEnabled(true);
    maxInvoledSingularitiesSpinBox->setEnabled(true);
    modeComboBox->setEnabled(true);
    searchButton->setEnabled(true);

    displayResults();
}


void TransverseCurveSearchWidget::setEstimatedTimeLabel(int nothing)
{
    (void)nothing;
    estimatedTimeLabel->setText(tr("Rough estimated time: %1 seconds").arg(static_cast<long>(estimatedTime())));
}

void TransverseCurveSearchWidget::onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem*)
{
    TransverseCurveTreeWidgetItem* item = dynamic_cast<TransverseCurveTreeWidgetItem*>(current);
    if(item){
        emit(drawTransverseCurve(&item->transverseCurve()));
    }
}


