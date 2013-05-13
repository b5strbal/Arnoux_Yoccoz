#include "FoliationWindow.h"
#include "../fol/Foliation.h"
#include "../drawing/FoliationDrawingArea.h"
#include "../qt.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QHeaderView>
#include <QPushButton>

FoliationWindow::FoliationWindow(balazs::Foliation *foliation, QWidget *parent) :
    QWidget(parent),
    pFoliation(foliation)
{
    drawingArea = new FoliationDrawingArea(*pFoliation);

    rotateLeftButton = new QPushButton(tr("Rotate left"));
    rotateRightButton = new QPushButton(tr("Rotate right"));
    reverseButton = new QPushButton(tr("Reverse"));
    flipOverButton = new QPushButton(tr("Flip over"));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(rotateLeftButton);
    buttonLayout->addWidget(rotateRightButton);
    buttonLayout->addWidget(reverseButton);
    buttonLayout->addWidget(flipOverButton);

    connect(rotateLeftButton, SIGNAL(clicked()), this, SLOT(rotateFoliationLeft()));
    connect(rotateRightButton, SIGNAL(clicked()), this, SLOT(rotateFoliationRight()));
    connect(reverseButton, SIGNAL(clicked()), this, SLOT(reverseFoliation()));
    connect(flipOverButton, SIGNAL(clicked()), this, SLOT(flopOverFoliation()));


    QVBoxLayout* middleLayout = new QVBoxLayout;
    middleLayout->addWidget(drawingArea);
    middleLayout->addLayout(buttonLayout);


    permutationLabelsCheckBox = new QCheckBox(tr("Show permutation"));
    lengthsLabelsCheckBox = new QCheckBox(tr("Show lengths"));
    fillingCheckBox = new QCheckBox(tr("Use colored filling"));

    connect(permutationLabelsCheckBox, SIGNAL(stateChanged(int)), drawingArea, SLOT(setPermutationCheckBoxState(int)));
    connect(lengthsLabelsCheckBox, SIGNAL(stateChanged(int)), drawingArea, SLOT(setLengthsCheckBoxState(int)));
    connect(fillingCheckBox, SIGNAL(stateChanged(int)), drawingArea, SLOT(setFillingCheckBoxState(int)));

    permutationLabelsCheckBox->setChecked(true);
    lengthsLabelsCheckBox->setChecked(true);
    fillingCheckBox->setChecked(true);


    permFontSizeLabel = new QLabel(tr("Font size:"));
    permFontSizeSpinBox = new QSpinBox;
    connect(permFontSizeSpinBox, SIGNAL(valueChanged(int)), drawingArea, SLOT(setPermFontSize(int)));
    permFontSizeSpinBox->setValue(14);

    lengthsFontSizeLabel = new QLabel(tr("Font size:"));
    lengthsFontSizeSpinBox = new QSpinBox;
    connect(lengthsFontSizeSpinBox, SIGNAL(valueChanged(int)), drawingArea, SLOT(setLengthsFontSize(int)));
    lengthsFontSizeSpinBox->setValue(10);

    QHBoxLayout* permFontSizeLayout = new QHBoxLayout;
    permFontSizeLayout->addWidget(permFontSizeLabel);
    permFontSizeLayout->addWidget(permFontSizeSpinBox);

    QHBoxLayout* lengthsFontSizeLayout = new QHBoxLayout;
    lengthsFontSizeLayout->addWidget(lengthsFontSizeLabel);
    lengthsFontSizeLayout->addWidget(lengthsFontSizeSpinBox);

    QVBoxLayout* optionsLayout = new QVBoxLayout;
    optionsLayout->addWidget(permutationLabelsCheckBox);
    optionsLayout->addLayout(permFontSizeLayout);
    optionsLayout->addWidget(lengthsLabelsCheckBox);
    optionsLayout->addLayout(lengthsFontSizeLayout);
    optionsLayout->addWidget(fillingCheckBox);

    drawingOptionsGroupBox = new QGroupBox(tr("Drawing Options"));
    drawingOptionsGroupBox->setLayout(optionsLayout);

    m_foliationDataTable = new FoliationDataTreeWidget(*pFoliation);

    QVBoxLayout* rightLayout = new QVBoxLayout;
    rightLayout->addWidget(drawingOptionsGroupBox);
    rightLayout->addWidget(m_foliationDataTable);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addLayout(middleLayout, 1);
    mainLayout->addLayout(rightLayout);


    setLayout(mainLayout);
}

