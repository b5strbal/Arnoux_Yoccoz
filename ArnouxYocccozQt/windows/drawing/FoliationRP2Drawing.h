#ifndef FOLIATIONRP2DRAWING_H
#define FOLIATIONRP2DRAWING_H

#include <QWidget>
#include "FoliationDiskDrawing.h"

namespace balazs{
    class FoliationRP2;
}


class FoliationRP2Drawing : public FoliationDiskDrawing
{
    Q_OBJECT
public:
    explicit FoliationRP2Drawing(const balazs::FoliationRP2& fol, int size, QWidget *parent = 0);
signals:

public slots:
    
};

#endif // FOLIATIONRP2DRAWING_H
