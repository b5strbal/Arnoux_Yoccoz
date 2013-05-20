#ifndef SEPSEGMENTCOLLECTIONLIST_H
#define SEPSEGMENTCOLLECTIONLIST_H

#include "SepSegmentCollection.h"


namespace balazs{





class SepSegmentCollectionList
{
public:
    class iterator{
    public:
        iterator(const SepSegmentCollectionList& parent, const begin_tag&);
        iterator(const SepSegmentCollectionList& parent, const end_tag&);
        const SepSegmentCollection& operator*() const { return m_sepSegmentCollection; }
        iterator& operator++()
            { m_sepSegmentCollection.advance(m_parent.m_maxDepth, m_parent.m_maxInvolvedSingularities); return *this; }

    private:


        const SepSegmentCollectionList& m_parent;
        SepSegmentCollection m_sepSegmentCollection;
    };




    SepSegmentCollectionList(SepSegmentDatabase& sepSegmentDatabase,
                          std::size_t maxDepth,
                          std::size_t maxInvolvedSingularities,
                          std::shared_ptr<SSCMode> sscMode);
    SepSegmentCollectionList(const SepSegmentCollectionList&) = delete;
    SepSegmentCollectionList& operator=(const SepSegmentCollectionList&) = delete;

    iterator begin() const { return iterator(*this, balazs::begin_tag()); }
    iterator end() const { return iterator(*this, balazs::end_tag()); }


private:
    SepSegmentDatabase& m_sepSegmentDatabase;
    std::size_t m_maxDepth;
    std::size_t m_maxInvolvedSingularities;
    std::shared_ptr<SSCMode> m_sscMode;
};











inline bool operator==(const SepSegmentCollectionList::iterator& it1, const SepSegmentCollectionList::iterator& it2)
    { return *it1 == *it2; }

inline bool operator!=(const SepSegmentCollectionList::iterator& it1, const SepSegmentCollectionList::iterator& it2)
    { return !(it1 == it2); }





}




#endif // SEPSEGMENTCOLLECTIONLIST_H
