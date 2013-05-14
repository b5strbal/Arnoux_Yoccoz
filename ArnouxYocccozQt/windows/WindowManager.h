#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

class CreateFoliationWizard;
class DocumentationWindow;
class FoliationMainWindow;



class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = 0);
    
signals:
    
public slots:
    void newFoliationWizard();
    void openDocumentation();
private:
    CreateFoliationWizard* createFoliationWizard;
    DocumentationWindow* documentationWindow;
    FoliationMainWindow* foliationMainWindow;
};

#endif // WINDOWMANAGER_H
