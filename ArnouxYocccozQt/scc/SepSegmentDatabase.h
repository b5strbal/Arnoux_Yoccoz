#ifndef SEPSEGMENTDATABASE_H
#define SEPSEGMENTDATABASE_H

#include <array>
#include <list>
#include "SeparatrixSegment.h"


namespace balazs{


class DisjointIntervals;



struct SepSegmentIndex{
    Direction::LeftOrRight m_leftOrRight;
    Direction::UpOrDown m_upOrDown;
    std::size_t m_singularityIntex;
};



class SepSegmentDatabase
{
public:
    SepSegmentDatabase(const Foliation& foliation);

    const Foliation& foliation() const { return m_foliation; }
    void findNextSepSegment(Direction::UpOrDown direction, int index);
    const SeparatrixSegment& getFirstIntersection(Direction::UpOrDown direction, int index, const DisjointIntervals& intervals);
    bool reachedSaddleConnection(Direction::UpOrDown direction, int index) const;
    void generateSepSegments(std::size_t maxdepth);
    void printGoodSepSegments(std::size_t maxdepth = 0, bool verbose = false);

    std::list<SeparatrixSegment>::const_iterator firstGoodSegment(const SepSegmentIndex &index) const;
    bool isLast(std::list<SeparatrixSegment>::const_iterator it, std::size_t maxDepth) const;



private:
    const Foliation& m_foliation;
    std::array<std::vector<SeparatrixSegment>, 2> m_currentSepSegments;
    std::array<std::array<std::vector<std::list<SeparatrixSegment>>, 2>, 2> m_goodShiftedSeparatrixSegments;

    std::size_t numIntervals() const;
    void addToGoodSegmentsIfGood(const SeparatrixSegment& s);
};


inline SepSegmentIndex getSepSegmentIndex(std::list<SeparatrixSegment>::const_iterator it){
    return { it->side(), it->direction(), it->startingSingularity() };
}


}


#endif // SEPSEGMENTDATABASE_H
