#ifndef FOLIATIONFROMRP2_H
#define FOLIATIONFROMRP2_H

#include "FoliationFromSphere.h"
#include "FoliationRP2.h"
#include <vector>
#include <set>



class FoliationFromRP2 : public FoliationFromSphere
{
    friend class SepSegmentDatabaseFromRP2;
public:
    FoliationFromRP2(const FoliationRP2& foliationRP2);

private:
    std::vector<unsigned int> m_separatrixPair;
};




#endif // FOLIATIONFROMRP2_H
