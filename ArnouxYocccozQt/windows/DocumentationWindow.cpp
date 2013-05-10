#include "DocumentationWindow.h"

#include <QTextEdit>
#include <QListView>
#include <QStringListModel>
#include <QHBoxLayout>

DocumentationWindow::DocumentationWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Documentation");

    docItems[tr("The general idea")] = tr("asdf");
    docItems[tr("Weighed Trees")] = tr("asdfg");
    docItems[tr("Foliations")] = tr("asdfgadsf");
    docItems[tr("Transverse Curves")] = tr("asdfadsfg");

    QStringList titleList;
    for(auto it = docItems.begin(); it != docItems.end(); it++){
        titleList << it.key();
    }


    titleListModel = new QStringListModel(titleList);
    titleListView = new QListView();
    titleListView->setModel(titleListModel);
    titleListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    displayArea = new QTextEdit();
    displayArea->setReadOnly(true);

    connect(titleListView, SIGNAL(activated(QModelIndex)), this, SLOT(setDisplayAreaText(QModelIndex)));

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(titleListView);
    layout->addWidget(displayArea);

    setLayout(layout);
}


void DocumentationWindow::setDisplayAreaText(const QModelIndex &index)
{
    displayArea->setText(docItems[titleListModel->data(index, Qt::DisplayRole).toString()]);
}
