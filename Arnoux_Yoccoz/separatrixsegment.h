#ifndef SEPARATRIXSEGMENT_H
#define SEPARATRIXSEGMENT_H

#include "Foliation.h"
#include "ArcsAroundDivPoints.h"

class Foliation;
class ArcsAroundDivPoints;

class SeparatrixSegment{
    friend class Foliation;
    friend class SepSegmentCollectionBase;
    friend class SepSegmentCollectionDefault;
    friend class TransverseCurve;
    friend class TransverseCurveDatabase;
    friend class SepSegmentDatabase;
    friend class SepSegmentDatabaseFromRP2;
    friend class TransverseCurveFinder;
public:
    SeparatrixSegment(const Foliation& foliation, int startingSingularity, UpDownDirection direction);
    std::string print(bool verbose = false) const;
    bool reachedSaddleConnection() const { return m_smallContainingInterval == CONTAINING_INTERVAL_NOT_UNIQUE; }
    void lengthen();
    bool isGood() const;

//    int depth() const { return m_depth; }
//    UnitIntervalPoint endpoint() const { return m_endpoint; }
//    int startingSingularity() const { return m_startingSingularity; }
//    UpDownDirection direction() const { return m_direction; }
//    const ArcsAroundDivPoints& arcsAroundDivPoints() const { return m_arcsAroundDivPoints; }
//    int smallContainingInterval() const { return m_smallContainingInterval; }

private:
    const Foliation& m_foliation;
    int m_startingSingularity;
    int m_depth;
    UnitIntervalPoint m_endpoint;
    int m_smallContainingInterval;
    ArcsAroundDivPoints m_arcsAroundDivPoints;
    std::vector<int> m_intervalIntersectionCount;
    UpDownDirection m_direction;
};




#endif // SEPARATRIXSEGMENT_H
