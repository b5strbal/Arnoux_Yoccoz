#ifndef TRANSVERSECURVE_H
#define TRANSVERSECURVE_H

#include "DisjointIntervals.h"
#include "SepSegmentCollection.h"


namespace balazs{

class TransverseCurve{
public:
    TransverseCurve(const Foliation& foliation, const SepSegmentCollection& segments, bool wrapsAroundEnds);
    TransverseCurve(const TransverseCurve&) = delete;
    TransverseCurve& operator=(const TransverseCurve&) = delete;

    long double length() const { return m_disjointIntervals.totalLength(); }

    friend bool operator<(const TransverseCurve& c1, const TransverseCurve& c2);



private:
    SepSegmentCollection m_goodSegmentIndices;
    DisjointIntervals m_disjointIntervals;
    const Foliation& m_foliation;
};


struct transverseCurve_compare{
    bool operator() (const TransverseCurve* cp1, const TransverseCurve* cp2);
};

}


#endif // TRANSVERSECURVE_H
