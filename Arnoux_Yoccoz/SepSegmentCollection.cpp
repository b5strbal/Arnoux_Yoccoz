#include "SepSegmentCollection.h"

//---------------------------------//
// Foliation::SepSegmentCollection //
//---------------------------------//



SepSegmentCollectionBase::SepSegmentCollectionBase(const SepSegmentDatabase& sepSegmentDatabase,
                                           const std::vector<int>& sepSegmentIndices,
                                           int maxdepth) :
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_maxdepth(maxdepth)
{
    m_segments.reserve(2 * sepSegmentIndices.size());
    for (int i : sepSegmentIndices){
        m_segments.push_back(m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[UPWARDS][i].begin());
        m_segments.push_back(m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[DOWNWARDS][i].begin());
    }
}





bool SepSegmentCollectionBase::isLast(std::list<SeparatrixSegment>::const_iterator it) const{
    it++;
    return (it == m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[it->m_direction][it->m_startingSingularity].end() ||
    it->m_depth > m_maxdepth);
}



/////////////////////////////////

void SepSegmentCollectionDefault::advance() {
    int indexToIncrease = static_cast<int>(m_segments.size()) - 1;
    while (indexToIncrease >= 0 && isLast(m_segments[indexToIncrease]) ){
        indexToIncrease--;
    }
    if (indexToIncrease < 0){
        m_segments.clear(); // indicating the element after the last one
    } else {
        m_segments[indexToIncrease]++;
        for (unsigned int i = indexToIncrease + 1; i < m_segments.size(); i++){
            m_segments[i] = m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[m_segments[i]->m_direction][m_segments[i]->m_startingSingularity].begin();
        }
    }
}





/////////////////////////////////


void SepSegmentCollectionFromRP2::advance() {

}




