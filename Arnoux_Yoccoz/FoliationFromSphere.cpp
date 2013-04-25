#include "FoliationFromSphere.h"

//---------------------//
// FoliationFromSphere //
//---------------------//


FoliationFromSphere::FoliationFromSphere(const FoliationSphere& foliationSphere) :
    Foliation(Foliation::fromFoliationSphere(foliationSphere))
{
}
