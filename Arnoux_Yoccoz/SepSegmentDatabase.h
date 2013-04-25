#ifndef SEPSEGMENTDATABASE_H
#define SEPSEGMENTDATABASE_H

#include <array>
#include <list>
#include "Foliation.h"
#include "SeparatrixSegment.h"
#include "DisjointIntervals.h"
#include "FoliationFromRP2.h"


class SepSegmentDatabase
{
    friend class TransverseCurveDatabase;
public:
    SepSegmentDatabase(const Foliation& foliation);

    void findNextSepSegment(Direction::UpOrDown direction, int index);
    const SeparatrixSegment& getFirstIntersection(Direction::UpOrDown direction, int index, const DisjointIntervals& intervals);
    bool reachedSaddleConnection(Direction::UpOrDown direction, int index) const;
    void generateSepSegments(int maxdepth);
    void printGoodSepSegments(int maxdepth = 0, bool verbose = false);

protected:
    const Foliation& m_foliation;
    std::array<std::vector<SeparatrixSegment>, 2> m_currentSepSegments;
    std::array<std::vector<std::list<SeparatrixSegment>>, 2> m_goodShiftedSeparatrixSegments;

    unsigned int numIntervals() const { return m_foliation.numIntervals(); }
};




class SepSegmentDatabaseFromRP2 : public SepSegmentDatabase
{
    friend class SepSegmentCollectionFromRP2;
public:
    SepSegmentDatabaseFromRP2(const FoliationFromRP2& foliationFromRP2) :
        SepSegmentDatabase(foliationFromRP2)
      //  m_separatrixPair(foliationFromRP2.m_separatrixPair)
    {}
private:
   // std::vector<unsigned int> m_separatrixPair;
};

#endif // SEPSEGMENTDATABASE_H
