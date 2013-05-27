#ifndef TRANSVERSECURVE_H
#define TRANSVERSECURVE_H

#include "DisjointIntervals.h"
#include "SepSegmentCollection.h"
#include "SepSegmentDatabase.h"
#include "../math/Permutation.h"
#include <map>

namespace balazs{

class SmallFoliation;


class TransverseCurve{
public:
    // Constructor throws if touching segments cannot be initialized
    TransverseCurve(const SepSegmentCollection& segments, bool wrapsAroundEnds, SepSegmentDatabase& ssDatabase);
    TransverseCurve(const TransverseCurve&) = delete;
    TransverseCurve& operator=(const TransverseCurve&) = delete;

    const SepSegmentCollection& sepSegmentCollection() const { return m_sepSegmentCollection; }
    const DisjointIntervals& disjointIntervals() const { return m_disjointIntervals; }
    const Foliation& foliation() const;
    Mod1NumberIntExchange length() const { return m_disjointIntervals.totalLength(); }

    const std::vector<Mod1NumberIntExchange>& topIntersections() const { return m_topIntersections; }
    const std::vector<Mod1NumberIntExchange>& bottomRightIntersections() const { return m_bottomRightIntersections; }
    const std::vector<Mod1NumberIntExchange>& bottomLeftIntersections() const { return m_bottomLeftIntersections; }

    Mod1NumberIntExchange distanceOnCurve(const Mod1NumberIntExchange& x, const Mod1NumberIntExchange& y) const;
    const SeparatrixSegment& touchingSepSegment(const SepSegmentIndex& ssIndex, SepSegmentDatabase::ShiftMode shiftMode) const;


    friend bool operator<(const TransverseCurve& c1, const TransverseCurve& c2);



private:
    void initIntervalsInOrder();
    void initTouchingSegments();

    SepSegmentCollection m_sepSegmentCollection;
    DisjointIntervals m_disjointIntervals;
    SepSegmentDatabase& m_sepSegmentDatabase;

    std::vector<Mod1NumberIntExchange> m_intervalsInOrder;
    std::vector<Mod1NumberIntExchange> m_topIntersections;
    std::vector<Mod1NumberIntExchange> m_bottomRightIntersections;
    std::vector<Mod1NumberIntExchange> m_bottomLeftIntersections;

    std::map<HDirection, std::map<VDirection, std::map<SepSegmentDatabase::ShiftMode, std::vector<const SeparatrixSegment*>>>> m_touchingSegments;
};


struct transverseCurve_compare{
    bool operator() (const TransverseCurve* cp1, const TransverseCurve* cp2);
};

}


#endif // TRANSVERSECURVE_H
