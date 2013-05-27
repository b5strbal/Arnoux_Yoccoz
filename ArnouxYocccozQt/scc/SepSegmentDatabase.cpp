#include "SepSegmentDatabase.h"
#include "DisjointIntervals.h"
#include "../fol/Foliation.h"
#include <cassert>

balazs::SepSegmentDatabase::SepSegmentDatabase(const Foliation& foliation) :
    m_foliation(foliation)
{
    for(HDirection hDirection : {HDirection::Left, HDirection::Right}){
        for(VDirection vDirection : {VDirection::Up, VDirection::Down}){
            m_currentSepSegments[hDirection][vDirection].reserve(foliation.numIntervals());
            m_goodShiftedSeparatrixSegments[hDirection][vDirection].resize(foliation.numIntervals());

            for (std::size_t i = 0; i < foliation.numIntervals(); i++) {
                m_currentSepSegments[hDirection][vDirection].push_back(SeparatrixSegment(foliation, {hDirection, vDirection, i}));
                m_goodShiftedSeparatrixSegments[hDirection][vDirection][i].push_back(m_currentSepSegments[hDirection][vDirection].back());
            }
        }
    }
}




std::list<balazs::SeparatrixSegment>::const_iterator balazs::SepSegmentDatabase::firstGoodSegment(const SepSegmentIndex& index) const
{
    return m_goodShiftedSeparatrixSegments.at(index.hDirection).at(index.vDirection)[index.singularityIndex].begin();
}



bool balazs::SepSegmentDatabase::isLast(std::list<SeparatrixSegment>::const_iterator it, std::size_t maxDepth) const
{
    assert(&foliation() == &it->foliation());
    std::list<SeparatrixSegment>::const_iterator nextIt = std::next(it);
    return (nextIt == m_goodShiftedSeparatrixSegments.at(it->hDirection()).at(it->vDirection())[it->startingSingularity()].end() ||
            nextIt->depth() > maxDepth);
}


const std::list<balazs::SeparatrixSegment> &balazs::SepSegmentDatabase::goodSegmentList(const balazs::SepSegmentIndex &index) const
{
    return m_goodShiftedSeparatrixSegments.at(index.hDirection).at(index.vDirection)[index.singularityIndex];
}











//bool balazs::SepSegmentDatabase::reachedSaddleConnection(const SepSegmentIndex& ssIndex) const
//{
//    return m_currentSepSegments.at(ssIndex.hDirection).at(ssIndex.vDirection)[ssIndex.singularityIndex].reachedSaddleConnection();
//}




//void balazs::SepSegmentDatabase::findNextSepSegment(const SepSegmentIndex& ssIndex){
//    SeparatrixSegment& s =
//            m_currentSepSegments[ssIndex.hDirection][ssIndex.vDirection][ssIndex.singularityIndex];
//    s.lengthen(); // might throw, strong guarantee

//    if (s.isGood()) {
//        m_goodShiftedSeparatrixSegments[s.hDirection()][s.vDirection()][s.startingSingularity()].push_back(s);
//    }
//}


void balazs::SepSegmentDatabase::findNextGoodSegment(const balazs::SepSegmentIndex &ssIndex) // strong guarantee
{
    while(true){
        SeparatrixSegment& s =
                m_currentSepSegments[ssIndex.hDirection][ssIndex.vDirection][ssIndex.singularityIndex];
        s.lengthen(); // might throw, strong guarantee

        if (s.isGood()) {
            m_goodShiftedSeparatrixSegments[s.hDirection()][s.vDirection()][s.startingSingularity()].push_back(s);
            return;
        }
    }
}




void balazs::SepSegmentDatabase::generateSepSegments(std::size_t maxdepth){ // doesn't throw
    for(HDirection hDirection : {HDirection::Left, HDirection::Right}){
        for (VDirection vDirection : {VDirection::Up, VDirection::Down}) {
            for (std::size_t index = 0; index < m_foliation.numIntervals(); index++) {
                while (m_currentSepSegments[hDirection][vDirection][index].depth() < maxdepth) {
                    try{
                        findNextGoodSegment({hDirection, vDirection, index});
                    } catch(const std::exception&){
                        // even if the sepSegments cannot be generated until the required depth because of
                        // saddle connection, the function doesn't throw, just silently ignores it
                        break;
                    }
                }
            }
        }
    }
}

namespace balazs{

bool touches(const SeparatrixSegment& s, const DisjointIntervals& intervals, SepSegmentDatabase::ShiftMode shiftMode)
{
    Mod1Number correctedEndpoint;
    switch(shiftMode){
    case SepSegmentDatabase::Centered:
        correctedEndpoint = s.endpoint().shiftedTo(HDirection::Center);
        break;

    case SepSegmentDatabase::ShiftedEvenMore:
        correctedEndpoint = s.endpoint().number().shiftEvenMore();
        break;
    }

    if (intervals.strictlyContains(correctedEndpoint)) {
        return true;
    }
    return false;
}

}


// For arbitrary intervals this function finds the first good separatrix segment that intersects the intervals.
// However, in the appliations, we use a special collection of intervals (a subset of an IntervalNeighborhoods),
// where the first intersection is always a good separatrix segment. More precisely, both the left and right
// shifts of the first intersection are good, so it is enough to check only the right ones for instance.
const balazs::SeparatrixSegment& balazs::SepSegmentDatabase::getFirstIntersection(const SepSegmentIndex& ssIndex,
                                                                    const DisjointIntervals& intervals,
                                                                                  ShiftMode shiftMode)
// strong guarantee
{
    for (const auto &segment :
         m_goodShiftedSeparatrixSegments[ssIndex.hDirection][ssIndex.vDirection][ssIndex.singularityIndex]) {

        if(touches(segment, intervals, shiftMode)){
            return segment;
        }
    }

    while(true){
        findNextGoodSegment(ssIndex);
        const auto &segment = m_goodShiftedSeparatrixSegments[ssIndex.hDirection][ssIndex.vDirection][ssIndex.singularityIndex].back();
        if(touches(segment, intervals, shiftMode)){
            return segment;
        }
    }

}






