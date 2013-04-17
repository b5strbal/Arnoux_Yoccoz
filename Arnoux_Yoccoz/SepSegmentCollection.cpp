#include "SepSegmentCollection.h"

//---------------------------------//
// Foliation::SepSegmentCollection //
//---------------------------------//



SepSegmentCollectionBase::SepSegmentCollectionBase(const SepSegmentDatabase& sepSegmentDatabase,
                                           int numLeafSegments,
                                           int maxdepth) :
    m_sepSegmentDatabase(sepSegmentDatabase),
    m_maxdepth(maxdepth),
    m_separatrixIndices(sepSegmentDatabase.numIntervals(), numLeafSegments)
{
    m_segments.resize(2 * numLeafSegments);
    setIndicesToBegin();
}





bool SepSegmentCollectionBase::isLast(std::list<SeparatrixSegment>::const_iterator it) const{
    it++;
    return (it == m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[it->direction()][it->startingSingularity()].end() ||
    it->depth() > m_maxdepth);
}

void SepSegmentCollectionBase::setIndicesToBegin(){
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

SepSegmentCollectionFromRP2::SepSegmentCollectionFromRP2(const SepSegmentDatabaseFromRP2& database, int numLeafSegments, int maxdepth) :
    SepSegmentCollectionBase(database, numLeafSegments, maxdepth)
{
    assert(numLeafSegments % 2 == 0);
    m_separatrixIndices = Choose(database.numIntervals()/2, numLeafSegments/2);
    m_allowedSeparatrixIndices.reserve(database.numIntervals()/2);
    for (unsigned int i = 0; i < database.numIntervals(); i++){
        if (i < database.m_separatrixPair[i])
            m_allowedSeparatrixIndices.push_back(i);
    }
    setIndicesToBegin();
}


void SepSegmentCollectionFromRP2::setIndicesToBegin(){
    for (unsigned int i = 0; i < m_segments.size()/4; i++){
        m_segments[4 * i] =     m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[UPWARDS][m_separatrixIndices[i]].begin();
        m_segments[4 * i + 1] = m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[DOWNWARDS][m_separatrixIndices[i]].begin();
        m_segments[4 * i + 2] = m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[UPWARDS][m_sepSegmentDatabase.m_separatrixPair[m_separatrixIndices[i]]].begin();
        m_segments[4 * i + 3] = m_sepSegmentDatabase.m_goodShiftedSeparatrixSegments[DOWNWARDS][m_sepSegmentDatabase.m_separatrixPair[m_separatrixIndices[i]]].begin();
    }
    s
}



void SepSegmentCollectionFromRP2::advance() {

}




