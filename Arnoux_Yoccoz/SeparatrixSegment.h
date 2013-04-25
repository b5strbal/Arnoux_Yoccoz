#ifndef SEPARATRIXSEGMENT_H
#define SEPARATRIXSEGMENT_H

#include "Foliation.h"
#include "IntervalNeighborhoods.h"

class Foliation;
class IntervalNeighborhoods;

class SeparatrixSegment{
//    friend class Foliation;
//    friend class TransverseCurve;
    friend class TransverseCurveDatabase;
//    friend class SepSegmentDatabase;
//    friend class SepSegmentDatabaseFromRP2;
private:


public:
    SeparatrixSegment(const Foliation& foliation, int startingSingularity, Direction::UpOrDown direction);
    friend std::ostream& operator<<(std::ostream& out, const SeparatrixSegment& s);
    bool reachedSaddleConnection() const { return m_reachedSaddleConnection; }
    void lengthen();
    bool isGood() const;

    int depth() const { return m_depth; }
    const Mod1NumberIntExchange& endpoint() const { return m_endpoint; }
    int startingSingularity() const { return m_startingSingularity; }
    Direction::UpOrDown direction() const { return m_direction; }
//    const ArcsAroundDivPoints& arcsAroundDivPoints() const { return m_arcsAroundDivPoints; }
//    int smallContainingInterval() const { return m_smallContainingInterval; }

private:
    bool isCentered() const { return !m_endpoint.isGeneralized(); }
    void shiftTo(Direction::LeftOrRight side);

    const Foliation& m_foliation;
    int m_startingSingularity;
    int m_depth;
    Mod1NumberIntExchange m_endpoint;
    int m_smallContainingInterval;
    IntervalNeighborhoods m_intervalNeighborhoods;
    std::vector<int> m_intervalIntersectionCount;
    Direction::UpOrDown m_direction;
    bool m_reachedSaddleConnection;
};




#endif // SEPARATRIXSEGMENT_H
