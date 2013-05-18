#ifndef FOLIATIONRP2_H
#define FOLIATIONRP2_H



#include "FoliationDisk.h"


namespace balazs{



class FoliationRP2
{
public:
    FoliationRP2(const WeighedTree &wt);
    FoliationRP2(std::size_t numWeighedTreeEdges); // random foliation
    FoliationRP2(); // the Arnoux-Yoccox foliation
    FoliationRP2(const FoliationRP2&) = delete;
    FoliationRP2& operator=(const FoliationRP2&) = delete;
    const FoliationDisk& foliationDisk() const { return m_foliationDisk; }

private:
    FoliationDisk m_foliationDisk;
    static const long double alpha;

};





}



#endif
