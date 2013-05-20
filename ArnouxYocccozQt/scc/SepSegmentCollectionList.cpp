#include "SepSegmentCollectionList.h"
#include "SepSegmentDatabase.h"
#include "../fol/Foliation.h"
#include <cassert>

balazs::SepSegmentCollectionList::SepSegmentCollectionList(SepSegmentDatabase &sepSegmentDatabase,
                                             std::size_t maxDepth,
                                             std::size_t maxInvolvedSingularities,
                                             std::shared_ptr<SSCMode> sscMode) :
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_maxDepth(maxDepth),
    m_maxInvolvedSingularities(maxInvolvedSingularities),
    m_sscMode(sscMode)
{
    assert(maxInvolvedSingularities > 0 &&
           maxInvolvedSingularities <= sepSegmentDatabase.foliation().numIntervals());
    m_sepSegmentDatabase.generateSepSegments(maxDepth);
}






balazs::SepSegmentCollectionList::iterator::iterator(const SepSegmentCollectionList& parent, const begin_tag&) :
    m_parent(parent),
    m_sepSegmentCollection(parent.m_sepSegmentDatabase, parent.m_sscMode, balazs::begin_tag())
{
}

balazs::SepSegmentCollectionList::iterator::iterator(const SepSegmentCollectionList& parent, const end_tag&) :
    m_parent(parent),
    m_sepSegmentCollection(parent.m_sepSegmentDatabase, parent.m_sscMode, balazs::end_tag())
{
}
