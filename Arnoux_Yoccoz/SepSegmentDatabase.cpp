#include "SepSegmentDatabase.h"

SepSegmentDatabase::SepSegmentDatabase(const Foliation& foliation) :
    m_foliation(foliation)
{
    m_currentSepSegments[UPWARDS].reserve(foliation.m_numIntervals);
    m_currentSepSegments[DOWNWARDS].reserve(foliation.m_numIntervals);
    m_goodShiftedSeparatrixSegments[UPWARDS].resize(foliation.m_numIntervals);
    m_goodShiftedSeparatrixSegments[DOWNWARDS].resize(foliation.m_numIntervals);
    for (int i = 0; i < foliation.m_numIntervals; i++) {
        m_currentSepSegments[DOWNWARDS].emplace_back(foliation, i, DOWNWARDS);
        m_currentSepSegments[UPWARDS].emplace_back(foliation, i, UPWARDS);
        m_goodShiftedSeparatrixSegments[DOWNWARDS][i].push_back(m_currentSepSegments[DOWNWARDS].back());
        m_goodShiftedSeparatrixSegments[UPWARDS][i].push_back(m_currentSepSegments[UPWARDS].back());
    }
}


void SepSegmentDatabase::printGoodSepSegments(int maxdepth, bool verbose){
    if (maxdepth > 0) {
        generateSepSegments(maxdepth);
    } else
        maxdepth = INT_MAX;
    for (UpDownDirection direction = UpDownDirection::UPWARDS; direction <= UpDownDirection::DOWNWARDS; direction++) {
        for (int i = 0; i < m_foliation.m_numIntervals; i++) {
            std::cout << i << (direction == UPWARDS ? " UP" : " DOWN") << "\n";
            for(auto it = m_goodShiftedSeparatrixSegments[direction][i].begin(); it !=
                m_goodShiftedSeparatrixSegments[direction][i].end() && it->m_depth < maxdepth; it++)
            {
                if (verbose) {
                    std::cout << it->print() << "\n\n";
                } else
                    std::cout << it->m_depth << " ";
            }
            if (!verbose) {
                std::cout << "\n\n";
            }
        }
    }
}






bool SepSegmentDatabase::reachedSaddleConnection(UpDownDirection direction, int index) const
{
    return m_currentSepSegments[direction][index].reachedSaddleConnection();
}



void SepSegmentDatabase::findNextSepSegment(UpDownDirection direction, int index){
    assert(!reachedSaddleConnection(direction, index));

    SeparatrixSegment& segment = m_currentSepSegments[direction][index];
    segment.lengthen();

    if (!segment.reachedSaddleConnection() && segment.isGood()) {
        m_goodShiftedSeparatrixSegments[direction][index].push_back(segment);
    }
}




void SepSegmentDatabase::generateSepSegments(int maxdepth){
    for (UpDownDirection direction = UpDownDirection::FIRST; direction <= UpDownDirection::LAST; direction++) {
        for (int index = 0; index < m_foliation.m_numIntervals; index++) {
            while (!reachedSaddleConnection(direction, index) &&
                   m_currentSepSegments[direction][index].m_depth < maxdepth) {
                findNextSepSegment(direction, index);
            }
    //        std::cout << std::endl << std::endl;
        }
    }
}



// For arbitrary intervals this function finds the first good separatrix segment that intersects the intervals.
// However, in the appliations, we use a special collection of intervals, in which case the first intersection is always a good one.
const SeparatrixSegment& SepSegmentDatabase::getFirstIntersection(UpDownDirection direction,
                                                                    int index,
                                                                    const DisjointIntervals& intervals)
{
    for (auto &segment : m_goodShiftedSeparatrixSegments[direction][index]) {
        if (intervals.containsQ(segment.m_endpoint)) { // we are gonna have to catch an error here
            return segment;
        }
    }
    while (!intervals.containsQ(m_currentSepSegments[direction][index].m_endpoint)) {
        if (reachedSaddleConnection(direction, index)) {
            throw std::runtime_error("getFirstIntersection: First intersection cannot be found, because we found a saddle connection.");
        }
        findNextSepSegment(direction, index);
    }
    return m_currentSepSegments[direction][index];
}






