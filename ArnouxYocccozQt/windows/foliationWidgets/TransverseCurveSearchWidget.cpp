#include "TransverseCurveSearchWidget.h"
#include "../../scc/TransverseCurve.h"
#include "../../scc/TransverseCurveDatabase.h"
#include "../../fol/FoliationFromRP2.h"
#include "../../scc/SSCMode.h"
#include "../../scc/SepSegmentDatabase.h"

#include <cassert>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTreeWidget>

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
    list << tr("Mode");
    tcTreeWidget->setHeaderLabels(list);

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

    searchButton = new QPushButton(tr("Search"));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(onSearchButtonClicked()));


    QHBoxLayout* lineLayout = new QHBoxLayout;
    lineLayout->addWidget(modeLabel);
    lineLayout->addWidget(modeComboBox);
    lineLayout->addWidget(maxInvoledSingularitiesLabel);
    lineLayout->addWidget(maxInvoledSingularitiesSpinBox);
    lineLayout->addWidget(maxDepthLabel);
    lineLayout->addWidget(maxDepthSpinBox);
    lineLayout->addWidget(searchButton);
    lineLayout->addStretch(1);





    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(lineLayout);
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
}





void TransverseCurveSearchWidget::onSearchButtonClicked()
{
    const QString &s = modeComboBox->currentText();
    tcDatabaseMap.at(s)->generateTransverseCurves(maxDepthSpinBox->value(), maxInvoledSingularitiesSpinBox->value());


}


