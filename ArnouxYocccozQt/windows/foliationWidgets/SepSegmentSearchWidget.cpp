#include "SepSegmentSearchWidget.h"
#include "../../fol/Foliation.h"

#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>

SepSegmentSearchWidget::SepSegmentSearchWidget(const balazs::Foliation& foliation, QWidget *parent) :
    QWidget(parent),
    sepSegmentDatabase(foliation)
{
    depthLabel = new QLabel(tr("Depth:"));
    depthSpinBox = new QSpinBox;
    depthSpinBox->setRange(1, INT_MAX);
    depthSpinBox->setSingleStep(10);
    searchButton = new QPushButton(tr("Search"));

    QHBoxLayout* lineLayout = new QHBoxLayout;
    lineLayout->addWidget(depthLabel);
    lineLayout->addWidget(depthSpinBox);
    lineLayout->addWidget(searchButton);
    lineLayout->addStretch(1);

    resultTable = new QTableWidget;
    resultTable->setColumnCount(foliation.numIntervals() * 4);
    QStringList headers;
    for(int i = 0; i < resultTable->columnCount(); i++){
        QString lr = i % 2 == 0 ? tr("Left") : tr("Right");
        QString up = i % 4 < 2 ? tr("Up") : tr("Down");
        QString index = QString::number(i / 4);
        headers << index + "/" + up + "/" + lr;
    }
    resultTable->setHorizontalHeaderLabels(headers);


    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(lineLayout);
    mainLayout->addWidget(resultTable);

    setLayout(mainLayout);

    connect(searchButton, SIGNAL(clicked()), this, SLOT(fillOutTable()));
}


void SepSegmentSearchWidget::fillOutTable()
{
    sepSegmentDatabase.generateSepSegments(depthSpinBox->value());

    for(std::size_t index = 0; index < sepSegmentDatabase.foliation().numIntervals(); index++){
        for(balazs::Direction::UpOrDown upOrDown = balazs::Direction::UP;
            upOrDown <= balazs::Direction::DOWN; upOrDown++){

            for(balazs::Direction::LeftOrRight leftOrRight = balazs::Direction::LEFT;
                leftOrRight <= balazs::Direction::RIGHT; leftOrRight++){

                balazs::SepSegmentIndex ssIndex = { leftOrRight, upOrDown, index };

                const std::list<balazs::SeparatrixSegment>& list = sepSegmentDatabase.goodSegmentList(ssIndex);

                int count = 1;
                for(auto &segment : list){
                    QTableWidgetItem* item = new QTableWidgetItem(QString::number(segment.depth()));
                    if(resultTable->rowCount() < count){
                        resultTable->setRowCount(resultTable->rowCount() + 1);
                    }

                    resultTable->setItem(4 * index + 2 * upOrDown + leftOrRight, count, item);
                }
            }
        }
    }
}
