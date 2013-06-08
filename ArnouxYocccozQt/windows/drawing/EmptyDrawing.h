#ifndef EMPTYDRAWING_H
#define EMPTYDRAWING_H

#include <QWidget>

class EmptyDrawing : public QWidget
{
    Q_OBJECT
public:
    explicit EmptyDrawing(QWidget *parent = 0);
    
    QSize minimumSizeHint() const { return QSize(200, 200); }
    QSize sizeHint() const { return QSize(500, 500); }
signals:
    
public slots:
    
};

#endif // EMPTYDRAWING_H
