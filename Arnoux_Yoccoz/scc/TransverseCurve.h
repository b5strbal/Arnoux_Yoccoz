#ifndef TRANSVERSECURVE_H
#define TRANSVERSECURVE_H

#include "DisjointIntervals.h"
#include "SepSegmentCollections.h"


namespace balazs{


class TransverseCurve{
public:
    TransverseCurve(const Foliation& foliation, const SepSegmentCollection& segments, bool wrapsAroundEnds);
    floating_point_type length() const { return m_disjointIntervals.totalLength(); }
    friend std::ostream & operator<<(std::ostream &out, const TransverseCurve &tc);

    friend bool operator<(const TransverseCurve& c1, const TransverseCurve& c2);



private:
    std::vector<const SeparatrixSegment*> m_goodSegmentIndices;
    DisjointIntervals m_disjointIntervals;
    const Foliation& m_foliation;
};


struct transverseCurve_compare{
    bool operator() (const TransverseCurve* cp1, const TransverseCurve* cp2);
};

}


#endif // TRANSVERSECURVE_H
