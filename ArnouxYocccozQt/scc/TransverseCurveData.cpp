#include "TransverseCurveData.h"
#include "TransverseCurve.h"
#include "SepSegmentDatabase.h"
#include "../fol/Foliation.h"
#include <cassert>



struct balazs::TransverseCurveData::Impl{
    Impl(const TransverseCurve& tcurve, SepSegmentDatabase& ssDatabase);


    void initIntervalsInOrder();
    const SeparatrixSegment& touchingSepSegment(const SepSegmentIndex& ssIndex);
    Mod1NumberIntExchange distanceOnCurve(const Mod1NumberIntExchange& x, const Mod1NumberIntExchange& y);


    const TransverseCurve& m_tc;
    SepSegmentDatabase& m_sepSegmentDatabase;
    std::vector<Mod1NumberIntExchange> m_intervalsInOrder;
    std::vector<Mod1NumberIntExchange> m_topDivPoints;
    std::vector<Mod1NumberIntExchange> m_bottomDivPoints;
    std::vector<Mod1NumberIntExchange> m_lengths;
    Permutation m_intervalPermutation;
    Mod1NumberIntExchange m_twist;
    Permutation m_singularityPermutation;
};



balazs::TransverseCurveData::Impl::Impl(const TransverseCurve& tcurve, SepSegmentDatabase& ssDatabase)
    : m_tc(tcurve), m_sepSegmentDatabase(ssDatabase)
{
    assert(&m_sepSegmentDatabase == &m_tc.sepSegmentCollection().sepSegmentDatabase());
    initIntervalsInOrder();


    // m_topDivPoints
    std::vector<Mod1NumberIntExchange> topDivPoints;
    topDivPoints.reserve(m_sepSegmentDatabase.foliation().numIntervals());

    const SeparatrixSegment& s = touchingSepSegment({HDirection::Right,
                                                      VDirection::Down, 0});
    Mod1NumberIntExchange referenceZero = s.endpoint().shiftedTo(HDirection::Center);
    Mod1NumberIntExchange p;
    for(std::size_t i = 0; i < m_sepSegmentDatabase.foliation().numIntervals(); i++){
        p = touchingSepSegment({HDirection::Right, VDirection::Down, i}).endpoint().shiftedTo(HDirection::Center);
        topDivPoints.push_back(distanceOnCurve(referenceZero, p));
    }
    m_topDivPoints = topDivPoints;
    std::sort(m_topDivPoints.begin(), m_topDivPoints.end());



    // m_singularityPermutation
    std::vector<std::size_t> permutationInput(m_sepSegmentDatabase.foliation().numIntervals());
    for(std::size_t i = 0; i < m_sepSegmentDatabase.foliation().numIntervals(); i++){
        auto it = std::lower_bound(m_topDivPoints.begin(), m_topDivPoints.end(), topDivPoints[i]);
        permutationInput[it - m_topDivPoints.begin()] = i;
    }

    m_singularityPermutation = Permutation(permutationInput);



    // m_lengths
    m_lengths.reserve(m_sepSegmentDatabase.foliation().numIntervals());  
    for(std::size_t i = 0; i < m_sepSegmentDatabase.foliation().numIntervals() - 1; i++){
        m_lengths.push_back(m_topDivPoints[i + 1] - m_topDivPoints[i]);
    }
    m_lengths.push_back(m_tc.disjointIntervals().totalLength() - m_topDivPoints.back());


    // m_bottomDivPoints
    std::vector<Mod1NumberIntExchange> bottomDivPoints;
    bottomDivPoints.reserve(m_sepSegmentDatabase.foliation().numIntervals());

    for(std::size_t i = 0; i < m_sepSegmentDatabase.foliation().numIntervals(); i++){
        p = touchingSepSegment({HDirection::Right, VDirection::Up, i}).endpoint().shiftedTo(HDirection::Center);
        bottomDivPoints.push_back(distanceOnCurve(referenceZero, p));
    }
    m_bottomDivPoints = bottomDivPoints;
    std::sort(m_bottomDivPoints.begin(), m_bottomDivPoints.end());



    // m_twist
    m_twist = m_bottomDivPoints[0];



    // m_intervalPermutation
    for(std::size_t i = 0; i < m_sepSegmentDatabase.foliation().numIntervals(); i++){
        permutationInput[i] = std::lower_bound(m_bottomDivPoints.begin(), m_bottomDivPoints.end(), bottomDivPoints[m_singularityPermutation[i]]) - m_bottomDivPoints.begin();
    }
    m_intervalPermutation = Permutation(permutationInput);

}






