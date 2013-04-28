#ifndef FOLIATIONFROMRP2_H
#define FOLIATIONFROMRP2_H

#include "FoliationFromSphere.h"
#include "FoliationRP2.h"
#include <vector>

namespace balazs{


class FoliationFromRP2 : public FoliationFromSphere
{
    friend class SepSegmentDatabaseFromRP2;
public:
    FoliationFromRP2(const FoliationRP2& foliationRP2);

private:
    Permutation m_intervalPairing;
};


}

#endif // FOLIATIONFROMRP2_H
