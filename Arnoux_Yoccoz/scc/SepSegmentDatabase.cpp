#include "SepSegmentDatabase.h"

balazs::SepSegmentDatabase::SepSegmentDatabase(const Foliation& foliation) :
    m_foliation(foliation)
{
    for(Direction::UpOrDown updown = Direction::UP; updown <= Direction::DOWN; updown++){
        m_currentSepSegments[updown].reserve(foliation.numIntervals());

        for(Direction::LeftOrRight leftright = Direction::LEFT; leftright <= Direction::RIGHT; leftright++){
            m_goodShiftedSeparatrixSegments[leftright][updown].resize(foliation.numIntervals());
        }

        for (unsigned int i = 0; i < foliation.numIntervals(); i++) {
            m_currentSepSegments[updown].emplace_back(foliation, i, updown);
            addToGoodSegmentsIfGood(m_currentSepSegments[updown].back());
        }
    }
}


void balazs::SepSegmentDatabase::printGoodSepSegments(unsigned int maxdepth, bool verbose){
    if (maxdepth > 0) {
        generateSepSegments(maxdepth);
    } else
        maxdepth = INT_MAX;
    for (Direction::UpOrDown direction = Direction::UP; direction <= Direction::DOWN; direction++) {
        for (unsigned int i = 0; i < m_foliation.numIntervals(); i++) {
            std::cout << i << (direction == Direction::UP ? " UP" : " DOWN") << "\n";
            for(auto it = m_goodShiftedSeparatrixSegments[Direction::RIGHT][direction][i].begin(); it !=
                m_goodShiftedSeparatrixSegments[Direction::RIGHT][direction][i].end() && it->depth() < maxdepth; it++)
            {
                if (verbose) {                  // only prints segments shifted to right
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




std::list<balazs::SeparatrixSegment>::const_iterator balazs::SepSegmentDatabase::firstGoodSegment(std::list<SeparatrixSegment>::const_iterator it) const
{
    assert(&foliation() == &it->foliation());
    return firstGoodSegment(it->side(), it->direction(), it->startingSingularity());
}




std::list<balazs::SeparatrixSegment>::const_iterator balazs::SepSegmentDatabase::firstGoodSegment(Direction::LeftOrRight leftOrRight, Direction::UpOrDown upOrDown, unsigned int singularityIndex) const
{
    return m_goodShiftedSeparatrixSegments[leftOrRight][upOrDown][singularityIndex].begin();
}



bool balazs::SepSegmentDatabase::isLast(std::list<SeparatrixSegment>::const_iterator it, unsigned int maxDepth) const
{
    assert(&foliation() == &it->foliation());
    it++;
    return (it == m_goodShiftedSeparatrixSegments[it->side()][it->direction()][it->startingSingularity()].end() ||
    it->depth() > maxDepth);
}




void balazs::SepSegmentDatabase::addToGoodSegmentsIfGood(const SeparatrixSegment &s)
{
    if (!s.reachedSaddleConnection() && s.isGood()) {
        for(Direction::LeftOrRight leftright = Direction::LEFT; leftright <= Direction::RIGHT; leftright++){
            m_goodShiftedSeparatrixSegments[leftright][s.direction()][s.startingSingularity()].push_back(s);
            m_goodShiftedSeparatrixSegments[leftright][s.direction()][s.startingSingularity()].back().shiftTo(leftright);
            if(!m_goodShiftedSeparatrixSegments[leftright][s.direction()][s.startingSingularity()].back().isGood()){
                m_goodShiftedSeparatrixSegments[leftright][s.direction()][s.startingSingularity()].pop_back();
            }
        }
    }
}






bool balazs::SepSegmentDatabase::reachedSaddleConnection(Direction::UpOrDown direction, int index) const
{
    return m_currentSepSegments[direction][index].reachedSaddleConnection();
}



void balazs::SepSegmentDatabase::findNextSepSegment(Direction::UpOrDown direction, int index){
    assert(!reachedSaddleConnection(direction, index));

    SeparatrixSegment& segment = m_currentSepSegments[direction][index];
    segment.lengthen();

    addToGoodSegmentsIfGood(segment);

}




void balazs::SepSegmentDatabase::generateSepSegments(unsigned int maxdepth){
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
// However, in the appliations, we use a special collection of intervals (a subset of an IntervalNeighborhoods),
// where the first intersection is always a good separatrix segment. More precisely, both the left and right
// shifts of the first intersection are good, so it is enough to check only the right ones for instance.
const balazs::SeparatrixSegment& balazs::SepSegmentDatabase::getFirstIntersection(Direction::UpOrDown direction,
                                                                    int index,
                                                                    const DisjointIntervals& intervals)
{
    for (auto &segment : m_goodShiftedSeparatrixSegments[Direction::RIGHT][direction][index]) {
        Mod1Number centeredEndpoint = segment.endpoint().shiftedTo(Direction::CENTER);
        if (intervals.contains(centeredEndpoint)) { // we are gonna have to catch an error here
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






