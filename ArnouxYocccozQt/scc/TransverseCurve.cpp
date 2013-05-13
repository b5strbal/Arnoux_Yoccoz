#include "TransverseCurve.h"
#include "SeparatrixSegment.h"


balazs::TransverseCurve::TransverseCurve(const Foliation& foliation, const SepSegmentCollection &segments, bool wrapsAroundEnds) :
    m_foliation(foliation)
{
    std::vector<Mod1Number> endpoints;
    endpoints.resize(segments.size());
    m_goodSegmentIndices.resize(segments.size());
    for (std::size_t i = 0; i < segments.size(); i++){
        endpoints.push_back(segments[i]->endpoint());
        m_goodSegmentIndices.push_back(&(*segments[i])); // We will probably have to find a more cleverly sorted way of storing the segments later.
       // std::cout << print() << "\n\n";
    }
    m_disjointIntervals = DisjointIntervals(endpoints, wrapsAroundEnds);

}



bool balazs::operator<(const TransverseCurve& c1, const TransverseCurve& c2) {
    if (c1.length() > c2.length()) return true;
    if (c1.length() < c2.length()) return false;
    auto &points1 = c1.m_disjointIntervals.endpoints();
    auto &points2 = c2.m_disjointIntervals.endpoints();
    if (points1.size() < points2.size()) {
        return true;
    } else if (points1.size() > points2.size())
        return false;
    for (std::size_t i = 0; i < points1.size(); i++) {
        if (points1[i] < points2[i]) {
            return true;
        } else if (points2[i] < points1[i])
            return false;
    }
    return true;
}




std::ostream& balazs::operator<<(std::ostream& out, const TransverseCurve& tc)
{
    out << "Separatrix segments:\n";
    for (auto goodSegmentIndex : tc.m_goodSegmentIndices){
        out << goodSegmentIndex << "\n";
    }
    out << "Connecting arcs: " << tc.m_disjointIntervals << "\n";
    out << "Length: " << tc.length();
    return out;
}


bool balazs::transverseCurve_compare::operator()(const TransverseCurve* cp1, const TransverseCurve* cp2)
{
    return *cp1 < *cp2;
}

