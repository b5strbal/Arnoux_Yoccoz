#ifndef FOLIATIONFROMRP2_H
#define FOLIATIONFROMRP2_H

#include "FoliationFromSphere.h"

namespace balazs{

class FoliationRP2;

class FoliationFromRP2 : public FoliationFromSphere
{
public:
    FoliationFromRP2(const FoliationRP2& foliationRP2);
    virtual ~FoliationFromRP2() {}
    static Foliation fromFoliationRP2(const FoliationRP2 &foliationRP2);
    const Permutation& intervalPermutationBeforeHalfTwist() const;
private:
    Permutation m_intervalPermutationBeforeHalfTwist;
};


}

#endif // FOLIATIONFROMRP2_H
