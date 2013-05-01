#include "SepSegmentCollectionList.h"


balazs::SepSegmentCollectionList::SepSegmentCollectionList(SepSegmentDatabase &sepSegmentDatabase,
                                             unsigned int maxDepth,
                                             unsigned int maxInvolvedSingularities,
                                             SepSegmentCollectionMode mode,
                                             Direction::LeftOrRight shiftToSide) :
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_maxDepth(maxDepth),
    m_maxInvolvedSingularities(maxInvolvedSingularities),
    m_mode(mode),
    m_shiftToSide(shiftToSide)
{
    assert(maxInvolvedSingularities > 0 &&
           maxInvolvedSingularities <= sepSegmentDatabase.foliation().numIntervals());
    if(mode == SepSegmentCollectionMode::SEGMENTS_SHIFTED_TO_SAME_SIDE_FROM_RP2){
        assert(dynamic_cast<const FoliationFromRP2*>(&sepSegmentDatabase.foliation()) != nullptr);
        assert(maxInvolvedSingularities % 2 == 0);
    }

    m_sepSegmentDatabase.generateSepSegments(maxDepth);
}




balazs::SepSegmentCollectionList::iterator balazs::SepSegmentCollectionList::begin() const
{
    return iterator::beginIterator(*this);
}





balazs::SepSegmentCollectionList::iterator balazs::SepSegmentCollectionList::end() const
{

    return iterator::endIterator(*this);
}






balazs::SepSegmentCollectionList::iterator &balazs::SepSegmentCollectionList::iterator::operator ++()
{
    m_sepSegmentCollection.advance(m_parent.m_maxDepth, m_parent.m_maxInvolvedSingularities);
    return *this;
}



balazs::SepSegmentCollectionList::iterator::iterator(const balazs::SepSegmentCollectionList &parent,
                                                     balazs::SepSegmentCollectionList::iterator::Begin) :
    m_parent(parent),
    m_sepSegmentCollection(parent.m_sepSegmentDatabase, parent.m_mode, parent.m_shiftToSide)
{
}



balazs::SepSegmentCollectionList::iterator::iterator(const balazs::SepSegmentCollectionList &parent,
                                                     balazs::SepSegmentCollectionList::iterator::End) :
    m_parent(parent),
    m_sepSegmentCollection(parent.m_sepSegmentDatabase)
{
}






bool balazs::operator ==(const SepSegmentCollectionList::iterator &it1, const SepSegmentCollectionList::iterator &it2)
{
    return *it1 == *it2;
}

bool balazs::operator !=(const SepSegmentCollectionList::iterator &it1, const SepSegmentCollectionList::iterator &it2)
{
    return it1 == it2 ? false : true;
}


balazs::SepSegmentCollectionList::iterator balazs::SepSegmentCollectionList::iterator::beginIterator(const balazs::SepSegmentCollectionList &parent)
{
    return iterator(parent, Begin());
}

balazs::SepSegmentCollectionList::iterator balazs::SepSegmentCollectionList::iterator::endIterator(const balazs::SepSegmentCollectionList &parent)
{
    return iterator(parent, End());
}




