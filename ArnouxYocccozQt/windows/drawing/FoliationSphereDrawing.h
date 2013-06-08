#ifndef FOLIATIONSPHEREDRAWING_H
#define FOLIATIONSPHEREDRAWING_H

#include <QWidget>

namespace balazs{
    class FoliationSphere;
}

class FoliationSphereDrawing : public QWidget
{
    Q_OBJECT
public:
    explicit FoliationSphereDrawing(const balazs::FoliationSphere &fol, QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // FOLIATIONSPHEREDRAWING_H
