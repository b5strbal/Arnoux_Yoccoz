#ifndef TRANSVERSECURVEDATABASE_H
#define TRANSVERSECURVEDATABASE_H

#include "TransverseCurve.h"
#include <set>
#include <array>
#include "SepSegmentDatabase.h"
#include "SepSegmentCollection.h"

class TransverseCurveDatabase
{
public:
    TransverseCurveDatabase(SepSegmentDatabase& sepSegmentDatabase);
    void applyToStoredTransverseCurves(void (*function)(const TransverseCurve&));
    void generateTransverseCurves(int maxdepth, int numLeafComponents, void (*function)(const TransverseCurve&) = nullptr);

protected:
    SepSegmentDatabase& m_sepSegmentDatabase;
    std::set<TransverseCurve> m_transverseCurves;

    std::array<bool, 2> whichTransverseCurvesExist(const SepSegmentCollectionBase& segments);
};


class TransverseCurveDatabaseFromRP2 : public TransverseCurveDatabase
{
public:
    TransverseCurveDatabaseFromRP2(SepSegmentDatabaseFromRP2& sepSegmentDatabaseFromRP2);
    void generateTransverseCurvesFromRP2(int maxdepth, int numLeafComponents);
    void printLiftsOfGoodTransverseCurves(int depth);

private:
    std::set<const TransverseCurve*> m_transverseCurvesFromRP2;
};


#endif // TRANSVERSECURVEDATABASE_H
