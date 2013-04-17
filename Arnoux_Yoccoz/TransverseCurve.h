#ifndef TRANSVERSECURVE_H
#define TRANSVERSECURVE_H

#include "Foliation.h"
#include "DisjointIntervals.h"
#include "SeparatrixSegment.h"
#include "SepSegmentCollection.h"
#include <list>

class SeparatrixSegment;

class TransverseCurve{
public:
    TransverseCurve(const Foliation& foliation, const SepSegmentCollectionBase& segments, bool wrapsAroundZero);
    floating_point_type length() const { return m_disjointIntervals.totalLength(); }
    std::string print() const;

    friend bool operator<(const TransverseCurve& c1, const TransverseCurve& c2);



private:
    std::vector<const SeparatrixSegment*> m_goodSegmentIndices;
    DisjointIntervals m_disjointIntervals;
    const Foliation& m_foliation;
};


struct transverseCurve_compare{
    bool operator() (const TransverseCurve* cp1, const TransverseCurve* cp2);
};


#endif // TRANSVERSECURVE_H
