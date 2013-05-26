#include "TransverseCurve.h"
#include "SepSegmentDatabase.h"
#include "SmallFoliation.h"
#include "../fol/Foliation.h"
#include <cassert>
#include <QDebug>


balazs::TransverseCurve::TransverseCurve(const SepSegmentCollection &segments, bool wrapsAroundEnds, SepSegmentDatabase &ssDatabase) :
    m_sepSegmentCollection(segments),
    m_disjointIntervals(getEndpoints(m_sepSegmentCollection), wrapsAroundEnds),
    m_sepSegmentDatabase(ssDatabase)
{
    assert(&m_sepSegmentDatabase == &m_sepSegmentCollection.sepSegmentDatabase());

    initIntervalsInOrder();
    initTouchingSegments();

    m_topIntersections.reserve(foliation().numIntervals());
    for(std::size_t i = 0; i < foliation().numIntervals(); i++){
        m_topIntersections.push_back(touchingSepSegment({HDirection::Right, VDirection::Down, i},
                                                        SepSegmentDatabase::Centered).endpoint().shiftedTo(HDirection::Center));
    }

    m_bottomRightIntersections.reserve(foliation().numIntervals());
    for(std::size_t i = 0; i < foliation().numIntervals(); i++){
        m_bottomRightIntersections.push_back(touchingSepSegment({HDirection::Right, VDirection::Up, i},
                                                        SepSegmentDatabase::Centered).endpoint().shiftedTo(HDirection::Center));
    }

    m_bottomLeftIntersections.reserve(foliation().numIntervals());
    for(std::size_t i = 0; i < foliation().numIntervals(); i++){
        m_bottomLeftIntersections.push_back(touchingSepSegment({HDirection::Left, VDirection::Up, i},
                                                               SepSegmentDatabase::Centered).endpoint().shiftedTo(HDirection::Center));
    }
}




void balazs::TransverseCurve::initIntervalsInOrder()
{
    std::vector<Mod1NumberIntExchange> collectionEndPoints = getEndpoints(m_sepSegmentCollection);

    std::size_t disjointIntervalIndex = m_disjointIntervals.wrapsAroundEnds() ?
                m_disjointIntervals.endpoints().size() - 1 : 0;

    Mod1NumberIntExchange currentPoint;

    m_intervalsInOrder.reserve(m_sepSegmentCollection.size());
    for(std::size_t i = 0; i < m_sepSegmentCollection.size(); i++){
        currentPoint = m_disjointIntervals.endpoints()[disjointIntervalIndex];
        m_intervalsInOrder.push_back(currentPoint);
        if(i % 2 == 0){
            disjointIntervalIndex = (disjointIntervalIndex + 1) % m_disjointIntervals.endpoints().size();
        } else {
            auto it = std::find(collectionEndPoints.begin(), collectionEndPoints.end(), currentPoint);
            std::size_t index = it - collectionEndPoints.begin();
            assert(index < collectionEndPoints.size());
            index = (index % 2 == 0) ? index + 1 : index - 1;
            auto it2 = std::lower_bound(m_disjointIntervals.endpoints().begin(),
                                          m_disjointIntervals.endpoints().end(),
                                          collectionEndPoints[index]);
            disjointIntervalIndex = it2 - m_disjointIntervals.endpoints().begin();
            assert(disjointIntervalIndex < m_disjointIntervals.endpoints().size());
        }
    }

}


void balazs::TransverseCurve::initTouchingSegments()
{
    for(HDirection hDirection : {HDirection::Left, HDirection::Right}){
        for(VDirection vDirection : {VDirection::Down, VDirection::Up }){
            for(SepSegmentDatabase::ShiftMode shiftmode :
            {SepSegmentDatabase::Centered, SepSegmentDatabase::ShiftedEvenMore}){
                for(std::size_t i = 0; i < foliation().numIntervals(); i++){ // catch something!!
                    m_touchingSegments[hDirection][vDirection][shiftmode].push_back(
                                &m_sepSegmentDatabase.getFirstIntersection({hDirection, vDirection, i},
                                                        m_disjointIntervals, shiftmode));
                    if((double)m_disjointIntervals.totalLength() > 0.125 &&
                           (double)m_disjointIntervals.totalLength() < 0.126){
                        qDebug() << "futyi";
                    }
                }

            }
        }
    }
}



const balazs::SeparatrixSegment &balazs::TransverseCurve::touchingSepSegment(const SepSegmentIndex &ssIndex,
                                                                             SepSegmentDatabase::ShiftMode shiftMode) const
{
    return *m_touchingSegments.at(ssIndex.hDirection).at(ssIndex.vDirection).at(shiftMode)[ssIndex.singularityIndex];
}




balazs::Mod1NumberIntExchange balazs::TransverseCurve::distanceOnCurve(const Mod1NumberIntExchange &x,
                                                                       const Mod1NumberIntExchange &y) const
{
    assert(m_disjointIntervals.strictlyContains(x) && m_disjointIntervals.strictlyContains(y));
    std::size_t index;
    std::size_t nextIndex;
    for(index = 0; index < m_intervalsInOrder.size(); index += 2){
        nextIndex = (index + 1) % m_intervalsInOrder.size();
        if(isBetween(m_intervalsInOrder[index], m_intervalsInOrder[nextIndex], x)) break;
    }
    if(isBetween(x, m_intervalsInOrder[nextIndex], y)){
        return y-x;
    }

    Mod1NumberIntExchange retval = m_intervalsInOrder[nextIndex] - x;
    index = (index + 2) % m_intervalsInOrder.size();
    nextIndex = (index + 1) % m_intervalsInOrder.size();
    while(!isBetween(m_intervalsInOrder[index], m_intervalsInOrder[nextIndex], y)){
        retval += m_intervalsInOrder[nextIndex] - m_intervalsInOrder[index];
        index = (index + 2) % m_intervalsInOrder.size();
        nextIndex = (index + 1) % m_intervalsInOrder.size();
    }
    return retval + (y - m_intervalsInOrder[index]);
}








bool balazs::operator<(const TransverseCurve& c1, const TransverseCurve& c2) {
    if (c1.disjointIntervals().totalLength() > c2.disjointIntervals().totalLength()) return true;
    if (c1.disjointIntervals().totalLength() < c2.disjointIntervals().totalLength()) return false;
    auto &points1 = c1.m_disjointIntervals.endpoints();
    auto &points2 = c2.m_disjointIntervals.endpoints();
    if (points1.size() < points2.size()) {
        return true;
    } else if (points1.size() > points2.size())
        return false;
    for (std::size_t i = 0; i < points1.size(); i++) {
        if (points1[i] < points2[i]) {
            return true;
        } else if (points2[i] < points1[i])
            return false;
    }
    return false;
}




bool balazs::transverseCurve_compare::operator()(const TransverseCurve* cp1, const TransverseCurve* cp2)
{
    return *cp1 < *cp2;
}


const balazs::Foliation &balazs::TransverseCurve::foliation() const
{
    return m_sepSegmentCollection.foliation();
}
