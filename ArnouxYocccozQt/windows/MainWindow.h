#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CreateFoliationWizard;
class DocumentationWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void newFoliation();
    void openDocumentation();

private:
    void createMenus();
    void createActions();
    
    QMenu* fileMenu;
    QMenu* helpMenu;
    
    QAction* newFoliationAct;
    QAction* openDocumentationAct;
    
    QTabWidget* openFoliationsTabWidget;

    CreateFoliationWizard* createFoliationWizard = nullptr;
    DocumentationWindow* documentationWindow = nullptr;
};

#endif // MAINWINDOW_H
