#include "SepSegmentCollectionList.h"
#include "SepSegmentDatabase.h"
#include "../fol/Foliation.h"

balazs::SepSegmentCollectionList::SepSegmentCollectionList(SepSegmentDatabase &sepSegmentDatabase,
                                             std::size_t maxDepth,
                                             std::size_t maxInvolvedSingularities,
                                             const SSCMode &sscMode) :
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_maxDepth(maxDepth),
    m_maxInvolvedSingularities(maxInvolvedSingularities),
    m_sscMode(sscMode)
{
    assert(maxInvolvedSingularities > 0 &&
           maxInvolvedSingularities <= sepSegmentDatabase.foliation().numIntervals());
    m_sepSegmentDatabase.generateSepSegments(maxDepth);
}











