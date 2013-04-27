#include "SepSegmentCollectionsFromRP2.h"







//SepSegmentCollectionsFromRP2::SepSegmentCollectionsFromRP2(SepSegmentDatabaseFromRP2& database,
//                                                           unsigned int maxDepth, Mode mode, unsigned int modeParameter) :
//    SepSegmentCollections(database, maxDepth, mode, modeParameter)
//{
//    assert(maxDepth % 2 == 0);
//    m_separatrixIndices = Choose(database.numIntervals()/2, maxDepth/2);
//    m_allowedSeparatrixIndices.reserve(database.numIntervals()/2);
//    for (unsigned int i = 0; i < database.numIntervals(); i++){
//        if (i < database.m_separatrixPair[i])
//            m_allowedSeparatrixIndices.push_back(i);
//    }
//    setIndicesToBegin();
//}


//void SepSegmentCollectionsFromRP2::setIndicesToBegin(){
//    const SepSegmentDatabaseFromRP2& sepSegmentDatabaseFromRP2 =
//            static_cast<const SepSegmentDatabaseFromRP2&>(m_sepSegmentDatabase);

//    for (unsigned int i = 0; i < m_segments.size()/4; i++){
//        m_segments[4 * i] = sepSegmentDatabaseFromRP2.m_goodShiftedSeparatrixSegments[UPWARDS][m_separatrixIndices[i]].begin();
//        m_segments[4 * i + 1] = sepSegmentDatabaseFromRP2.m_goodShiftedSeparatrixSegments[DOWNWARDS][m_separatrixIndices[i]].begin();
//        m_segments[4 * i + 2] = sepSegmentDatabaseFromRP2.m_goodShiftedSeparatrixSegments[UPWARDS][sepSegmentDatabaseFromRP2.m_separatrixPair[m_separatrixIndices[i]]].begin();
//        m_segments[4 * i + 3] = sepSegmentDatabaseFromRP2.m_goodShiftedSeparatrixSegments[DOWNWARDS][sepSegmentDatabaseFromRP2.m_separatrixPair[m_separatrixIndices[i]]].begin();
//    }
//    // something comes here
//}



//void SepSegmentCollectionsFromRP2::advance() {

//}

