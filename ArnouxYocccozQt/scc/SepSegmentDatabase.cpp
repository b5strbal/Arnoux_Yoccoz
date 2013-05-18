#include "SepSegmentDatabase.h"
#include "DisjointIntervals.h"
#include "../fol/Foliation.h"
#include <cassert>

balazs::SepSegmentDatabase::SepSegmentDatabase(const Foliation& foliation) :
    m_foliation(foliation)
{
    for(VDirection vDirection : {VDirection::Up, VDirection::Down}){
        m_currentSepSegments[vDirection].reserve(foliation.numIntervals());

        for(HDirection hDirection : {HDirection::Left, HDirection::Right}){
            m_goodShiftedSeparatrixSegments[hDirection][vDirection].resize(foliation.numIntervals());
        }

        for (std::size_t i = 0; i < foliation.numIntervals(); i++) {
            m_currentSepSegments[vDirection].push_back(SeparatrixSegment(foliation, i, vDirection));
            addToGoodSegmentsIfGood(m_currentSepSegments[vDirection].back());
        }
    }
}





std::size_t balazs::SepSegmentDatabase::numIntervals() const
{
    return m_foliation.numIntervals();
}




std::list<balazs::SeparatrixSegment>::const_iterator balazs::SepSegmentDatabase::firstGoodSegment(const SepSegmentIndex& index) const
{
    return m_goodShiftedSeparatrixSegments.at(index.hDirection).at(index.vDirection)[index.singularityIntex].begin();
}



bool balazs::SepSegmentDatabase::isLast(std::list<SeparatrixSegment>::const_iterator it, std::size_t maxDepth) const
{
    assert(&foliation() == &it->foliation());
    it++;
    return (it == m_goodShiftedSeparatrixSegments.at(it->side()).at(it->vDirection())[it->startingSingularity()].end() ||
            it->depth() > maxDepth);
}

const std::list<balazs::SeparatrixSegment> &balazs::SepSegmentDatabase::goodSegmentList(const balazs::SepSegmentIndex &index) const
{
    return m_goodShiftedSeparatrixSegments.at(index.hDirection).at(index.vDirection)[index.singularityIntex];
}




void balazs::SepSegmentDatabase::addToGoodSegmentsIfGood(const SeparatrixSegment &s)
{
    if (!s.reachedSaddleConnection() && s.isGood()) {
        for(HDirection hDirection : {HDirection::Left, HDirection::Right}){
            m_goodShiftedSeparatrixSegments[hDirection][s.vDirection()][s.startingSingularity()].push_back(s);
            m_goodShiftedSeparatrixSegments[hDirection][s.vDirection()][s.startingSingularity()].back().shiftTo(hDirection);
            if(!m_goodShiftedSeparatrixSegments[hDirection][s.vDirection()][s.startingSingularity()].back().isGood()){
                m_goodShiftedSeparatrixSegments[hDirection][s.vDirection()][s.startingSingularity()].pop_back();
            }
        }
    }
}






bool balazs::SepSegmentDatabase::reachedSaddleConnection(VDirection vDirection, int index) const
{
    return m_currentSepSegments.at(vDirection)[index].reachedSaddleConnection();
}



void balazs::SepSegmentDatabase::findNextSepSegment(VDirection direction, int index){
    assert(!reachedSaddleConnection(direction, index));

    SeparatrixSegment& segment = m_currentSepSegments[direction][index];
    segment.lengthen();

    addToGoodSegmentsIfGood(segment);
}




void balazs::SepSegmentDatabase::generateSepSegments(std::size_t maxdepth){
    for (VDirection vDirection : {VDirection::Up, VDirection::Down}) {
        for (std::size_t index = 0; index < m_foliation.numIntervals(); index++) {
            while (!reachedSaddleConnection(vDirection, index) &&
                   m_currentSepSegments[vDirection][index].depth() < maxdepth) {
                findNextSepSegment(vDirection, index);
            }
        }
    }
}



// For arbitrary intervals this function finds the first good separatrix segment that intersects the intervals.
// However, in the appliations, we use a special collection of intervals (a subset of an IntervalNeighborhoods),
// where the first intersection is always a good separatrix segment. More precisely, both the left and right
// shifts of the first intersection are good, so it is enough to check only the right ones for instance.
const balazs::SeparatrixSegment& balazs::SepSegmentDatabase::getFirstIntersection(VDirection direction,
                                                                    int index,
                                                                    const DisjointIntervals& intervals)
{
    for (auto &segment : m_goodShiftedSeparatrixSegments[HDirection::Right][direction][index]) {
        Mod1Number centeredEndpoint = segment.endpoint().shiftedTo(HDirection::Center);
        if (intervals.contains(centeredEndpoint)) { // we are gonna have to catch an error here?
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






