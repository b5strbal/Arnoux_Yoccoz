#ifndef FOLIATIONFROMSPHERE_H
#define FOLIATIONFROMSPHERE_H

#include "Foliation.h"

namespace balazs{

class FoliationFromSphere : public Foliation
{
public:
    FoliationFromSphere(const FoliationSphere& foliationSphere);
};

}

#endif // FOLIATIONFROMSPHERE_H
