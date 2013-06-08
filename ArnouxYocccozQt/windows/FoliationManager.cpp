#include "FoliationManager.h"
#include "../fol/Foliation.h"
#include "drawing/FoliationDrawing.h"
#include "foliationWindow/SepSegmentSearchWidget.h"
#include "foliationWindow/FoliationDataTreeWidget.h"
#include "foliationWindow/TransverseCurveSearchWidget.h"

#include <QMenu>

FoliationManager::FoliationManager(std::unique_ptr<balazs::Foliation> foliation, QObject *parent) :
    QObject(parent),
    pFoliation(std::move(foliation))
{
    pDrawingArea = new FoliationDrawing(*pFoliation, QSize(500, 500));
    pFoliationDataTableWidget = new FoliationDataTreeWidget(*pFoliation);
    pSepSegmentSearchWidget = new SepSegmentSearchWidget(*pFoliation);
    pTransverseCurveSearchWidget = new TransverseCurveSearchWidget(pSepSegmentSearchWidget->sepSegmentDatabase());
    createViewMenu();

    connect(pSepSegmentSearchWidget, SIGNAL(drawSepSegment(const balazs::SeparatrixSegment*)),
            pDrawingArea, SLOT(drawSepSegment(const balazs::SeparatrixSegment*)));
    connect(pTransverseCurveSearchWidget, SIGNAL(drawTransverseCurve(const balazs::TransverseCurve*)),
            pDrawingArea, SLOT(drawTransverseCurve(const balazs::TransverseCurve*)));
}

FoliationManager::~FoliationManager() = default;


const balazs::Foliation& FoliationManager::foliation() const
{
    return *pFoliation;
}

SepSegmentSearchWidget *FoliationManager::sepSegmentSearchWidget() const
{
    return pSepSegmentSearchWidget;
}

TransverseCurveSearchWidget *FoliationManager::transverseCurveSearchWidget() const
{
    return pTransverseCurveSearchWidget;
}

FoliationDrawing *FoliationManager::drawingArea() const
{
    return pDrawingArea;
}

QMenu *FoliationManager::viewMenu() const
{
    return m_viewMenu;
}


FoliationDataTreeWidget* FoliationManager::foliationDataTableWidget() const
{
    return pFoliationDataTableWidget;
}


void FoliationManager::createViewMenu()
{
    permutationLabelsAct = new QAction(tr("Show permutation"), this);
    lengthsLabelsAct = new QAction(tr("Show lengths"), this);
    coloredfillingAct = new QAction(tr("Use colored filling"), this);

    permutationLabelsAct->setCheckable(true);
    lengthsLabelsAct->setCheckable(true);
    coloredfillingAct->setCheckable(true);

    connect(permutationLabelsAct, SIGNAL(toggled(bool)), pDrawingArea, SLOT(setPermutationLabels(bool)));
    connect(lengthsLabelsAct, SIGNAL(toggled(bool)), pDrawingArea, SLOT(setLengthsLabels(bool)));
    connect(coloredfillingAct, SIGNAL(toggled(bool)), pDrawingArea, SLOT(setColoredFilling(bool)));

    permutationLabelsAct->setChecked(true);
    lengthsLabelsAct->setChecked(true);
    coloredfillingAct->setChecked(true);

    touchingSepSegmentsAct = new QAction(tr("Show touching separatrix segments"), this);
    touchingSepSegmentsAct->setCheckable(true);
    connect(touchingSepSegmentsAct, SIGNAL(toggled(bool)), pDrawingArea, SLOT(setTouchingSepSegments(bool)));
    touchingSepSegmentsAct->setChecked(true);

    m_viewMenu = new QMenu(tr("View"));
    m_viewMenu->addAction(permutationLabelsAct);
    m_viewMenu->addAction(lengthsLabelsAct);
    m_viewMenu->addAction(coloredfillingAct);

    transverseCurveMenu = m_viewMenu->addMenu(tr("Transverse curves"));
    transverseCurveMenu->addAction(touchingSepSegmentsAct);
}






