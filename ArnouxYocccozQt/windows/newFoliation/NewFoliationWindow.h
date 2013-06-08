#ifndef NEWFOLIATIONWINDOW_H
#define NEWFOLIATIONWINDOW_H

#include <QWidget>
#include <map>

class QListWidget;
class QListWidgetItem;
class QLabel;
class QStackedWidget;
class QPushButton;

namespace balazs{
    class Foliation;
    class FoliationRP2;
    class FoliationSphere;
}


class NewFoliationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit NewFoliationWindow(QWidget *parent = 0);
signals:
    void foliation(balazs::Foliation* newFoliation);
    void foliationRP2(balazs::FoliationRP2* newFoliationRP2);
    void foliationSphere(balazs::FoliationSphere* newFoliationSphere);

private slots:
    void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void foliationSignalEmitted(balazs::Foliation* newFoliation);
    void foliationRP2SignalEmitted(balazs::FoliationRP2* newFoliationRP2);
    void foliationSphereSignalEmitted(balazs::FoliationSphere* newFoliationSphere);

private:
    void createStackedWidget();

    QLabel* surfaceLabel;
    QListWidget* listWidget;

    QStackedWidget* detailsStackedWidget;

    std::map<QListWidgetItem*, QWidget*> map;
};

#endif // NEWFOLIATIONWINDOW_H
