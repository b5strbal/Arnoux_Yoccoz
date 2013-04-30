#include "SeparatrixSegment.h"


balazs::SeparatrixSegment::SeparatrixSegment(const Foliation& foliation, unsigned int startingSingularity, Direction::UpOrDown direction) :
    m_foliation(foliation),
    m_startingSingularity(startingSingularity),
    m_depth(1),
    m_endpoint(foliation.firstIntersection(startingSingularity, direction), &foliation),
    m_intervalNeighborhoods(foliation),
    m_intervalIntersectionCount(std::vector<unsigned int>(foliation.numIntervals(), 0)),
    m_direction(direction),
    m_reachedSaddleConnection(false)
{
}






std::ostream& balazs::operator<<(std::ostream& out, const SeparatrixSegment& s)
{
    out << "SEPARATRIX SEGMENT\n";
    out << "(" << s.m_startingSingularity << ", ";
    out << s.m_depth << ", ";
    out << (s.m_direction == Direction::DOWN ? "down" : "up") << ", ";
    out << s.m_endpoint.number() << ")\n";

    return out;
}


void balazs::SeparatrixSegment::lengthen()
{
    assert(isCentered());
    if(m_depth > 1){
        m_intervalIntersectionCount[containingInterval(m_foliation.topDivPoints(), m_endpoint)]++;
        m_intervalNeighborhoods.insertPoint(m_endpoint);
    }
    m_depth++;
    Mod1NumberIntExchange newPoint = (m_direction == Direction::UP) ?
                m_foliation.intExchange().applyTo(m_endpoint.number()) :
                m_foliation.intExchange().applyInverseTo(m_endpoint.number());
    m_endpoint = Mod1NumberIntExWithInfo(newPoint, &m_foliation);

    if(m_endpoint.isTooCloseToADivPoint()){
        m_reachedSaddleConnection = true;
    }
}

bool balazs::SeparatrixSegment::isGood() const
{
    assert(!isCentered());
    return m_intervalNeighborhoods.containsInTwoSidedInterval(m_endpoint);
}

void balazs::SeparatrixSegment::shiftTo(Direction::LeftOrRight side)
{
    assert(isCentered());
    m_endpoint = m_endpoint.shiftedTo(side);

    // inserting the first intersection that we have previously omitted
    Mod1NumberIntExWithInfo pointToInsert(m_foliation.firstIntersection(m_startingSingularity,
                                                                        m_direction).shiftedTo(side), &m_foliation);
    m_intervalNeighborhoods.insertPoint(pointToInsert);

    m_intervalIntersectionCount[pointToInsert.smallContainingInterval()]++;
}


