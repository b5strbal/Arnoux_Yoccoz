#ifndef TRANSVERSECURVEDATABASE_H
#define TRANSVERSECURVEDATABASE_H

#include "TransverseCurve.h"
#include <set>
#include <array>
#include "SepSegmentDatabase.h"
#include "SepSegmentCollections.h"

namespace balazs{


class TransverseCurveDatabase
{
public:
    TransverseCurveDatabase(SepSegmentDatabase& sepSegmentDatabase);
    void applyToStoredTransverseCurves(void (*function)(const TransverseCurve&));
    void generateTransverseCurves(int maxdepth, int numLeafComponents, void (*function)(const TransverseCurve&) = nullptr);
    const Foliation& foliation() const { return m_sepSegmentDatabase.foliation(); }
protected:
    SepSegmentDatabase& m_sepSegmentDatabase;
    std::set<TransverseCurve> m_transverseCurves;

    std::array<bool, 2> whichTransverseCurvesExist(const SepSegmentCollection& segments);
};

}


#endif // TRANSVERSECURVEDATABASE_H
