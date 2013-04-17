#ifndef FOLIATIONSPHERE_H
#define FOLIATIONSPHERE_H

#include "FoliationDisk.h"
#include "FoliationRP2.h"

// Both foliations are seen from above.
// The top foliation is fixed in the standard position, and the bottom foliation is twisted.
class FoliationSphere{
public:
    FoliationSphere(const FoliationDisk& topFoliation, const FoliationDisk& bottomFoliation, floating_point_type twist);
    FoliationSphere(const FoliationRP2& foliationRP2);
    const FoliationDisk& topFoliation() const { return m_topFoliation; }
    const FoliationDisk& bottomFoliation() const { return m_bottomFoliation; }
    floating_point_type twist() const { return m_twist; }

    friend std::ostream& operator<<(std::ostream& Out, const FoliationSphere&);

private:
    FoliationDisk m_topFoliation;
    FoliationDisk m_bottomFoliation;
    floating_point_type m_twist;

};

#endif // FOLIATIONSPHERE_H
