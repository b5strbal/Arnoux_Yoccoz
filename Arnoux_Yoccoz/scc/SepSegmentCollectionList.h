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
        iterator& operator++()
            { m_sepSegmentCollection.advance(m_parent.m_maxDepth, m_parent.m_maxInvolvedSingularities); return *this; }

    private:
        iterator(const SepSegmentCollectionList& parent, const SepSegmentCollection &ssc)
            : m_parent(parent), m_sepSegmentCollection(ssc) {}

        const SepSegmentCollectionList& m_parent;
        SepSegmentCollection m_sepSegmentCollection;
    };




    SepSegmentCollectionList(SepSegmentDatabase& sepSegmentDatabase,
                          std::size_t maxDepth,
                          std::size_t maxInvolvedSingularities,
                          const SSCMode& sscMode);

    iterator begin() const { return iterator::beginIterator(*this); }
    iterator end() const { return iterator::endIterator(*this); }


private:
    SepSegmentDatabase& m_sepSegmentDatabase;
    std::size_t m_maxDepth;
    std::size_t m_maxInvolvedSingularities;
    const SSCMode& m_sscMode;
};





inline SepSegmentCollectionList::iterator
SepSegmentCollectionList::iterator::beginIterator(const SepSegmentCollectionList &parent)
{
    return iterator(parent, SepSegmentCollection::firstCollection(parent.m_sepSegmentDatabase, parent.m_sscMode));
}


inline SepSegmentCollectionList::iterator
SepSegmentCollectionList::iterator::endIterator(const SepSegmentCollectionList &parent)
{
    return iterator(parent, SepSegmentCollection::emptyCollection(parent.m_sepSegmentDatabase, parent.m_sscMode));
}







inline bool operator==(const SepSegmentCollectionList::iterator& it1, const SepSegmentCollectionList::iterator& it2)
    { return *it1 == *it2; }

inline bool operator!=(const SepSegmentCollectionList::iterator& it1, const SepSegmentCollectionList::iterator& it2)
    { return !(it1 == it2); }





}




#endif // SEPSEGMENTCOLLECTIONLIST_H
