#ifndef FOLIATIONRP2_H
#define FOLIATIONRP2_H



#include "FoliationDisk.h"


namespace balazs{



class FoliationRP2
{
public:
    FoliationRP2(const FoliationDisk &fd);
    const FoliationDisk& foliationDisk() const { return m_foliationDisk; }

private:
    FoliationDisk m_foliationDisk;
};

FoliationRP2 randomFoliationRP2(std::size_t numWeighedTreeEdges);
FoliationRP2 arnouxYoccozRP2();
std::ostream& operator<<(std::ostream& out, const FoliationRP2& f);



}



#endif
