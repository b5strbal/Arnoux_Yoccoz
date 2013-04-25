#ifndef TRANSVERSECURVEDATABASEFROMRP2_H
#define TRANSVERSECURVEDATABASEFROMRP2_H

#include "TransverseCurveDatabase.h"
#include "SepSegmentCollectionsFromRP2.h"

class TransverseCurveDatabaseFromRP2 : public TransverseCurveDatabase
{
public:
    TransverseCurveDatabaseFromRP2(SepSegmentDatabaseFromRP2& sepSegmentDatabaseFromRP2);
    void generateTransverseCurvesFromRP2(int maxdepth, int numLeafComponents);
    void printLiftsOfGoodTransverseCurves(int depth);

private:
    std::set<const TransverseCurve*> m_transverseCurvesFromRP2;
};


#endif // TRANSVERSECURVEDATABASEFROMRP2_H
