#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CreateFoliationWizard;
class DocumentationWindow;
class QListWidget;
class QStackedWidget;

namespace balazs{
    class Foliation;
    class FoliationRP2;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void newFoliationWizard();
    void openDocumentation();
    void newFoliationWindow(balazs::Foliation *pFoliation);

private:
    void createMenus();
    void createActions();
    
    QListWidget* foliationListWidget;
    QStackedWidget* foliationPagesWidget;

    QMenu* fileMenu;
    QMenu* helpMenu;
    
    QAction* newFoliationAct;
    QAction* openDocumentationAct;
    
    CreateFoliationWizard* createFoliationWizard = nullptr;
    DocumentationWindow* documentationWindow = nullptr;
};

#endif // MAINWINDOW_H
