#include "NewFoliationWindow.h"
#include "ParameterWidgets.h"

#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QListWidgetItem>

NewFoliationWindow::NewFoliationWindow(QWidget *parent) :
    QWidget(parent)
{
    surfaceLabel = new QLabel(tr("Choose way of defining a new foliation"));

    listWidget = new QListWidget;

    QListWidgetItem* newItem;

    QFont boldFont;
    boldFont.setBold(true);

    newItem = new QListWidgetItem(tr("Orientable closed surface with positive genus"), listWidget);
    newItem->setFont(boldFont);
    newItem->setFlags(Qt::ItemIsEnabled);
    newItem->setBackgroundColor(palette().color(QPalette::Window));
  //  newItem->setForeground(QBrush(Qt::white));


    {
        newItem = new QListWidgetItem(tr("Specify a twisted interval exchange map"), listWidget);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        NewFoliationIntExchangeWidget* widget = new NewFoliationIntExchangeWidget;
        map[newItem] = widget;
        connect(widget, SIGNAL(foliation(balazs::Foliation*)), this, SLOT(foliationSignalEmitted(balazs::Foliation*)));
    }

    {
        newItem = new QListWidgetItem(tr("Random twisted interval exchange map"), listWidget);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        NewFoliationRandomWidget* widget = new NewFoliationRandomWidget;
        map[newItem] = widget;
        connect(widget, SIGNAL(foliation(balazs::Foliation*)), this, SLOT(foliationSignalEmitted(balazs::Foliation*)));
    }

    {
        newItem = new QListWidgetItem(tr("An Arnoux-Yoccox foliation"), listWidget);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        NewFoliationAYWidget* widget = new NewFoliationAYWidget;
        map[newItem] = widget;
        connect(widget, SIGNAL(foliation(balazs::Foliation*)), this, SLOT(foliationSignalEmitted(balazs::Foliation*)));
    }




    newItem = new QListWidgetItem(tr("Sphere"), listWidget);
    newItem->setFont(boldFont);
    newItem->setFlags(Qt::ItemIsEnabled);
    newItem->setBackgroundColor(palette().color(QPalette::Window));
   // newItem->setForeground(QBrush(Qt::white));

    {
        newItem = new QListWidgetItem(tr("Specify the foliation on the upper and lower hemisphere"), listWidget);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        NewFoliationSphereDiskWidget* widget = new NewFoliationSphereDiskWidget;
        map[newItem] = widget;
        connect(widget, SIGNAL(foliationSphere(balazs::FoliationSphere*)),
                this, SLOT(foliationSphereSignalEmitted(balazs::FoliationSphere*)));
    }
    {
        newItem = new QListWidgetItem(tr("The Hubert-Lanneau-Moller example"), listWidget);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        NewFoliationSphereHLMWidget* widget = new NewFoliationSphereHLMWidget;
        map[newItem] = widget;
        connect(widget, SIGNAL(foliationSphere(balazs::FoliationSphere*)),
                this, SLOT(foliationSphereSignalEmitted(balazs::FoliationSphere*)));
    }
    {
        newItem = new QListWidgetItem(tr("Random foliation"), listWidget);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        NewFoliationSphereRandomWidget* widget = new NewFoliationSphereRandomWidget;
        map[newItem] = widget;
        connect(widget, SIGNAL(foliationSphere(balazs::FoliationSphere*)),
                this, SLOT(foliationSphereSignalEmitted(balazs::FoliationSphere*)));
    }



    newItem = new QListWidgetItem(tr("RP2"), listWidget);
    newItem->setFont(boldFont);
    newItem->setFlags(Qt::ItemIsEnabled);
    newItem->setBackgroundColor(palette().color(QPalette::Window));
  //  newItem->setForeground(QBrush(Qt::white));

    {
        newItem = new QListWidgetItem(tr("Specify a weighed tree"), listWidget);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        NewFoliationRP2WeighedTreeWidget* widget = new NewFoliationRP2WeighedTreeWidget;
        map[newItem] = widget;
        connect(widget, SIGNAL(foliationRP2(balazs::FoliationRP2*)),
                this, SLOT(foliationRP2SignalEmitted(balazs::FoliationRP2*)));
    }
    {
        newItem = new QListWidgetItem(tr("Random weighed tree"), listWidget);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        NewFoliationRP2RandomWidget* widget = new NewFoliationRP2RandomWidget;
        map[newItem] = widget;
        connect(widget, SIGNAL(foliationRP2(balazs::FoliationRP2*)),
                this, SLOT(foliationRP2SignalEmitted(balazs::FoliationRP2*)));
    }
    {
        newItem = new QListWidgetItem(tr("The Arnoux-Yoccox foliation"), listWidget);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        NewFoliationRP2AYWidget* widget = new NewFoliationRP2AYWidget;
        map[newItem] = widget;
        connect(widget, SIGNAL(foliationRP2(balazs::FoliationRP2*)),
                this, SLOT(foliationRP2SignalEmitted(balazs::FoliationRP2*)));
    }




    detailsStackedWidget = new QStackedWidget;
    for(const auto& x : map){
        detailsStackedWidget->addWidget(x.second);
    }
    connect(listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(onCurrentItemChanged(QListWidgetItem*,QListWidgetItem*)));


    QVBoxLayout* surfaceLayout = new QVBoxLayout;
    surfaceLayout->addWidget(surfaceLabel);
    surfaceLayout->addWidget(listWidget);




    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addLayout(surfaceLayout);
    topLayout->addWidget(detailsStackedWidget, 1);


    setLayout(topLayout);
}





void NewFoliationWindow::onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    (void)previous;
    if(map.find(current) != map.end()){
        detailsStackedWidget->setCurrentWidget(map.at(current));
    }
}



void NewFoliationWindow::foliationSignalEmitted(balazs::Foliation* newFoliation)
{
    emit(foliation(newFoliation));
    hide();
}

void NewFoliationWindow::foliationRP2SignalEmitted(balazs::FoliationRP2 *newFoliationRP2)
{
    emit(foliationRP2(newFoliationRP2));
    hide();
}

void NewFoliationWindow::foliationSphereSignalEmitted(balazs::FoliationSphere* newFoliationSphere)
{
    emit(foliationSphere(newFoliationSphere));
    hide();
}