void balazs::TransverseCurveData::Impl::initIntervalsInOrder()
{
    std::vector<Mod1NumberIntExchange> collectionEndPoints = getEndpoints(m_tc.sepSegmentCollection());

    std::size_t disjointIntervalIndex = m_tc.disjointIntervals().wrapsAroundEnds() ?
                m_tc.disjointIntervals().endpoints().size() - 1 : 0;

    Mod1NumberIntExchange currentPoint;

    m_intervalsInOrder.reserve(m_tc.sepSegmentCollection().size());
    for(std::size_t i = 0; i < m_tc.sepSegmentCollection().size(); i++){
        currentPoint = m_tc.disjointIntervals().endpoints()[disjointIntervalIndex];
        m_intervalsInOrder.push_back(currentPoint);
        if(i % 2 == 0){
            disjointIntervalIndex = (disjointIntervalIndex + 1) % m_tc.disjointIntervals().endpoints().size();
        } else {
            auto it = std::find(collectionEndPoints.begin(), collectionEndPoints.end(), currentPoint);
            std::size_t index = it - collectionEndPoints.begin();
            assert(index < collectionEndPoints.size());
            index = (index % 2 == 0) ? index + 1 : index - 1;
            auto it2 = std::lower_bound(m_tc.disjointIntervals().endpoints().begin(),
                                          m_tc.disjointIntervals().endpoints().end(),
                                          collectionEndPoints[index]);
            disjointIntervalIndex = it2 - m_tc.disjointIntervals().endpoints().begin();
            assert(disjointIntervalIndex < m_tc.disjointIntervals().endpoints().size());
        }
    }

}






const balazs::SeparatrixSegment &balazs::TransverseCurveData::Impl::touchingSepSegment(const SepSegmentIndex &ssIndex)
{
    return m_sepSegmentDatabase.getFirstIntersection(ssIndex, m_tc.disjointIntervals());
}



balazs::Mod1NumberIntExchange balazs::TransverseCurveData::Impl::distanceOnCurve(const Mod1NumberIntExchange &x,
                                                                       const Mod1NumberIntExchange &y)
{
    assert(m_tc.disjointIntervals().contains(x) && m_tc.disjointIntervals().contains(y));
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





//===============================================//


balazs::TransverseCurveData::TransverseCurveData(const TransverseCurve& tc, SepSegmentDatabase& ssDatabase)
    : pimpl(new Impl(tc, ssDatabase))
{
}

balazs::TransverseCurveData::~TransverseCurveData()
{
}

balazs::TransverseCurveData::TransverseCurveData(balazs::TransverseCurveData &&other)
    : pimpl(std::move(other.pimpl))
{
}

const balazs::TransverseCurve &balazs::TransverseCurveData::transverseCurve() const
{
    return pimpl->m_tc;
}

const balazs::SeparatrixSegment &balazs::TransverseCurveData::touchingSepSegment(const balazs::SepSegmentIndex &ssIndex) const
{
    return pimpl->touchingSepSegment(ssIndex);
}

const std::vector<balazs::Mod1NumberIntExchange> &balazs::TransverseCurveData::lengths() const
{
    return pimpl->m_lengths;
}

const balazs::Permutation &balazs::TransverseCurveData::intervalPermutation() const
{
    return pimpl->m_intervalPermutation;
}

const balazs::Permutation &balazs::TransverseCurveData::singularityPermutation() const
{
    return pimpl->m_singularityPermutation;
}

const balazs::Mod1NumberIntExchange &balazs::TransverseCurveData::twist() const
{
    return pimpl->m_twist;
}

balazs::Mod1NumberIntExchange balazs::TransverseCurveData::length() const
{
    return pimpl->m_tc.disjointIntervals().totalLength();
}


