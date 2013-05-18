#ifndef FOLIATIONSPHERE_H
#define FOLIATIONSPHERE_H

#include "FoliationDisk.h"

namespace balazs{

class FoliationRP2;

// Both foliations are seen from above.
// The top foliation is fixed in the standard position, and the bottom foliation is twisted.
class FoliationSphere{
public:
    FoliationSphere(const WeighedTree& topWeighedTree, const WeighedTree& bottomWeighedTree, long double twist);
    FoliationSphere(const FoliationSphere&) = delete;
    FoliationSphere& operator=(const FoliationSphere&) = delete;

    FoliationSphere(const FoliationRP2& foliationRP2);
    const FoliationDisk& topFoliation() const { return m_topFoliation; }
    const FoliationDisk& bottomFoliation() const { return m_bottomFoliation; }
    long double twist() const { return m_twist; }

private:
    FoliationDisk m_topFoliation;
    FoliationDisk m_bottomFoliation;
    long double m_twist;
};



}


#endif // FOLIATIONSPHERE_H
