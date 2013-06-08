#ifndef FOLIATIONLISTWIDGET_H
#define FOLIATIONLISTWIDGET_H

#include <QListWidget>
#include <QResizeEvent>

class FoliationListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit FoliationListWidget(QWidget *parent = 0);
    virtual QSize sizeHint() const { return QSize(100, 200); }
signals:
    
public slots:
    
};

#endif // FOLIATIONLISTWIDGET_H