FoliationWindow::~FoliationWindow()
{
    delete pFoliation;
}


void FoliationWindow::rotateFoliationLeft()
{
    *pFoliation = pFoliation->rotateBy(-1);
    drawingArea->update();
    m_foliationDataTable->update();
}

void FoliationWindow::rotateFoliationRight()
{
    *pFoliation = pFoliation->rotateBy(1);
    drawingArea->update();
    m_foliationDataTable->update();
}

void FoliationWindow::reverseFoliation()
{
    *pFoliation = pFoliation->reflect();
    drawingArea->update();
    m_foliationDataTable->update();
}

void FoliationWindow::flopOverFoliation()
{
    *pFoliation = pFoliation->flipOver();
    drawingArea->update();
    m_foliationDataTable->update();
}






FoliationDataTreeWidget::FoliationDataTreeWidget(const balazs::Foliation &foliation, QWidget *parent) :
    QTreeWidget(parent),
    m_foliation(foliation)
{
    setColumnCount(2);
    QFont font;
    font.setPixelSize(10);
    setFont(font);

    QStringList list;
    list << "Property" << "Value";
    setHeaderLabels(list);

    eulerCharItem = new QTreeWidgetItem(this);
    eulerCharItem->setText(0, tr("Euler characteristic"));

    genusItem = new QTreeWidgetItem(this);
    genusItem->setText(0, tr("Genus"));

    singListAbDiffItem = new QTreeWidgetItem(this);
    singListAbDiffItem->setText(0, tr("Singularity type (Abelian differential)"));

    singListProngsItem = new QTreeWidgetItem(this);
    singListProngsItem->setText(0, tr("Singularity type (number of prongs)"));

    lengthsItem = new QTreeWidgetItem(this);
    lengthsItem->setText(0, tr("Interval lengths"));
    lengthsItem->setText(1, tr(""));

    topDivPointsItem = new QTreeWidgetItem(this);
    topDivPointsItem->setText(0, tr("Top division points"));
    topDivPointsItem->setText(1, tr(""));

    bottomDivPointsItem = new QTreeWidgetItem(this);
    bottomDivPointsItem->setText(0, tr("Bottom division points"));
    bottomDivPointsItem->setText(1, tr(""));

    update();
}

void FoliationDataTreeWidget::update()
{
    eulerCharItem->setText(1, tr("%1").arg(m_foliation.eulerChar()));
    genusItem->setText(1, tr("%1").arg(m_foliation.genus()));

    QString s;
    s << m_foliation.singularityTypeOfAbelianDiff();
    singListAbDiffItem->setText(1, tr("(%1)").arg(s));

    s.clear();
    s << m_foliation.singularityTypeProngs();
    singListProngsItem->setText(1, tr("(%1)").arg(s));

    QTreeWidgetItem* childItem;

    lengthsItem->takeChildren();
    for(auto &x : m_foliation.intExchange().lengths()){
        childItem = new QTreeWidgetItem(lengthsItem);
        childItem->setText(1, QString::number(static_cast<double>(x)));
    }

    topDivPointsItem->takeChildren();
    for(auto &x : m_foliation.topDivPoints()){
        childItem = new QTreeWidgetItem(topDivPointsItem);
        childItem->setText(1, QString::number(static_cast<double>(x)));
    }

    bottomDivPointsItem->takeChildren();
    for(auto &x : m_foliation.bottomDivPoints()){
        childItem = new QTreeWidgetItem(bottomDivPointsItem);
        childItem->setText(1, QString::number(static_cast<double>(x)));
    }


    resizeColumnToContents(0);
}


