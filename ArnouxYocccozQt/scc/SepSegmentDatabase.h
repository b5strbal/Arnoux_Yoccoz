#ifndef SEPSEGMENTDATABASE_H
#define SEPSEGMENTDATABASE_H

#include <map>
#include <list>
#include "SeparatrixSegment.h"


namespace balazs{


class DisjointIntervals;



struct SepSegmentIndex{
    HDirection hDirection;
    VDirection vDirection;
    std::size_t singularityIntex;
};



class SepSegmentDatabase
{
public:
    SepSegmentDatabase(const Foliation& foliation);
    SepSegmentDatabase(const SepSegmentDatabase&) = delete;
    SepSegmentDatabase& operator=(const SepSegmentDatabase&) = delete;

    const Foliation& foliation() const { return m_foliation; }
    void findNextSepSegment(VDirection direction, int index);
    const SeparatrixSegment& getFirstIntersection(VDirection direction, int index, const DisjointIntervals& intervals);
    bool reachedSaddleConnection(VDirection direction, int index) const;
    void generateSepSegments(std::size_t maxdepth);

    std::list<SeparatrixSegment>::const_iterator firstGoodSegment(const SepSegmentIndex &index) const;
    bool isLast(std::list<SeparatrixSegment>::const_iterator it, std::size_t maxDepth) const;
    const std::list<SeparatrixSegment>& goodSegmentList(const SepSegmentIndex& index) const;

private:
    std::size_t numIntervals() const;
    void addToGoodSegmentsIfGood(const SeparatrixSegment& s);


private:
    const Foliation& m_foliation;
    std::map<VDirection, std::vector<SeparatrixSegment>> m_currentSepSegments;
    std::map<HDirection, std::map<VDirection, std::vector<std::list<SeparatrixSegment>>>> m_goodShiftedSeparatrixSegments;
};


inline SepSegmentIndex getSepSegmentIndex(std::list<SeparatrixSegment>::const_iterator it){
    return { it->side(), it->vDirection(), it->startingSingularity() };
}


}


#endif // SEPSEGMENTDATABASE_H
