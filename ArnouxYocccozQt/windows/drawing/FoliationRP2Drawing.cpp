#include "FoliationRP2Drawing.h"
#include "fol/FoliationRP2.h"

FoliationRP2Drawing::FoliationRP2Drawing(const balazs::FoliationRP2& fol, int size, QWidget *parent) :
    FoliationDiskDrawing(fol.foliationDisk(), size, parent)
{
}

