#include "SepSegmentSearchWidget.h"
#include "../../fol/Foliation.h"

#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>

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
    resultTable->setRowCount(foliation.numIntervals() * 4);
    QStringList headers;
    for(int i = 0; i < resultTable->rowCount(); i++){
        QString lr = i % 2 == 0 ? tr("Left") : tr("Right");
        QString ud = i % 4 < 2 ? tr("Up") : tr("Down");
        QString index = QString::number(i / 4);
        headers << index + "/" + ud + "/" + lr;
    }
    resultTable->setVerticalHeaderLabels(headers);
    resultTable->horizontalHeader()->setDefaultSectionSize(30);
    resultTable->verticalHeader()->setDefaultSectionSize(20);


    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(lineLayout);
    mainLayout->addWidget(resultTable);

    setLayout(mainLayout);

    connect(searchButton, SIGNAL(clicked()), this, SLOT(fillOutTable()));
    connect(resultTable, SIGNAL(cellPressed(int,int)), this, SLOT(drawSepSegment(int,int)));
}



int rowIndex(const balazs::SepSegmentIndex& ssIndex)
{
    int retval = 0;
    retval += 4 * ssIndex.singularityIntex;
    retval += ssIndex.vDirection == balazs::VDirection::Up ? 0 : 2;
    retval += ssIndex.hDirection == balazs::HDirection::Left ? 0 : 1;
    return retval;
}










void SepSegmentSearchWidget::fillOutTable()
{
    std::size_t maxDepth = depthSpinBox->value();
    sepSegmentDatabase.generateSepSegments(maxDepth);

    std::size_t maxRowLengths = 0;
    for(std::size_t index = 0; index < sepSegmentDatabase.foliation().numIntervals(); index++){
        for(balazs::VDirection vDirection : {balazs::VDirection::Up, balazs::VDirection::Down}){
            for(balazs::HDirection hDirection : {balazs::HDirection::Left, balazs::HDirection::Right}){
                balazs::SepSegmentIndex ssIndex = { hDirection, vDirection, index };
                const std::list<balazs::SeparatrixSegment>& list = sepSegmentDatabase.goodSegmentList(ssIndex);

                std::size_t countRow = 0;
                for(auto it = list.begin(); it != list.end() && it->depth() <= maxDepth; it++){
                    countRow++;
                }
                maxRowLengths = std::max(maxRowLengths, countRow);
            }
        }
    }
    resultTable->clearContents();
    resultTable->setColumnCount(maxRowLengths);

    for(std::size_t index = 0; index < sepSegmentDatabase.foliation().numIntervals(); index++){
        for(balazs::VDirection vDirection : {balazs::VDirection::Up, balazs::VDirection::Down}){
            for(balazs::HDirection hDirection : {balazs::HDirection::Left, balazs::HDirection::Right}){
                balazs::SepSegmentIndex ssIndex = { hDirection, vDirection, index };

                const std::list<balazs::SeparatrixSegment>& list = sepSegmentDatabase.goodSegmentList(ssIndex);

                int rowIndex = ::rowIndex(ssIndex);
                int columnIndex = 0;
                for(auto &segment : list){
                    QTableWidgetItem* item = new QTableWidgetItem(QString::number(segment.depth()));
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

                    resultTable->setItem(rowIndex, columnIndex++, item);
                }
            }
        }
    }
}


balazs::SepSegmentIndex ssIndex(int rowIndex)
{
    balazs::SepSegmentIndex retval;
    retval.singularityIntex = rowIndex / 4;
    retval.vDirection = rowIndex % 4 < 2 ? balazs::VDirection::Up : balazs::VDirection::Down;
    retval.hDirection = rowIndex % 2 == 0 ? balazs::HDirection::Left : balazs::HDirection::Right;
    return retval;
}


void SepSegmentSearchWidget::drawSepSegment(int row, int column)
{
    QTableWidgetItem* item = resultTable->item(row, column);
    if(!item) emit(drawSepSegment(nullptr));
    std::size_t depth = item->text().toULong();
    balazs::SepSegmentIndex ssIndex = ::ssIndex(row);

    auto it = sepSegmentDatabase.goodSegmentList(ssIndex).begin();
    while(it->depth() != depth){
        it++;
    }
    emit(drawSepSegment(&(*it)));
}
