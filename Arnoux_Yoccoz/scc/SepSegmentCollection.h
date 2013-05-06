#ifndef SEPSEGMENTCOLLECTION_H
#define SEPSEGMENTCOLLECTION_H

#include "../math/Choose.h"
#include "SepSegmentDatabase.h"

namespace balazs{

enum class SepSegmentCollectionMode{
    SEGMENTS_SHIFTED_TO_SAME_SIDE,
    WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY,
    SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2
};



class SepSegmentCollection
{
public:
    SepSegmentCollection(const SepSegmentDatabase& sepSegmentDatabase); // empty collection
    SepSegmentCollection(const SepSegmentDatabase& sepSegmentDatabase, // first collection
                            SepSegmentCollectionMode mode,
                            Direction::LeftOrRight shiftToSide = Direction::RIGHT);

    sepSegmentDatabaseConstIterator_t operator[](std::size_t index) const;
    bool isEmpty() const { return m_segments.empty(); }
    void clear() { m_segments.clear(); }
    std::size_t size() const { return m_segments.size(); }
    std::vector<sepSegmentDatabaseConstIterator_t>::iterator begin() { return m_segments.begin(); }
    std::vector<sepSegmentDatabaseConstIterator_t>::iterator end() { return m_segments.end(); }
    void setInitialSetting(const Choose &sepIndicesChoose);

    void advance(std::size_t maxDepth, std::size_t maxInvolvedSingularities);

    friend bool operator==(const SepSegmentCollection& lhs, const SepSegmentCollection& rhs){
        return lhs.m_segments == rhs.m_segments;
    }
private:
    SepSegmentCollection(const SepSegmentDatabase& sepSegmentDatabase,
                         const Choose& sepIndicesChoose,
                         SepSegmentCollectionMode mode,
                         Direction::LeftOrRight shiftToSide = Direction::RIGHT);

    std::size_t numInvolvedSingularities() const;
    void setSegmentToFirst(std::size_t segmentIndex);
    const Foliation& foliation() const;
    const FoliationFromRP2& foliationFromRP2() const;

    const SepSegmentDatabase& m_sepSegmentDatabase;
    std::vector<sepSegmentDatabaseConstIterator_t> m_segments;
    Choose m_sepIndicesChoose;
    SepSegmentCollectionMode m_mode;
    Direction::LeftOrRight m_shiftToSide;

    // This vector is only filled in if the underlying foliation is a FoliationFromRP2.
    // In that case the singularities/separatrices are naturally paired up by the covering map.
    // That pairing is described by FoliationFromRP2::intervalPermutationBeforeHalfTwist().
    // This vector contains the smaller indices of the singularities in each pair.
    // So its length is half of numIntervals of the foliation.
    std::vector<std::size_t> m_choiceOfSingularities_RP2;
};



std::size_t howMuchToChooseFrom(SepSegmentCollectionMode mode, std::size_t numSingularities);



}

#endif // SEPSEGMENTCOLLECTION_H
