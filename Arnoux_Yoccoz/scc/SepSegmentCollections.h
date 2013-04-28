#ifndef SEPSEGMENTCOLLECTIONSGENERAL_H
#define SEPSEGMENTCOLLECTIONSGENERAL_H

#include "../math/Choose.h"
#include "SepSegmentDatabase.h"

class SepSegmentDatabase;

namespace balazs{


typedef std::vector<std::list<SeparatrixSegment>::const_iterator> SepSegmentCollection;





class SepSegmentCollections
{
public:
    class iterator{
    public:
        iterator(const SepSegmentCollections& parent);
        static iterator endIterator(const SepSegmentCollections& parent);
        const SepSegmentCollection& operator*() { return m_sepSegmentCollection; }
        iterator& operator++();
        friend bool operator==(const iterator& it1, const iterator& it2);
        friend bool operator!=(const iterator& it1, const iterator& it2);
    private:
        iterator(const SepSegmentCollections &parent,
                 const SepSegmentCollection& sepSegmentCollection,
                 const Choose& sepIndicesChoose);
        void setInitialSetting();

        const SepSegmentCollections& m_parent;
        SepSegmentCollection m_sepSegmentCollection; // if empty, it is the end iterator
        Choose m_sepIndicesChoose;
    };

    enum class Mode{
        SEGMENTS_SHIFTED_TO_SAME_SIDE, // mode parameter: number of leaf segments
        WRAPPING_AROUND_SINGULARITIES_SYMMETRICALLY
    };


    SepSegmentCollections(SepSegmentDatabase& sepSegmentDatabase,
                          unsigned int maxDepth,
                          unsigned int maxInvolvedSingularities,
                          Mode mode,
                          Direction::LeftOrRight shiftToSide = Direction::RIGHT);

    iterator begin() const;
    iterator end() const;

private:
    SepSegmentDatabase& m_sepSegmentDatabase;
    unsigned int m_maxDepth;
    unsigned int m_maxInvolvedSingularities;
    Mode m_mode;
    Direction::LeftOrRight m_shiftToSide;
};







//inline bool isAfterLast() const{ return m_segments.empty(); }


//bool isLast(std::list<SeparatrixSegment>::const_iterator it) const;





}








#endif // SEPSEGMENTCOLLECTIONSGENERAL_H
