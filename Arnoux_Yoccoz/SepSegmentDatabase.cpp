#include "SepSegmentDatabase.h"

SepSegmentDatabase::SepSegmentDatabase(const Foliation& foliation) :
    m_foliation(foliation)
{
    m_currentSepSegments[Direction::UP].reserve(foliation.numIntervals());
    m_currentSepSegments[Direction::DOWN].reserve(foliation.numIntervals());
    m_goodShiftedSeparatrixSegments[Direction::UP].resize(foliation.numIntervals());
    m_goodShiftedSeparatrixSegments[Direction::DOWN].resize(foliation.numIntervals());
    for (unsigned int i = 0; i < foliation.numIntervals(); i++) {
        m_currentSepSegments[Direction::DOWN].emplace_back(foliation, i, Direction::DOWN);
        m_currentSepSegments[Direction::UP].emplace_back(foliation, i, Direction::UP);
        m_goodShiftedSeparatrixSegments[Direction::DOWN][i].push_back(m_currentSepSegments[Direction::DOWN].back());
        m_goodShiftedSeparatrixSegments[Direction::UP][i].push_back(m_currentSepSegments[Direction::UP].back());
    }
}


void SepSegmentDatabase::printGoodSepSegments(int maxdepth, bool verbose){
    if (maxdepth > 0) {
        generateSepSegments(maxdepth);
    } else
        maxdepth = INT_MAX;
    for (Direction::UpOrDown direction = Direction::UP; direction <= Direction::DOWN; direction++) {
        for (unsigned int i = 0; i < m_foliation.numIntervals(); i++) {
            std::cout << i << (direction == Direction::UP ? " UP" : " DOWN") << "\n";
            for(auto it = m_goodShiftedSeparatrixSegments[direction][i].begin(); it !=
                m_goodShiftedSeparatrixSegments[direction][i].end() && it->depth() < maxdepth; it++)
            {
                if (verbose) {
                    std::cout << *it << "\n\n";
                } else
                    std::cout << it->depth() << " ";
            }
            if (!verbose) {
                std::cout << "\n\n";
            }
        }
    }
}






bool SepSegmentDatabase::reachedSaddleConnection(Direction::UpOrDown direction, int index) const
{
    return m_currentSepSegments[direction][index].reachedSaddleConnection();
}



void SepSegmentDatabase::findNextSepSegment(Direction::UpOrDown direction, int index){
    assert(!reachedSaddleConnection(direction, index));

    SeparatrixSegment& segment = m_currentSepSegments[direction][index];
    segment.lengthen();

    if (!segment.reachedSaddleConnection() && segment.isGood()) {
        m_goodShiftedSeparatrixSegments[direction][index].push_back(segment);
    }
}




void SepSegmentDatabase::generateSepSegments(int maxdepth){
    for (Direction::UpOrDown direction = Direction::UP; direction <= Direction::DOWN; direction++) {
        for (unsigned int index = 0; index < m_foliation.numIntervals(); index++) {
            while (!reachedSaddleConnection(direction, index) &&
                   m_currentSepSegments[direction][index].depth() < maxdepth) {
                findNextSepSegment(direction, index);
            }
    //        std::cout << std::endl << std::endl;
        }
    }
}



// For arbitrary intervals this function finds the first good separatrix segment that intersects the intervals.
// However, in the appliations, we use a special collection of intervals, in which case the first intersection is always a good one.
const SeparatrixSegment& SepSegmentDatabase::getFirstIntersection(Direction::UpOrDown direction,
                                                                    int index,
                                                                    const DisjointIntervals& intervals)
{
    for (auto &segment : m_goodShiftedSeparatrixSegments[direction][index]) {
        if (intervals.contains(segment.endpoint())) { // we are gonna have to catch an error here
            return segment;
        }
    }
    while (!intervals.contains(m_currentSepSegments[direction][index].endpoint())) {
        if (reachedSaddleConnection(direction, index)) {
            throw std::runtime_error("getFirstIntersection: First intersection cannot be found, because we found a saddle connection.");
        }
        findNextSepSegment(direction, index);
    }
    return m_currentSepSegments[direction][index];
}






