#ifndef SEPSEGMENTCOLLECTIONLIST_H
#define SEPSEGMENTCOLLECTIONLIST_H

#include "SepSegmentCollection.h"


namespace balazs{





class SepSegmentCollectionList
{
public:
    class iterator{
    public:
        static iterator beginIterator(const SepSegmentCollectionList& parent);
        static iterator endIterator(const SepSegmentCollectionList& parent);
        const SepSegmentCollection& operator*() const { return m_sepSegmentCollection; }
        iterator& operator++();
    private:
        class Begin {};
        class End {};
        iterator(const SepSegmentCollectionList& parent, Begin); // constructs end iterator
        iterator(const SepSegmentCollectionList& parent, End); // constructs end iterator


        const SepSegmentCollectionList& m_parent;
        SepSegmentCollection m_sepSegmentCollection;
    };




    SepSegmentCollectionList(SepSegmentDatabase& sepSegmentDatabase,
                          std::size_t maxDepth,
                          std::size_t maxInvolvedSingularities,
                          SepSegmentCollectionMode mode,
                          Direction::LeftOrRight shiftToSide = Direction::RIGHT);

    iterator begin() const;
    iterator end() const;

private:



    SepSegmentDatabase& m_sepSegmentDatabase;
    std::size_t m_maxDepth;
    std::size_t m_maxInvolvedSingularities;
    const SepSegmentCollectionMode m_mode;
    Direction::LeftOrRight m_shiftToSide;
};


bool operator==(const SepSegmentCollectionList::iterator& it1, const SepSegmentCollectionList::iterator& it2);
bool operator!=(const SepSegmentCollectionList::iterator& it1, const SepSegmentCollectionList::iterator& it2);





}




#endif // SEPSEGMENTCOLLECTIONLIST_H
