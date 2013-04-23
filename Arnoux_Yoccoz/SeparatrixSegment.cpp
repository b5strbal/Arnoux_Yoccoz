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
        m_endpoint = Mod1Number( foliation.m_bottomRealDivPoints[foliation.m_pairOfTopDivPoints[startingSingularity]].getPosition(), 1);
    } else
        m_endpoint = Mod1Number( foliation.m_topRealDivPoints[startingSingularity].getPosition(), 1);

    m_smallContainingInterval = containingInterval(foliation.m_allRealDivPoints, m_endpoint);
}






std::ostream& operator<<(std::ostream& out, const SeparatrixSegment& s)
{
    out << "SEPARATRIX SEGMENT\n";
    out << "(" << s.m_startingSingularity << ", ";
    out << s.m_depth << ", ";
    out << (s.m_direction == UpDownDirection::DOWNWARDS ? "down" : "up") << ", ";
    out << s.m_endpoint << ")\n";
//    if (verbose) {
//        out << "IIC: " << s.m_intervalIntersectionCount << "\n";
//        out << "AAD: " << s.m_arcsAroundDivPoints;
//    }

    return out;
}


void SeparatrixSegment::lengthen()
{
    m_intervalIntersectionCount[containingInterval(m_foliation.m_topRealDivPoints, m_endpoint)]++;
    m_arcsAroundDivPoints.insertPoint(m_endpoint, m_smallContainingInterval);
    m_depth++;
    m_endpoint = m_direction == UPWARDS ? m_foliation.m_twistedIntervalExchange.applyTo(m_endpoint) : m_foliation.m_twistedIntervalExchange.applyInverseTo(m_endpoint);
    m_endpoint = Mod1Number(m_endpoint.getPosition()); // remove infinitesimal shift
    m_smallContainingInterval = containingInterval(m_foliation.m_allRealDivPoints, m_endpoint);
}

bool SeparatrixSegment::isGood() const
{
    return m_arcsAroundDivPoints.contains(m_endpoint, m_smallContainingInterval);
}


