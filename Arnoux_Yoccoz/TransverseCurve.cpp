#include "TransverseCurve.h"

//----------------------------//
// Foliation::TransverseCurve //
//----------------------------//



TransverseCurve::TransverseCurve(const Foliation& foliation, const SepSegmentCollectionBase &segments, bool wrapsAroundZero) :
    m_foliation(foliation)
{
    std::vector<Mod1Number> endpoints;
    endpoints.reserve(segments.size());
    m_goodSegmentIndices.reserve(segments.size());
    for (unsigned int i = 0; i < segments.size(); i++){
        endpoints.push_back(segments[i].endpoint());
        m_goodSegmentIndices.push_back(&segments[i]); // We will probably have to find a more cleverly sorted way of storing the segments later.
       // std::cout << print() << "\n\n";
    }
    m_disjointIntervals = DisjointIntervals(endpoints, wrapsAroundZero);

}



bool operator<(const TransverseCurve& c1, const TransverseCurve& c2) {
    if (c1.length() > c2.length()) return true;
    if (c1.length() < c2.length()) return false;
    auto &points1 = c1.m_disjointIntervals.endpoints();
    auto &points2 = c2.m_disjointIntervals.endpoints();
    if (points1.size() < points2.size()) {
        return true;
    } else if (points1.size() > points2.size())
        return false;
    for (unsigned int i = 0; i < points1.size(); i++) {
        if (points1[i] < points2[i]) {
            return true;
        } else if (points2[i] < points1[i])
            return false;
    }
    return true;
}




std::string TransverseCurve::print() const
{
    std::ostringstream s;
    s << "Separatrix segments:\n";
    for (auto goodSegmentIndex : m_goodSegmentIndices){
        s << goodSegmentIndex->print() << "\n";
    }
    s << "Connecting arcs: " << m_disjointIntervals.print() << "\n";
    s << "Length: " << length();
    return s.str();
}


bool transverseCurve_compare::operator()(const TransverseCurve* cp1, const TransverseCurve* cp2)
{
    return *cp1 < *cp2;
}

