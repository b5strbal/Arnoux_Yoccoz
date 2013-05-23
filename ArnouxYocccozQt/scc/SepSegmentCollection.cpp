#include "SepSegmentCollection.h"
#include "SepSegmentDatabase.h"
#include "SSCMode.h"
#include "../intex/Mod1Number.h"
#include <cassert>

balazs::SepSegmentCollection::SepSegmentCollection(const SepSegmentDatabase& sepSegmentDatabase,
                                                   std::shared_ptr<SSCMode> sscMode,
                                                   const begin_tag &) : // first/begin collection
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_sscMode(sscMode)
{
    assert(&sscMode->foliation() == &m_sepSegmentDatabase.foliation());
    setInitialSetting(Choose(sscMode->howMuchToChooseFrom(), 1));
}


balazs::SepSegmentCollection::SepSegmentCollection(const SepSegmentDatabase& sepSegmentDatabase,
                                                   std::shared_ptr<SSCMode> sscMode,
                                                   const end_tag &) : // empty/end collection
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_sscMode(sscMode)
{
    assert(&sscMode->foliation() == &m_sepSegmentDatabase.foliation());
}

const balazs::Foliation &balazs::SepSegmentCollection::foliation() const
{
    return m_sepSegmentDatabase.foliation();
}







void balazs::SepSegmentCollection::setFirstSegments(const std::vector<SepSegmentIndex>& ssic){
    m_segments.clear();
    m_segments.reserve(ssic.size());
    for(const SepSegmentIndex &index : ssic){
        m_segments.push_back(m_sepSegmentDatabase.firstGoodSegment(index));
    }
}

void balazs::SepSegmentCollection::setSegmentToFirst(std::size_t segmentIndex){
    m_segments[segmentIndex] = m_sepSegmentDatabase.firstGoodSegment(index(*m_segments[segmentIndex]));
}


void balazs::SepSegmentCollection::setInitialSetting(const Choose &sepIndicesChoose){
    m_sepIndicesChoose = sepIndicesChoose;
    setFirstSegments(m_sscMode->initialSegments(sepIndicesChoose));
}


void balazs::SepSegmentCollection::advance(std::size_t maxDepth, std::size_t maxInvolvedSingularities)
{
    assert(!isEmpty());

    int indexToIncrease = m_segments.size() - 1;
    while (indexToIncrease >= 0 && m_sepSegmentDatabase.isLast(m_segments[indexToIncrease], maxDepth)){
        indexToIncrease--;
    }

    if (indexToIncrease >= 0){
        // One of the sepSegments is not the last one, so we can lengthen that, and set the
        // ones after it to the first.

        for (std::size_t index : m_sscMode->segmentsToLengthen(indexToIncrease)){
            m_segments[index]++;
        }

        for (std::size_t index = indexToIncrease + 1; index < m_segments.size(); index++){
             setSegmentToFirst(index);
        }

        for (std::size_t index : m_sscMode->additionalSegmentsToSetToFirst(indexToIncrease)){
            setSegmentToFirst(index);
        }


    } else {
        // All of the segments are the last one, so we have to iterate m_sepIndicesChoose,,
        // the choice of separatrix choices.

        ++m_sepIndicesChoose;

        if(!m_sepIndicesChoose.isAfterLast()){
            // The new Choose is still valid, therefore, we can set all sepSegments to the first one.

            setInitialSetting(m_sepIndicesChoose);

        } else {
            // Even m_sepIndicesChoose was the last one. Now we can try to increase the k-value of Choose,
            // i.e. take larger subsets.

            if(m_sscMode->numInvolvedSingularities(m_sepIndicesChoose) < maxInvolvedSingularities){
                // We can make subsets larger.

                setInitialSetting(Choose(m_sscMode->howMuchToChooseFrom(),
                                         m_sepIndicesChoose.k() + 1));

            } else {
                // We reached the very last collection, there is no next one.

                clear();
            }
        }
    }
}




std::vector<balazs::Mod1NumberIntExchange> balazs::getEndpoints(const SepSegmentCollection &collection)
{
    std::vector<Mod1NumberIntExchange> endpoints;
    endpoints.reserve(collection.size());
    for (std::size_t i = 0; i < collection.size(); i++){
        endpoints.push_back(collection[i]->endpoint());
    }
    return endpoints;
}

