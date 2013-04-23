#include "DisjointIntervals.h"


//------------------------------//
// Foliation::DisjointIntervals //
//------------------------------//



DisjointIntervals::DisjointIntervals(const std::vector<Mod1Number>& unsortedPoints, bool wrapsAroundZero) :
    m_endpoints(unsortedPoints),
    m_wrapsAroundZero(wrapsAroundZero),
    m_totalLength(0)
{
    assert(unsortedPoints.size() % 2 == 0);
    std::sort(m_endpoints.begin(), m_endpoints.end());
    for (unsigned int i = 0; i < m_endpoints.size(); i += 2) {
        m_totalLength += Mod1Number::distanceBetween(m_endpoints[i], m_endpoints[i + 1]);
    }
    m_totalLength = wrapsAroundZero ? 1 - m_totalLength : m_totalLength;
}


bool DisjointIntervals::contains(const Mod1Number& point) const {
    int containingIntervalIndex = containingInterval(m_endpoints, point);
    if (containingIntervalIndex == CONTAINING_INTERVAL_NOT_UNIQUE) {
        throw std::runtime_error("DisjointIntervals::contains"); // ExceptionContainingIntervalNotUnique ?
    }
    if ((containingIntervalIndex % 2 == 0 && !m_wrapsAroundZero) || (containingIntervalIndex % 2 == 1 && m_wrapsAroundZero)) {
        return true;
    }
    return false;
}





std::ostream& operator<<(std::ostream& out, const DisjointIntervals& d){
    for (unsigned int i = d.m_wrapsAroundZero ? 1 : 0; i < d.m_endpoints.size() - 2; i += 2) {
        out << Mod1Number::printInterval(d.m_endpoints[i], d.m_endpoints[i + 1]) << " ";
    }
    if (d.m_wrapsAroundZero) {
        out << Mod1Number::printInterval(d.m_endpoints.back(), d.m_endpoints.front());
    } else
        out << Mod1Number::printInterval(d.m_endpoints[d.m_endpoints.size() - 2], d.m_endpoints.back());
    return out;
}
