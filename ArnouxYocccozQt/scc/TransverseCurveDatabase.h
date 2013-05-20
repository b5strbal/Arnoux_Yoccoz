#ifndef TRANSVERSECURVEDATABASE_H
#define TRANSVERSECURVEDATABASE_H

#include <set>
#include <array>
#include "TransverseCurve.h"

namespace balazs{


class TransverseCurveDatabase
{
public:
    TransverseCurveDatabase(SepSegmentDatabase& sepSegmentDatabase);
    TransverseCurveDatabase(const TransverseCurveDatabase&) = delete;
    TransverseCurveDatabase& operator=(const TransverseCurveDatabase&) = delete;

    void applyToStoredTransverseCurves(void (*function)(const TransverseCurve&));
    void generateTransverseCurves(int maxdepth, int numLeafComponents, std::shared_ptr<SSCMode> sscmode, void (*function)(const TransverseCurve&) = nullptr);
    const Foliation& foliation() const;

private:
    SepSegmentDatabase& m_sepSegmentDatabase;
    std::set<TransverseCurve> m_transverseCurves;

    std::array<bool, 2> whichTransverseCurvesExist(const SepSegmentCollection& segments);
};

}


#endif // TRANSVERSECURVEDATABASE_H
