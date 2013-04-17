#include "SeparatrixSegment.h"



SeparatrixSegment::SeparatrixSegment(const Foliation& foliation, int startingSingularity, UpDownDirection direction) :
    m_foliation(foliation),
    m_startingSingularity(startingSingularity),
    m_depth(1),
    m_arcsAroundDivPoints(foliation),
    m_intervalIntersectionCount(std::vector<int>(foliation.numIntervals(), 0)),
    m_direction(direction)
{
    if (direction == UPWARDS) {
        m_endpoint = UnitIntervalPoint( foliation.m_bottomRealDivPoints[foliation.m_pairOfTopDivPoints[startingSingularity]].getPosition(), 1);
    } else
        m_endpoint = UnitIntervalPoint( foliation.m_topRealDivPoints[startingSingularity].getPosition(), 1);

    m_smallContainingInterval = containingInterval(foliation.m_allRealDivPoints, m_endpoint);
}






std::string SeparatrixSegment::print(bool verbose) const
{
    std::ostringstream s;
    s << "SEPARATRIX SEGMENT\n";
    s << "(" << m_startingSingularity << ", ";
    s << m_depth << ", ";
    s << (m_direction == UpDownDirection::DOWNWARDS ? "down" : "up") << ", ";
    s << m_endpoint << ")\n";
    if (verbose) {
        s << "IIC: " << m_intervalIntersectionCount << "\n";
        s << "AAD: " << m_arcsAroundDivPoints.print();
    }

    return s.str();
}


void SeparatrixSegment::lengthen()
{
    m_intervalIntersectionCount[containingInterval(m_foliation.m_topRealDivPoints, m_endpoint)]++;
    m_arcsAroundDivPoints.InsertPoint(m_endpoint, m_smallContainingInterval);
    m_depth++;
    m_endpoint = m_direction == UPWARDS ? m_foliation.m_twistedIntervalExchange.applyTo(m_endpoint) : m_foliation.m_twistedIntervalExchange.applyInverseTo(m_endpoint);
    m_endpoint = UnitIntervalPoint(m_endpoint.getPosition()); // remove infinitesimal shift
    m_smallContainingInterval = containingInterval(m_foliation.m_allRealDivPoints, m_endpoint);
}

bool SeparatrixSegment::isGood() const
{
    return m_arcsAroundDivPoints.ContainsQ(m_endpoint, m_smallContainingInterval);
}


