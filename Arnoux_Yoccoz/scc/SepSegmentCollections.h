#ifndef SEPSEGMENTCOLLECTIONSGENERAL_H
#define SEPSEGMENTCOLLECTIONSGENERAL_H

#include "../math/Choose.h"
#include "SepSegmentDatabase.h"


namespace balazs{


typedef std::vector<std::list<SeparatrixSegment>::const_iterator> SepSegmentCollection;



class SepSegmentCollections
{
public:
    class iterator{
    public:
        static iterator beginIterator(const SepSegmentCollections& parent);
        static iterator endIterator(const SepSegmentCollections& parent);
        const SepSegmentCollection& operator*() const { return m_sepSegmentCollection; }
        iterator& operator++();
    private:
        iterator(const SepSegmentCollections& parent);

        void setToEndIterator();
        void setSegmentToFirst(unsigned int segmentIndex);
        void setInitialSetting(const Choose& sepIndicesChoose);
        unsigned int numInvolvedSingularities() const;
        unsigned int howMuchToChooseFrom() const;

        const SepSegmentCollections& m_parent;
        SepSegmentCollection m_sepSegmentCollection; // if empty, it is the end iterator
        Choose m_sepIndicesChoose;
    };

    enum class Mode{
        SEGMENTS_SHIFTED_TO_SAME_SIDE,
        WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY,
        SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2
    };


    SepSegmentCollections(SepSegmentDatabase& sepSegmentDatabase,
                          unsigned int maxDepth,
                          unsigned int maxInvolvedSingularities,
                          Mode mode,
                          Direction::LeftOrRight shiftToSide = Direction::RIGHT);

    iterator begin() const;
    iterator end() const;

private:
    const Foliation& foliation() const;
    const FoliationFromRP2& foliationFromRP2() const;


    SepSegmentDatabase& m_sepSegmentDatabase;
    unsigned int m_maxDepth;
    unsigned int m_maxInvolvedSingularities;
    const Mode m_mode;
    Direction::LeftOrRight m_shiftToSide;

    // This vector is only filled in if the underlying foliation is a FoliationFromRP2.
    // In that case the singularities/separatrices are naturally paired up by the covering map.
    // That pairing is described by FoliationFromRP2::intervalPermutationBeforeHalfTwist().
    // This vector contains the smaller indices of the singularities in each pair.
    // So its length is half of numIntervals of the foliation.
    std::vector<unsigned int> m_choiceOfSingularities_RP2;
};


bool operator==(const SepSegmentCollections::iterator& it1, const SepSegmentCollections::iterator& it2);
bool operator!=(const SepSegmentCollections::iterator& it1, const SepSegmentCollections::iterator& it2);





}




#endif // SEPSEGMENTCOLLECTIONSGENERAL_H
