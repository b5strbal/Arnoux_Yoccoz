#ifndef SEPSEGMENTDATABASE_H
#define SEPSEGMENTDATABASE_H

#include <array>
#include <list>
#include "SeparatrixSegment.h"
#include "DisjointIntervals.h"
#include "../fol/FoliationFromRP2.h"

namespace balazs{


class SepSegmentDatabase
{
    friend class SepSegmentCollections;
public:
    SepSegmentDatabase(const Foliation& foliation);

    const Foliation& foliation() const { return m_foliation; }
    void findNextSepSegment(Direction::UpOrDown direction, int index);
    const SeparatrixSegment& getFirstIntersection(Direction::UpOrDown direction, int index, const DisjointIntervals& intervals);
    bool reachedSaddleConnection(Direction::UpOrDown direction, int index) const;
    void generateSepSegments(int maxdepth);
    void printGoodSepSegments(int maxdepth = 0, bool verbose = false);

protected:
    const Foliation& m_foliation;
    std::array<std::vector<SeparatrixSegment>, 2> m_currentSepSegments;
    std::array<std::array<std::vector<std::list<SeparatrixSegment>>, 2>, 2> m_goodShiftedSeparatrixSegments;

    unsigned int numIntervals() const { return m_foliation.numIntervals(); }
    void addToGoodSegmentsIfGood(const SeparatrixSegment& s);
};




class SepSegmentDatabaseFromRP2 : public SepSegmentDatabase
{
public:
    SepSegmentDatabaseFromRP2(const FoliationFromRP2& foliationFromRP2) :
        SepSegmentDatabase(foliationFromRP2)
    {}
private:
};

}


#endif // SEPSEGMENTDATABASE_H
