#ifndef SEPSEGMENTDATABASE_H
#define SEPSEGMENTDATABASE_H

#include <map>
#include <list>
#include "SeparatrixSegment.h"


namespace balazs{


class DisjointIntervals;







class SepSegmentDatabase
{
public:
    enum ShiftMode { Centered, ShiftedEvenMore };

    SepSegmentDatabase(const Foliation& foliation);
    SepSegmentDatabase(const SepSegmentDatabase&) = delete;
    SepSegmentDatabase& operator=(const SepSegmentDatabase&) = delete;

    const Foliation& foliation() const { return m_foliation; }
    void findNextSepSegment(const SepSegmentIndex &ssIndex);
    const SeparatrixSegment& getFirstIntersection(const SepSegmentIndex& ssIndex, const DisjointIntervals& intervals, ShiftMode shiftMode);
    bool reachedSaddleConnection(const SepSegmentIndex &ssIndex) const;
    void generateSepSegments(std::size_t maxdepth);

    std::list<SeparatrixSegment>::const_iterator firstGoodSegment(const SepSegmentIndex &index) const;
    bool isLast(std::list<SeparatrixSegment>::const_iterator it, std::size_t maxDepth) const;
    const std::list<SeparatrixSegment>& goodSegmentList(const SepSegmentIndex& index) const;

private:
    void addToGoodSegmentsIfGood(const SeparatrixSegment& s);


private:
    const Foliation& m_foliation;
    std::map<HDirection, std::map<VDirection, std::vector<SeparatrixSegment>>> m_currentSepSegments;
    std::map<HDirection, std::map<VDirection, std::vector<std::list<SeparatrixSegment>>>> m_goodShiftedSeparatrixSegments;
};





}


#endif // SEPSEGMENTDATABASE_H
