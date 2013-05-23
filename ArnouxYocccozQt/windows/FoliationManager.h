#ifndef FOLIATIONMANAGER_H
#define FOLIATIONMANAGER_H

#include <QObject>

class FoliationDataTreeWidget;
class SepSegmentSearchWidget;
class TransverseCurveSearchWidget;
class FoliationDrawingArea;
class QDockWidget;
class QMenu;
class QAction;


namespace balazs{
    class Foliation;
}


class FoliationManager : public QObject
{
    Q_OBJECT
public:
    explicit FoliationManager(std::unique_ptr<balazs::Foliation> foliation, QObject *parent = 0);
    virtual ~FoliationManager();
    const balazs::Foliation& foliation() const;
    SepSegmentSearchWidget* sepSegmentSearchWidget() const;
    TransverseCurveSearchWidget* transverseCurveSearchWidget() const;
    FoliationDataTreeWidget* foliationDataTableWidget() const;
    FoliationDrawingArea* drawingArea() const;
    QMenu* viewMenu() const;


signals:
    
public slots:

private:
    void createViewMenu();

    QMenu* m_viewMenu;
    QAction* permutationLabelsAct;
    QAction* lengthsLabelsAct;
    QAction* coloredfillingAct;
    QMenu* transverseCurveMenu;
    QAction* touchingSepSegmentsAct;

    std::unique_ptr<balazs::Foliation> pFoliation;

    FoliationDrawingArea* pDrawingArea;
    FoliationDataTreeWidget* pFoliationDataTableWidget;
    SepSegmentSearchWidget* pSepSegmentSearchWidget;
    TransverseCurveSearchWidget* pTransverseCurveSearchWidget;
};

#endif // FOLIATIONMANAGER_H
