#ifndef FOLIATIONDISK_H
#define FOLIATIONDISK_H

#include "WeighedTree.h"
#include "IntervalExchangeMap.h"

class FoliationDisk
{
public:
    FoliationDisk(const WeighedTree& wt);

    const WeighedTree& weighedTree() const { return m_weighedTree; }
    const IntervalExchangeFoliationDisk& intervalPairing() const { return m_intervalPairing; }
    int numSeparatrices() const { return m_intervalPairing.size(); }
    const std::vector<UnitIntervalPoint>& divPoints() const { return m_intervalPairing.divPoints(); }

    friend std::ostream& operator<<(std::ostream& Out, const FoliationDisk& f);
private:
    IntervalExchangeFoliationDisk m_intervalPairing;
    WeighedTree m_weighedTree;

};

#endif // FOLIATIONDISK_H
