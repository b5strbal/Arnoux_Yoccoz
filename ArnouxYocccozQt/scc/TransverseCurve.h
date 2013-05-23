#ifndef TRANSVERSECURVE_H
#define TRANSVERSECURVE_H

#include "DisjointIntervals.h"
#include "SepSegmentCollection.h"
#include "../math/Permutation.h"

namespace balazs{



class TransverseCurve{
public:
    TransverseCurve(const SepSegmentCollection& segments, bool wrapsAroundEnds);
    TransverseCurve(const TransverseCurve&) = delete;
    TransverseCurve& operator=(const TransverseCurve&) = delete;

    const SepSegmentCollection& sepSegmentCollection() const { return m_sepSegmentCollection; }
    const DisjointIntervals& disjointIntervals() const { return m_disjointIntervals; }

    friend bool operator<(const TransverseCurve& c1, const TransverseCurve& c2);



private:
    SepSegmentCollection m_sepSegmentCollection;
    DisjointIntervals m_disjointIntervals;
};


struct transverseCurve_compare{
    bool operator() (const TransverseCurve* cp1, const TransverseCurve* cp2);
};

}


#endif // TRANSVERSECURVE_H
