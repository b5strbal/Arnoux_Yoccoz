#include "TransverseCurveSearchWidget.h"
#include "../../scc/TransverseCurve.h"
#include "../../fol/FoliationFromRP2.h"

#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTreeWidget>

TransverseCurveSearchWidget::TransverseCurveSearchWidget(balazs::SepSegmentDatabase &ssDatabase, QWidget *parent) :
    QWidget(parent),
    tcDatabase(ssDatabase)
{
    searchButton = new QPushButton(tr("Search"));

    modeLabel = new QLabel(tr("Mode:"));
    modeComboBox = new QComboBox;
    modeComboBox->addItem(tr("Shift leaf segments to left"));
    modeComboBox->addItem(tr("Shift leaf segments to right"));
    modeComboBox->addItem(tr("Wrap leaf segments onto singularities"));

    if(balazs::comesFromRP2(tcDatabase.foliation())){
        modeComboBox->addItem(tr("Shift leaf segments to left (lifts of curves from RP2)"));
        modeComboBox->addItem(tr("Shift leaf segments to right (lifts of curves from RP2)"));
    }

    maxInvoledSingularitiesLabel = new QLabel(tr("Max number of involved singularities:"));
    maxInvoledSingularitiesSpinBox = new QSpinBox;
    maxInvoledSingularitiesSpinBox->setRange(1, tcDatabase.foliation().numIntervals());

    maxDepthLabel = new QLabel(tr("Max depth:"));
    maxDepthSpinBox = new QSpinBox;
    maxDepthSpinBox->setMinimum(1);

    QHBoxLayout* lineLayout = new QHBoxLayout;
    lineLayout->addWidget(modeLabel);
    lineLayout->addWidget(modeComboBox);
    lineLayout->addWidget(maxInvoledSingularitiesLabel);
    lineLayout->addWidget(maxInvoledSingularitiesSpinBox);
    lineLayout->addWidget(maxDepthLabel);
    lineLayout->addWidget(maxDepthSpinBox);
    lineLayout->addWidget(searchButton);

    tcTreeWidget = new QTreeWidget;



    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(lineLayout);
    mainLayout->addWidget(tcTreeWidget);

    setLayout(mainLayout);
}
