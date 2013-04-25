#ifndef TRANSVERSECURVEDATABASE_H
#define TRANSVERSECURVEDATABASE_H

#include "TransverseCurve.h"
#include <set>
#include <array>
#include "SepSegmentDatabase.h"
#include "SepSegmentCollectionsGeneral.h"

class TransverseCurveDatabase
{
public:
    TransverseCurveDatabase(SepSegmentDatabase& sepSegmentDatabase);
    void applyToStoredTransverseCurves(void (*function)(const TransverseCurve&));
    void generateTransverseCurves(int maxdepth, int numLeafComponents, void (*function)(const TransverseCurve&) = nullptr);

protected:
    SepSegmentDatabase& m_sepSegmentDatabase;
    std::set<TransverseCurve> m_transverseCurves;

    std::array<bool, 2> whichTransverseCurvesExist(const SepSegmentCollection& segments);
};



#endif // TRANSVERSECURVEDATABASE_H
