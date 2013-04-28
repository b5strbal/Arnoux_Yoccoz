#include "FoliationFromSphere.h"



balazs::FoliationFromSphere::FoliationFromSphere(const FoliationSphere& foliationSphere) :
    Foliation(Foliation::fromFoliationSphere(foliationSphere))
{
}
