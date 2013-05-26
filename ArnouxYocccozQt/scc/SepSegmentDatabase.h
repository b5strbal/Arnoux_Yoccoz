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
  //  void findNextSepSegment(const SepSegmentIndex &ssIndex); // strong guarantee
    void findNextGoodSegment(const SepSegmentIndex& ssIndex); // strong guarantee
    const SeparatrixSegment& getFirstIntersection(const SepSegmentIndex& ssIndex, const DisjointIntervals& intervals, ShiftMode shiftMode); // strong guarantee
//    bool reachedSaddleConnection(const SepSegmentIndex &ssIndex) const;
    void generateSepSegments(std::size_t maxdepth); // doesn't throw

    std::list<SeparatrixSegment>::const_iterator firstGoodSegment(const SepSegmentIndex &index) const;
    bool isLast(std::list<SeparatrixSegment>::const_iterator it, std::size_t maxDepth) const;
    const std::list<SeparatrixSegment>& goodSegmentList(const SepSegmentIndex& index) const;


private:

    const Foliation& m_foliation;
    std::map<HDirection, std::map<VDirection, std::vector<SeparatrixSegment>>> m_currentSepSegments;
    std::map<HDirection, std::map<VDirection, std::vector<std::list<SeparatrixSegment>>>> m_goodShiftedSeparatrixSegments;
};





}


#endif // SEPSEGMENTDATABASE_H
