#include "DisjointIntervals.h"


//------------------------------//
// Foliation::DisjointIntervals //
//------------------------------//



DisjointIntervals::DisjointIntervals(const std::vector<UnitIntervalPoint>& unsortedPoints, bool wrapsAroundZero) :
    m_endpoints(unsortedPoints),
    m_wrapsAroundZero(wrapsAroundZero),
    m_totalLength(0)
{
    assert(unsortedPoints.size() % 2 == 0);
    std::sort(m_endpoints.begin(), m_endpoints.end());
    for (unsigned int i = 0; i < m_endpoints.size(); i += 2) {
        m_totalLength += distanceBetween(m_endpoints[i], m_endpoints[i + 1]);
    }
    m_totalLength = wrapsAroundZero ? 1 - m_totalLength : m_totalLength;
}


bool DisjointIntervals::contains(const UnitIntervalPoint& point) const {
    int containingIntervalIndex = containingInterval(m_endpoints, point);
    if (containingIntervalIndex == CONTAINING_INTERVAL_NOT_UNIQUE) {
        throw ExceptionContainingIntervalNotUnique("DisjointIntervals::contains");
    }
    if ((containingIntervalIndex % 2 == 0 && !m_wrapsAroundZero) || (containingIntervalIndex % 2 == 1 && m_wrapsAroundZero)) {
        return true;
    }
    return false;
}





std::string DisjointIntervals::print() const{
    std::ostringstream s;
    for (unsigned int i = m_wrapsAroundZero ? 1 : 0; i < m_endpoints.size() - 2; i += 2) {
        s << printInterval(m_endpoints[i], m_endpoints[i + 1]) << " ";
    }
    if (m_wrapsAroundZero) {
        s << printInterval(m_endpoints.back(), m_endpoints.front());
    } else
        s << printInterval(m_endpoints[m_endpoints.size() - 2], m_endpoints.back());
    return s.str();
}
