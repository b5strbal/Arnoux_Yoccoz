#ifndef FOLIATIONMAINWINDOW_H
#define FOLIATIONMAINWINDOW_H

#include <QMainWindow>
#include <vector>

class QDockWidget;
class QStackedWidget;
class QListWidget;
class FileMenu;
class HelpMenu;
class WindowManager;
class FoliationManager;

class FoliationListWidget;

namespace balazs{
    class Foliation;
}

class FoliationMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit FoliationMainWindow(const WindowManager& manager, QWidget *parent = 0);
    
signals:
public slots:
    void createNewFoliation(balazs::Foliation* pFoliation);

private slots:
    void openSepSegmentSearch();
    void openTransverseCurveSearch();

    void rotateFoliationLeft();
    void rotateFoliationRight();
    void reverseFoliation();
    void flipOverFoliation();
    void updateMenus(int newActiveIndex);

private:
    const balazs::Foliation& activeFoliation() const;
    FoliationManager& activeFolationManager() const;
    int activeIndex() const;

    void createMenus(const WindowManager& manager);
    void createActions();


    QList<FoliationManager*> foliations;

    QDockWidget* foliationListDockWidget;
    QDockWidget* foliationDataTableDockWidget;
    QDockWidget* sepSegmentSearchDockWidget;
    QDockWidget* transverseCurveSearchDockWidget;

    FoliationListWidget* foliationListWidget;
    QStackedWidget* foliationDataTableStackedWidget;
    QStackedWidget* sepSegmentSearchStackedWidget;
    QStackedWidget* drawingAreaStackWidget;
    QStackedWidget* transverseCurveSearchStackedWidget;

    QAction* findSepSegmentsAct;
    QAction* findTransverseCurvesAct;

    QMenu* reparametrizeMenu;
    QAction* rotateLeftAct;
    QAction* rotateRightAct;
    QAction* reverseAct;
    QAction* flipOverAct;

    FileMenu* fileMenu;
    QMenu* searchMenu;
    QMenu* viewMenu;
    QMenu* toolsMenu;
    HelpMenu* helpMenu;
    
//    QToolBar* toolBar;

};






#endif // FOLIATIONMAINWINDOW_H
