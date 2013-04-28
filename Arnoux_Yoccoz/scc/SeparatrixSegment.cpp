#include "SeparatrixSegment.h"
#include "Modint.h"


balazs::SeparatrixSegment::SeparatrixSegment(const Foliation& foliation, int startingSingularity, Direction::UpOrDown direction) :
    m_foliation(foliation),
    m_startingSingularity(startingSingularity),
    m_depth(1),
    m_endpoint(foliation.firstIntersection(startingSingularity, direction)),
    m_smallContainingInterval(foliation.smallContainingInterval(m_endpoint)), // m_endpoint should be defined by this
    m_intervalNeighborhoods(foliation),
    m_intervalIntersectionCount(std::vector<int>(foliation.numIntervals(), 0)),
    m_direction(direction),
    m_reachedSaddleConnection(false),
    m_smallIntervalOfFirstIntersection(m_smallContainingInterval)
{
}






std::ostream& balazs::operator<<(std::ostream& out, const SeparatrixSegment& s)
{
    out << "SEPARATRIX SEGMENT\n";
    out << "(" << s.m_startingSingularity << ", ";
    out << s.m_depth << ", ";
    out << (s.m_direction == Direction::DOWN ? "down" : "up") << ", ";
    out << s.m_endpoint << ")\n";
//    if (verbose) {
//        out << "IIC: " << s.m_intervalIntersectionCount << "\n";
//        out << "AAD: " << s.m_arcsAroundDivPoints;
//    }

    return out;
}


void balazs::SeparatrixSegment::lengthen()
{
    assert(isCentered());
    if(m_depth > 1){
        m_intervalIntersectionCount[containingInterval(m_foliation.topDivPoints(), m_endpoint)]++;
        m_intervalNeighborhoods.insertPoint(m_endpoint, m_smallContainingInterval);
    }
    m_depth++;
    m_endpoint = (m_direction == Direction::UP) ? m_foliation.m_twistedIntervalExchange.applyTo(m_endpoint) :
                                            m_foliation.m_twistedIntervalExchange.applyInverseTo(m_endpoint);
    m_smallContainingInterval = m_foliation.smallContainingInterval(m_endpoint);
    if(distanceBetween(m_foliation.m_allDivPoints[m_smallContainingInterval], m_endpoint) < PRECISION ||
       distanceBetween(m_endpoint,
       m_foliation.m_allDivPoints[Modint(m_smallContainingInterval + 1, m_foliation.numSeparatrices())]) < PRECISION){
        m_reachedSaddleConnection = true;
       }
}

bool balazs::SeparatrixSegment::isGood() const
{
    return m_intervalNeighborhoods.containsInTwoSidedInterval(m_endpoint, m_smallContainingInterval,
                                                              m_smallIntervalOfFirstIntersection);
}

void balazs::SeparatrixSegment::shiftTo(Direction::LeftOrRight side)
{
    assert(isCentered());
    m_endpoint.shiftTo(side);

    // inserting the first intersection that we have previously omitted
    Mod1Number pointToInsert = m_foliation.firstIntersection(m_startingSingularity, m_direction);
    pointToInsert.shiftTo(side);
    unsigned int smallContainingInterval = m_foliation.smallContainingInterval(pointToInsert);
    m_intervalNeighborhoods.insertPoint(pointToInsert, smallContainingInterval);

    m_intervalIntersectionCount[smallContainingInterval]++;
}


