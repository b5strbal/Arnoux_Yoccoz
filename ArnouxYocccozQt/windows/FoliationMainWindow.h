#ifndef FOLIATIONMAINWINDOW_H
#define FOLIATIONMAINWINDOW_H

#include <QMainWindow>
#include <vector>

class QDockWidget;
class QListWidget;
class FileMenu;
class HelpMenu;
class WindowManager;
class FoliationManager;

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
    void setActiveFoliation(int index);

  //  void setPermFontSize(int fontSize);
    void rotateFoliationLeft();
    void rotateFoliationRight();
    void reverseFoliation();
    void flipOverFoliation();


private:
    const balazs::Foliation& activeFoliation() const;
    FoliationManager& activeFolationManager() const;

    void createMenus(const WindowManager& manager);
    void createActions();
  //  void updateConnections();
    void addDockWidgets();
    void removeDockWidgets();

    int activeFoliationIndex;

 //   std::vector<std::unique_ptr<FoliationManager>> foliations;

    QList<FoliationManager*> foliations;

    QDockWidget* foliationListDockWidget;
    QDockWidget* foliationDataTableDockWidget;
    QDockWidget* sepSegmentSearchDockWidget;

    QListWidget* foliationListWidget;

    QAction* findSepSegmentsAct;

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
