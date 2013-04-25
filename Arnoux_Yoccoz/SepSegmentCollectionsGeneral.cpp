#include "SepSegmentCollectionsGeneral.h"

SepSegmentCollections::SepSegmentCollections(SepSegmentDatabase &sepSegmentDatabase,
                                                           unsigned int maxDepth, Mode mode,
                                                           unsigned int modeParameter) :
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_maxDepth(maxDepth),
    m_mode(mode),
    m_modeParameter(modeParameter)
{
    assert(modeParameter > 0 && modeParameter <= sepSegmentDatabase.numIntervals());
}

SepSegmentCollections::iterator SepSegmentCollections::begin() const
{

}

SepSegmentCollections::iterator SepSegmentCollections::end() const
{
}















bool SepSegmentCollection::isLast(std::list<SeparatrixSegment>::const_iterator it) const{
    it++;
    return (it == m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[it->direction()][it->startingSingularity()].end() ||
    it->depth() > m_maxdepth);
}



void SepSegmentCollection::setIndicesToBegin(){
    for (unsigned int i = 0; i < m_segments.size()/2; i++){
        m_segments[2 * i] = m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[UPWARDS][m_separatrixIndices[i]].begin();
        m_segments[2 * i + 1] = m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[DOWNWARDS][m_separatrixIndices[i]].begin();
    }
}

/////////////////////////////////

void SepSegmentCollectionDefault::advance() {
    assert(!isAfterLast());
    int indexToIncrease = static_cast<int>(m_segments.size()) - 1;
    while (indexToIncrease >= 0 && isLast(m_segments[indexToIncrease]) ){
        indexToIncrease--;
    }
    if (indexToIncrease < 0){
        ++m_separatrixIndices;
        if (m_separatrixIndices.isAfterLast()){
            m_segments.clear(); // indicating the element after the last one
        } else {
            setIndicesToBegin();
        }
    } else {
        m_segments[indexToIncrease]++;
        for (unsigned int i = indexToIncrease + 1; i < m_segments.size(); i++){
            m_segments[i] = m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[m_segments[i]->direction()][m_segments[i]->startingSingularity()].begin();
        }
    }
}





/////////////////////////////////


SepSegmentCollections::iterator &SepSegmentCollections::iterator::operator ++()
{
}

bool SepSegmentCollections::iterator::operator ==(const SepSegmentCollections::iterator &it1, const SepSegmentCollections::iterator &it2)
{
    return it1.m_sepSegmentCollection == it2.m_sepSegmentCollection;
}

bool SepSegmentCollections::iterator::operator !=(const SepSegmentCollections::iterator &it1, const SepSegmentCollections::iterator &it2)
{
    return i1 == t2 ? false : true;
}

SepSegmentCollections::iterator::iterator(const SepSegmentCollections &parent) :
    m_parent(parent),
    m_separatrixIndices(Choose(parent.m_sepSegmentDatabase.numIntervals(), parent.m_modeParameter))
{
    switch(parent.m_mode)
    {
        case Mode::SEGMENTS_SHIFTED_TO_RIGHT:






    }
}
