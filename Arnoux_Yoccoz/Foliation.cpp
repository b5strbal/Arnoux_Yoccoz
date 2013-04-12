/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains implementation for the Foliation class.
 */

#include "Foliation.h"


//------------------------------//
// Foliation::DisjointIntervals //
//------------------------------//



Foliation::DisjointIntervals::DisjointIntervals(const std::vector<UnitIntervalPoint>& unsortedPoints, bool wrapsAroundZero) :
m_points(unsortedPoints),
m_wrapsAroundZero(wrapsAroundZero),
m_totalLength(0)
{
    assert(unsortedPoints.size() % 2 == 0);
    std::sort(m_points.begin(), m_points.end());
    for (int i = 0; i < m_points.size(); i += 2) {
        m_totalLength += distanceBetween(m_points[i], m_points[i + 1]);
    }
    m_totalLength = wrapsAroundZero ? 1 - m_totalLength : m_totalLength;
}


bool Foliation::DisjointIntervals::containsQ(const UnitIntervalPoint& point) const {
    int containingIntervalIndex = containingInterval(m_points, point);
    if (containingIntervalIndex == CONTAINING_INTERVAL_NOT_UNIQUE) {
        throw std::runtime_error("DisjointIntervals::containsQ : Containing interval is not unique.");
    }
    if ((containingIntervalIndex % 2 == 0 && !m_wrapsAroundZero) || (containingIntervalIndex % 2 == 1 && m_wrapsAroundZero)) {
        return true;
    }
    return false;
}





std::string Foliation::DisjointIntervals::print() const{
    std::ostringstream s;
    for (int i = m_wrapsAroundZero ? 1 : 0; i < m_points.size() - 2; i += 2) {
        s << printInterval(m_points[i], m_points[i + 1]) << " ";
    }
    if (m_wrapsAroundZero) {
        s << printInterval(m_points.back(), m_points.front());
    } else
        s << printInterval(m_points[m_points.size() - 2], m_points.back());
    return s.str();
}








//-------------------------------//
// Foliation::ArcAroundDivPoints //
//-------------------------------//

void Foliation::ArcsAroundDivPoints::InsertPoint(const UnitIntervalPoint& NewCuttingPoint, int IndexOfInterval){
    if (m_isIntervalEmpty[IndexOfInterval]) {
        m_firstCuttingPoint[IndexOfInterval] = m_secondCuttingPoint[IndexOfInterval] = NewCuttingPoint;
        m_isIntervalEmpty[IndexOfInterval] = false;
    } else if (NewCuttingPoint < m_firstCuttingPoint[IndexOfInterval])
        m_firstCuttingPoint[IndexOfInterval] = NewCuttingPoint;
    else if (m_secondCuttingPoint[IndexOfInterval] < NewCuttingPoint)
        m_secondCuttingPoint[IndexOfInterval] = NewCuttingPoint;
}





bool Foliation::ArcsAroundDivPoints::ContainsQ(const UnitIntervalPoint& c, int IndexOfInterval) const{
    if (m_isIntervalEmpty[IndexOfInterval]) {
        return true;
    }
    if (c < m_firstCuttingPoint[IndexOfInterval] || m_secondCuttingPoint[IndexOfInterval] < c)
    {
        return true;
    }
    return false;
}






bool Foliation::ArcsAroundDivPoints::ContainsArcThroughADivPointQ(const UnitIntervalPoint& LeftEndPoint, int LeftIndexOfInterval,
                                                                  const UnitIntervalPoint& RightEndPoint, int RightIndexOfInterval,
                                                                  bool throughTopDivPointQ) const{
    
    if (LeftIndexOfInterval == RightIndexOfInterval) {
        return false;
    }
    if (!m_isIntervalEmpty[LeftIndexOfInterval] && LeftEndPoint < m_secondCuttingPoint[LeftIndexOfInterval]) {
        return false;
    }
    if (!m_isIntervalEmpty[RightIndexOfInterval] && m_firstCuttingPoint[RightIndexOfInterval] < RightEndPoint) {
        return false;
    }
    
    for (Modint i(LeftIndexOfInterval + 1, m_foliation.m_numSeparatrices); i != RightIndexOfInterval; ++i) {
        if (!m_isIntervalEmpty[i]) {
            return false;
        }
    }
        
    for (Modint i(LeftIndexOfInterval, m_foliation.m_numSeparatrices); i != RightIndexOfInterval; ++i) {
        if ((throughTopDivPointQ && m_foliation.m_isTopDivPoint[i + 1]) ||
            (!throughTopDivPointQ && !m_foliation.m_isTopDivPoint[i + 1])) {
            return true;
        }
    }

    return false;
}



std::string Foliation::ArcsAroundDivPoints::print() const
{
    std::ostringstream s;
    for (int i = 0; i < m_isIntervalEmpty.size(); i++){
        if (m_isIntervalEmpty[i]) {
            s << "() ";
        } else
            s << "(" << m_firstCuttingPoint[i] << "," << m_secondCuttingPoint[i] << ") ";
    }
    return s.str();
}







/**
 * @details The intersection of the two Arcs corresponding to each division point is taken and that is assigned to the division point
 *          it the intersection. The operation is well-defined, because although in general the intersection of two arcs on the circle
 *          might not be a single arc, in our case it can't happen because of the properties the Arcs in an ArcAroundDivPoints object
 *          have to satisfy.
 *
 *          The motivation for this operation is to take the union of two sets of cutting points. A set of cutting points determines
 *          an ArcsAroundDivPoints, another set determines another one, and the union of the two sets determines exactly the
 *          intersection of the two objects.
 */
Foliation::ArcsAroundDivPoints Foliation::intersect(const std::vector<const ArcsAroundDivPoints*>& adpVector) const
{
    assert(adpVector.size() >= 2);
    ArcsAroundDivPoints adp = *adpVector[0];
    for (int i = 0; i < m_numSeparatrices; i++) {
        for (auto it = adpVector.begin() + 1; it != adpVector.end(); it++) {
            if ((*it)->m_isIntervalEmpty[i]) {
            } else
            if (adp.m_isIntervalEmpty[i]) {
                adp.m_firstCuttingPoint[i] = (*it)->m_firstCuttingPoint[i];
                adp.m_secondCuttingPoint[i] = (*it)->m_secondCuttingPoint[i];
                adp.m_isIntervalEmpty[i] = false;
            } else {
                adp.m_firstCuttingPoint[i] = std::min(adp.m_firstCuttingPoint[i], (*it)->m_firstCuttingPoint[i]);
                adp.m_secondCuttingPoint[i] = std::max(adp.m_secondCuttingPoint[i], (*it)->m_secondCuttingPoint[i]);
            }
        }
    }
    return adp;
}








Foliation::SeparatrixSegment::SeparatrixSegment(const Foliation& foliation, int startingSingularity, Direction direction) :
    m_foliation(foliation),
    m_startingSingularity(startingSingularity),
    m_direction(direction),
    m_depth(1),
    m_intervalIntersectionCount(std::vector<int>(foliation.m_numIntervals, 0)),
    m_arcsAroundDivPoints(foliation)
{
    if (direction == UPWARDS) {
        m_endpoint = UnitIntervalPoint( foliation.m_bottomRealDivPoints[foliation.m_pairOfTopDivPoints[startingSingularity]].getPosition(), 1);
    } else
        m_endpoint = UnitIntervalPoint( foliation.m_topRealDivPoints[startingSingularity].getPosition(), 1);
    
    m_smallContainingInterval = containingInterval(foliation.m_allRealDivPoints, m_endpoint);
}






std::string Foliation::SeparatrixSegment::print(bool verbose) const
{
    std::ostringstream s;
    s << "SEPARATRIX SEGMENT\n";
    s << "(" << m_startingSingularity << ", ";
    s << m_depth << ", ";
    s << (m_direction == Foliation::DOWNWARDS ? "down" : "up") << ", ";
    s << m_endpoint << ")\n";
    if (verbose) {
        s << "IIC: " << m_intervalIntersectionCount << "\n";
        s << "AAD: " << m_arcsAroundDivPoints.print();
    }

    return s.str();
}






//----------------------------//
// Foliation::TransverseCurve //
//----------------------------//



Foliation::TransverseCurve::TransverseCurve(const Foliation& foliation, const std::vector<std::list<SeparatrixSegment>::iterator>& goodSegmentIndices, bool wrapsAroundZero) :
    m_foliation(foliation)
{
    std::vector<UnitIntervalPoint> endpoints;
    endpoints.reserve(goodSegmentIndices.size());
    for ( auto it : goodSegmentIndices)
        endpoints.push_back(it->m_endpoint);
    
    m_disjointIntervals = DisjointIntervals(endpoints, wrapsAroundZero);
    m_goodSegmentIndices = goodSegmentIndices; // We will probably have to find a more cleverly sorted way of storing the segments later.
   // std::cout << print() << "\n\n";
}



bool operator<(const Foliation::TransverseCurve& c1, const Foliation::TransverseCurve& c2) {
    if (c1.length() > c2.length()) return true;
    if (c1.length() < c2.length()) return false;
    auto &points1 = c1.m_disjointIntervals.points();
    auto &points2 = c2.m_disjointIntervals.points();
    if (points1.size() < points2.size()) {
        return true;
    } else if (points1.size() > points2.size())
        return false;
    for (int i = 0; i < points1.size(); i++) {
        if (points1[i] < points2[i]) {
            return true;
        } else if (points2[i] < points1[i])
            return false;
    }
    return true;
}




std::string Foliation::TransverseCurve::print() const
{
    std::ostringstream s;
    s << "Separatrix segments:\n";
    for (auto goodSegmentIndex : m_goodSegmentIndices){
        s << goodSegmentIndex->print() << "\n";
    }
    s << "Connecting arcs: " << m_disjointIntervals.print() << "\n";
    s << "Length: " << length();
    return s.str();
}










//-------------------------//
// InitArguments_Foliation //
//-------------------------//


std::vector<floating_point_type> InitArguments_Foliation::arg_lengths;
Permutation InitArguments_Foliation::arg_permutation;
floating_point_type InitArguments_Foliation::arg_twist;


InitArguments_Foliation::InitArguments_Foliation(const FoliationSphere& foliationSphere)
{
    
    std::vector<ConnectedPoints> allConnectedPoints;
    allConnectedPoints.reserve(foliationSphere.topFoliation().numSeparatrices() + foliationSphere.bottomFoliation().numSeparatrices());
    
    generateTopConnectingPairs(foliationSphere, allConnectedPoints);
    generateBottomConnectingPairs(foliationSphere, allConnectedPoints);
    
    
    std::sort(allConnectedPoints.begin(), allConnectedPoints.end(), [] (const ConnectedPoints& cp1, const ConnectedPoints& cp2)
              {
                  return cp1.topPoint < cp2.topPoint;
              } );
    std::vector<ConnectedPoints>& allConnectedPointsSortedByTop = allConnectedPoints;
    std::vector<UnitIntervalPoint> allBottomPoints;
    allBottomPoints.reserve(allConnectedPoints.size());
    
    for (ConnectedPoints cp : allConnectedPoints) {
        allBottomPoints.push_back(cp.bottomPoint);
    }
    
    std::sort(allBottomPoints.begin(), allBottomPoints.end());
    
    arg_lengths.resize(allConnectedPoints.size());
    for (int i = 0; i < allConnectedPoints.size() - 1; i++) {
        arg_lengths[i] = distanceBetween( allConnectedPointsSortedByTop[i].topPoint, allConnectedPointsSortedByTop[i + 1].topPoint);
    }
    arg_lengths[allConnectedPoints.size() - 1] = 1 - allConnectedPointsSortedByTop[allConnectedPoints.size() - 1].topPoint.getPosition();
    
    std::vector<int> permutationInput(allConnectedPoints.size());
    for (int i = 0; i < allConnectedPoints.size(); i++) {
        auto it = std::lower_bound(allBottomPoints.begin(), allBottomPoints.end(), allConnectedPointsSortedByTop[i].bottomPoint);
        permutationInput[i] = static_cast<int>(it - allBottomPoints.begin()) ;
    }
    arg_permutation = Permutation(permutationInput);
   // std::cout << arg_permutation;
    arg_twist = allBottomPoints[0].getPosition();
}






void InitArguments_Foliation::generateTopConnectingPairs(const FoliationSphere& foliationSphere,
                                                         std::vector<ConnectedPoints>& allConnectedPoints)
{
    int numSeparatrices = foliationSphere.topFoliation().intervalPairing().size();
    for (int i = 0; i < numSeparatrices; i++) {
        Modint modi(i, numSeparatrices);
        if (foliationSphere.topFoliation().intervalPairing().permutation()[modi] != Modint(i - 1, numSeparatrices)) {
            // otherwise the current separatrix emanates from a 1-pronged singularity which is not important
            
            ConnectedPoints newConnectedPoints;
            newConnectedPoints.topPoint = foliationSphere.topFoliation().intervalPairing().divPoints()[i];
            
            int indexOfConnectedSeparatrix = Modint(foliationSphere.topFoliation().intervalPairing().permutation()[i]+ 1, numSeparatrices);
            UnitIntervalPoint middlePoint = foliationSphere.topFoliation().intervalPairing().divPoints()[indexOfConnectedSeparatrix];
            
            newConnectedPoints.bottomPoint = foliationSphere.bottomFoliation().intervalPairing().applyTo(middlePoint - foliationSphere.twist()) + foliationSphere.twist();
            
            
            allConnectedPoints.push_back( newConnectedPoints );
        }
    }
}



void InitArguments_Foliation::generateBottomConnectingPairs(const FoliationSphere& foliationSphere,
                                                            std::vector<ConnectedPoints>& allConnectedPoints)
{
    int numSeparatrices = foliationSphere.bottomFoliation().intervalPairing().size();
    for (int i = 0; i < numSeparatrices; i++) {
        if (foliationSphere.bottomFoliation().intervalPairing().permutation()[i] != Modint(i - 1, numSeparatrices) ) {
            
            ConnectedPoints newConnectedPoints;
            newConnectedPoints.bottomPoint = foliationSphere.bottomFoliation().intervalPairing().divPoints()[i] + foliationSphere.twist();
            
            int indexOfConnectedSeparatrix = Modint(foliationSphere.bottomFoliation().intervalPairing().permutation()[i] + 1, numSeparatrices);
            UnitIntervalPoint middlePoint = foliationSphere.topFoliation().intervalPairing().divPoints()[indexOfConnectedSeparatrix] + foliationSphere.twist();
            
            newConnectedPoints.topPoint = foliationSphere.topFoliation().intervalPairing().applyTo(middlePoint);
            
            allConnectedPoints.push_back( newConnectedPoints );
        }
    }
    
}
















//-----------//
// Foliation //
//-----------//


Foliation::Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist) :
    InitArguments_Foliation(),
    m_twistedIntervalExchange(lengths, permutation, twist)
{
    Init();
}

Foliation::Foliation(const FoliationRP2& foliationRP2) :
    Foliation(FoliationSphere(foliationRP2))
{
}

Foliation::Foliation(const FoliationSphere& foliationSphere) :
    InitArguments_Foliation(foliationSphere),
    m_twistedIntervalExchange(arg_lengths, arg_permutation, arg_twist)
{
    Init();
}



Foliation::Foliation(const TwistedIntervalExchangeMap& twistedintervalExchange) :
    InitArguments_Foliation(),
    m_twistedIntervalExchange(twistedintervalExchange)
{
    Init();
}



void Foliation::Init(){
    m_numIntervals = m_twistedIntervalExchange.sizeBeforeTwist();
    m_numSeparatrices = 2 * m_numIntervals;
    
    struct DivPoint{
        UnitIntervalPoint m_point;
        bool m_isTopPoint;
    };
    
    std::vector<DivPoint> divPoints;
    divPoints.reserve(m_numSeparatrices);
    
    m_topRealDivPoints.reserve(m_numIntervals);
    m_pairOfTopDivPoints.reserve(m_numIntervals);
    for (int i = 0; i < m_numIntervals + 1; i++) {
        if (i != m_twistedIntervalExchange.m_indexOfFakeDivPoint) {
            m_topRealDivPoints.push_back(m_twistedIntervalExchange.m_intervalExchangeAfterTwist.divPoints()[i]);
            m_pairOfTopDivPoints.push_back(m_twistedIntervalExchange.m_intervalExchangeAfterTwist.permutation()[i] - 1);

            DivPoint divpoint;
            divpoint.m_point = m_twistedIntervalExchange.m_intervalExchangeAfterTwist.divPoints()[i];
            divpoint.m_isTopPoint = true;
            divPoints.push_back(divpoint);
        }
    }
    
    m_bottomRealDivPoints.reserve(m_numIntervals);
    auto temp = m_twistedIntervalExchange.m_intervalExchangeAfterTwist.divPointsAfterExchange();
    for(auto it = temp.begin() + 1; it != temp.end(); it++)
        m_bottomRealDivPoints.push_back(*it);
    
    
    for (int i = 1; i < m_numIntervals + 1; i++) {
        DivPoint divpoint;
        divpoint.m_point = m_twistedIntervalExchange.m_intervalExchangeAfterTwist.divPointsAfterExchange()[i];
        divpoint.m_isTopPoint = false;
        divPoints.push_back(divpoint);
    }
    
    std::inplace_merge(divPoints.begin(), divPoints.begin() + m_numIntervals, divPoints.end(), [] (const DivPoint& p1, const DivPoint& p2)
                       {
                           return p1.m_point < p2.m_point;
                       } );
    
    m_allRealDivPoints.reserve(m_numSeparatrices);
    m_isTopDivPoint.reserve(m_numSeparatrices);
    for (auto p : divPoints) {
        m_allRealDivPoints.push_back(p.m_point);
        m_isTopDivPoint.push_back(p.m_isTopPoint);
    }
    
    checkPointsAreNotTooClose(m_allRealDivPoints);
  /*
    std::cout << m_isTopDivPoint << std::endl;
    std::cout << m_allRealDivPoints << std::endl;
    std::cout << m_topRealDivPoints << std::endl;
    std::cout << m_bottomRealDivPoints << std::endl;
    std::cout << m_pairOfTopDivPoints << std::endl;
*/
    
    m_goodShiftedSeparatrixSegments[UPWARDS].resize(m_numIntervals);
    m_goodShiftedSeparatrixSegments[DOWNWARDS].resize(m_numIntervals);
    for (int i = 0; i < m_numIntervals; i++) {
        m_goodShiftedSeparatrixSegments[DOWNWARDS][i].emplace_back(*this, i, DOWNWARDS);
        m_goodShiftedSeparatrixSegments[UPWARDS][i].emplace_back(*this, i, UPWARDS);
        m_goodShiftedSeparatrixSegments[DOWNWARDS][i].push_back(m_goodShiftedSeparatrixSegments[DOWNWARDS][i].back());
        m_goodShiftedSeparatrixSegments[UPWARDS][i].push_back(m_goodShiftedSeparatrixSegments[UPWARDS][i].back());
    }
}






bool Foliation::reachedSaddleConnection(Direction direction, int index) const
{
    return m_goodShiftedSeparatrixSegments[direction][index].back().m_smallContainingInterval == CONTAINING_INTERVAL_NOT_UNIQUE;
}








void Foliation::findNextSepSegment(Direction direction, int index){
    assert(!reachedSaddleConnection(direction, index));
    
    SeparatrixSegment& segment = m_goodShiftedSeparatrixSegments[direction][index].back();
    segment.m_intervalIntersectionCount[containingInterval(m_topRealDivPoints, segment.m_endpoint)]++;
    segment.m_arcsAroundDivPoints.InsertPoint(segment.m_endpoint, segment.m_smallContainingInterval);
    segment.m_depth++;
    segment.m_endpoint = segment.m_direction == UPWARDS ? m_twistedIntervalExchange.applyTo(segment.m_endpoint) : m_twistedIntervalExchange.applyInverseTo(segment.m_endpoint);
    segment.m_endpoint = UnitIntervalPoint(segment.m_endpoint.getPosition()); // remove infinitesimal shift
    segment.m_smallContainingInterval = containingInterval(m_allRealDivPoints, segment.m_endpoint);
    
    if (segment.m_smallContainingInterval != CONTAINING_INTERVAL_NOT_UNIQUE &&
        segment.m_arcsAroundDivPoints.ContainsQ(segment.m_endpoint, segment.m_smallContainingInterval)) {
        m_goodShiftedSeparatrixSegments[direction][index].push_back(segment);
    }
}




void Foliation::generateSepSegments(int depth){
    for (Direction direction = Direction::UPWARDS; direction <= Direction::DOWNWARDS; direction++) {
        for (int index = 0; index < m_numIntervals; index++) {
            while (!reachedSaddleConnection(direction, index) &&
                   m_goodShiftedSeparatrixSegments[direction][index].back().m_depth < depth) {
                findNextSepSegment(direction, index);
            }
    //        std::cout << std::endl << std::endl;
        }
    }
}




// For arbitrary intervals this function finds the first good separatrix segment that intersects the intervals.
// However, in the appliations, we use a special collection of intervals, in which case the first intersection is always a good one.
const Foliation::SeparatrixSegment& Foliation::getFirstIntersection(Direction direction,
                                                                    int index,
                                                                    const DisjointIntervals& intervals)
{
    for (auto &segment : m_goodShiftedSeparatrixSegments[direction][index]) {
        if (intervals.containsQ(segment.m_endpoint)) { // we are gonna have to catch an error here
            return segment;
        }
    }
    while (!intervals.containsQ(m_goodShiftedSeparatrixSegments[direction][index].back().m_endpoint)) {
        if (m_goodShiftedSeparatrixSegments[direction][index].back().m_smallContainingInterval == CONTAINING_INTERVAL_NOT_UNIQUE) {
            throw std::runtime_error("getFirstIntersection: First intersection cannot be found, because we found a saddle connection.");
        }
        findNextSepSegment(direction, index);
    }
    return m_goodShiftedSeparatrixSegments[direction][index].back();
}




void Foliation::checkPointsAreNotTooClose(const std::vector<UnitIntervalPoint>& points){
    for (auto it = points.begin() + 1; it != points.end(); it++) {
        if (!(*(it - 1) < *it)) {
            throw std::runtime_error("Some points are so close that we can't distinguish them.");
        }
    }
}








std::array<bool, 2> Foliation::whichTransverseCurvesExist(const std::vector<std::list<SeparatrixSegment>::iterator>& goodSegmentIndices){
    
    
    assert(goodSegmentIndices.size() % 2 == 0);
    assert(goodSegmentIndices.size() >= 2);
    std::vector<short> singularities(m_numIntervals, 0);
    for (int i = 0; i < goodSegmentIndices.size(); i += 2){
        assert(goodSegmentIndices[i]->m_startingSingularity == goodSegmentIndices[i + 1]->m_startingSingularity);
        assert(singularities[goodSegmentIndices[i]->m_startingSingularity] == 0);
        singularities[goodSegmentIndices[i]->m_startingSingularity] = 1;
        assert(goodSegmentIndices[i]->m_direction != goodSegmentIndices[i + 1]->m_direction);
    }
    
    
    std::array<bool, 2> isCandidateForWrapsAroundZero = {true, true};
    std::vector<std::pair<UnitIntervalPoint, int>> endpointsAndIndices;
    
    endpointsAndIndices.reserve(goodSegmentIndices.size());
    for (int i = 0; i < goodSegmentIndices.size(); i++){
        endpointsAndIndices.emplace_back(goodSegmentIndices[i]->m_endpoint, i);
    }
    std::sort(endpointsAndIndices.begin(), endpointsAndIndices.end());
    for (auto it = endpointsAndIndices.begin() + 1; it != endpointsAndIndices.end(); it++) {
        if (!((it - 1)->first < it->first)) {
            throw std::runtime_error("Some points are so close that we can't distinguish them.");
        }
    }
    
    
    
    
    // checking that the curve is transverse
    // this part does not depend on wrapsAroundZero
    {
        std::vector<short> isEndpointIndexOdd(goodSegmentIndices.size());
        for (int i = 0; i < endpointsAndIndices.size(); i++) {
            isEndpointIndexOdd[endpointsAndIndices[i].second] = (i % 2);
        }
        for (int i = 0; i < endpointsAndIndices.size(); i += 2) {
            if (isEndpointIndexOdd[i] == isEndpointIndexOdd[i + 1]) {
                return {false, false};
            }
        }
    }
    
    // checking that the curve is connected
    
    std::vector<UnitIntervalPoint> endpoints;
    endpoints.reserve(endpointsAndIndices.size());
    for (auto& x : endpointsAndIndices) {
        endpoints.push_back(x.first);
    }
    

    for (short wrapsAroundZero = 0; wrapsAroundZero < 2; wrapsAroundZero++){
        Modint index(0, static_cast<int>(goodSegmentIndices.size()));
        int length = 0;
        do {
            if ((wrapsAroundZero && index % 2 == 1) || (!wrapsAroundZero && index % 2 == 0)) {
                ++index;
            } else
                --index;
            int pair = endpointsAndIndices[index].second % 2 == 0 ? endpointsAndIndices[index].second + 1 :
            endpointsAndIndices[index].second - 1;
            auto it = std::lower_bound(endpoints.begin(), endpoints.end(), goodSegmentIndices[pair]->m_endpoint);
            index = Modint(static_cast<int>(it - endpoints.begin()), static_cast<int>(goodSegmentIndices.size()));
            length += 2;
        } while (index != 0);
        if (length < goodSegmentIndices.size()) {
            isCandidateForWrapsAroundZero[wrapsAroundZero] = false;
        }
    }
    
    
    // checking that the curve is minimal (can't be simplified trivially) and that it is simple
    
    std::vector<const ArcsAroundDivPoints*> adpVector;
    adpVector.reserve(goodSegmentIndices.size());
    for (auto ps : goodSegmentIndices)
        adpVector.push_back(&ps->m_arcsAroundDivPoints);
    ArcsAroundDivPoints adpIntersection = intersect(adpVector);

    for (short wrapsAroundZero = 0; wrapsAroundZero < 2; wrapsAroundZero++){
        if (isCandidateForWrapsAroundZero[wrapsAroundZero]) {
            for (int i = wrapsAroundZero ? 1 : 0; i < goodSegmentIndices.size(); i += 2) {
                int next = (i + 1) % goodSegmentIndices.size();
                bool throughTopDivPoint = goodSegmentIndices[endpointsAndIndices[i].second]->m_direction == DOWNWARDS &&
                goodSegmentIndices[endpointsAndIndices[next].second]->m_direction == DOWNWARDS ? true : false;
                
                
                if (!adpIntersection.ContainsArcThroughADivPointQ(endpointsAndIndices[i].first,
                                                                  goodSegmentIndices[endpointsAndIndices[i].second]->m_smallContainingInterval,
                                                                  endpointsAndIndices[next].first,
                                                                  goodSegmentIndices[endpointsAndIndices[next].second]->m_smallContainingInterval,
                                                                  throughTopDivPoint))
                {
                    isCandidateForWrapsAroundZero[wrapsAroundZero] = false;
                }
            }
        }
    }
    
    return isCandidateForWrapsAroundZero;
}









Foliation Foliation::rotateBy(int rotationAmount) const{
    return Foliation(m_twistedIntervalExchange.rotateBy(rotationAmount));
}


Foliation Foliation::reflect() const{
    return Foliation(m_twistedIntervalExchange.reflect());
}


Foliation Foliation::flipOver() const{
    return Foliation(m_twistedIntervalExchange.invert().reflect());
}






std::ostream& operator<<(std::ostream& Out, Foliation f){
    std::cout << f.m_twistedIntervalExchange;
    return Out;
}







void Foliation::printGoodSepSegments(int depth, bool verbose){
    generateSepSegments(depth);
    for (Direction direction = Direction::UPWARDS; direction <= Direction::DOWNWARDS; direction++) {
        for (int i = 0; i < m_numIntervals; i++) {
            std::cout << i << (direction == UPWARDS ? " UP" : " DOWN") << "\n";
            for(auto it = m_goodShiftedSeparatrixSegments[direction][i].begin(); it !=
                ++m_goodShiftedSeparatrixSegments[direction][i].end(); it++)
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











Foliation arnouxYoccozFoliation(int genus){
    std::vector<int> permutationInput(2 * genus);
    std::vector<floating_point_type> lengths(2 * genus);
    floating_point_type shrinkingNumber = 1/arnouxYoccozStretchFactor(genus);
    floating_point_type currentLength = shrinkingNumber;
    for (int i = 0; i < genus; i++) {
        permutationInput[2 * i] = 2 * i + 1;
        permutationInput[2 * i + 1] = 2 * i;
        lengths[2 * i] = lengths[2 * i + 1] = currentLength;
        currentLength *= shrinkingNumber;
    }
    try {
        return Foliation(lengths, Permutation(permutationInput), 0.5);
    } catch (const std::exception& ex) {
        throw std::runtime_error("Machine precision is not enough to handle such a high genus Arnoux-Yoccox surface.");
    }
}








//------------------//
// FoliationFromRP2 //
//------------------//


FoliationFromRP2::FoliationFromRP2(const FoliationRP2& foliationRP2):
    FoliationFromSphere(foliationRP2)
{
    m_separatrixPair.resize(m_numIntervals);
    for (int i = 0 ; i < m_numIntervals; i++) {
        UnitIntervalPoint pair = m_twistedIntervalExchange.applyTo(m_goodShiftedSeparatrixSegments[DOWNWARDS][i].front().m_endpoint) - 0.5;
        for (int j = 0; j < m_numIntervals; j++) {
            if (distanceBetween(m_topRealDivPoints[j], pair) < UnitIntervalPoint::PRECISION || distanceBetween(pair, m_topRealDivPoints[j]) < UnitIntervalPoint::PRECISION) {
                m_separatrixPair[i] = j;
                break;
            }
        }
    }
    std::cout << m_separatrixPair;
}

bool FoliationFromRP2::TransverseCurveIteratorComp::operator()(std::set<TransverseCurve>::const_iterator it1,
                                                               std::set<TransverseCurve>::const_iterator it2)
{
    return *it1 < *it2;
}




void FoliationFromRP2::generateLiftsOfGoodTransverseCurves(int depth){
    generateSepSegments(depth);
    std::vector<std::list<SeparatrixSegment>::iterator> transverseCurveInput;
    transverseCurveInput.reserve(4);
    
    for (int index = 0; index < m_numIntervals; index++) {
        if (index < m_separatrixPair[index]) {

            
            auto itFirstDown = m_goodShiftedSeparatrixSegments[DOWNWARDS][index].begin();
            auto itSecondUp = m_goodShiftedSeparatrixSegments[UPWARDS][m_separatrixPair[index]].begin();
            while (itFirstDown != --m_goodShiftedSeparatrixSegments[DOWNWARDS][index].end() &&
                   itSecondUp != --m_goodShiftedSeparatrixSegments[UPWARDS][m_separatrixPair[index]].end() &&
                   itFirstDown->m_depth <= depth &&
                   itSecondUp->m_depth <= depth) {
                assert(itFirstDown->m_depth == itSecondUp->m_depth);
                
                auto itFirstUp = m_goodShiftedSeparatrixSegments[UPWARDS][index].begin();
                auto itSecondDown = m_goodShiftedSeparatrixSegments[DOWNWARDS][m_separatrixPair[index]].begin();
                while (itFirstUp != --m_goodShiftedSeparatrixSegments[UPWARDS][index].end() &&
                       itSecondDown != --m_goodShiftedSeparatrixSegments[DOWNWARDS][m_separatrixPair[index]].end() &&
                       itFirstUp->m_depth <= depth &&
                       itSecondDown->m_depth <= depth) {
                    
                    assert(itFirstUp->m_depth == itSecondDown->m_depth);
                    
                    transverseCurveInput = {
                        itFirstDown,
                        itFirstUp,
                        itSecondUp,
                        itSecondDown
                    };
                    
                    std::array<bool, 2> isWrapsAroundZeroGood = whichTransverseCurvesExist(transverseCurveInput);
                    for (short wrapsAroundZero = 0; wrapsAroundZero < 2; wrapsAroundZero++ ) {
                        if (isWrapsAroundZeroGood[wrapsAroundZero]) {
                            auto ret = m_transverseCurves.emplace(*this, transverseCurveInput, wrapsAroundZero);
                            if (ret.second) {
                                m_liftsOfGoodTransverseCurves.insert(ret.first);
                            }
                        }
                    }
                    
                    
                    itFirstUp++;
                    itSecondDown++;
                }

                
                itFirstDown++;
                itSecondUp++;
            }
            
                
                
            
        }
    }
}


void FoliationFromRP2::printLiftsOfGoodTransverseCurves(int depth){
    generateLiftsOfGoodTransverseCurves(depth);
    for (auto it : m_liftsOfGoodTransverseCurves)
        std::cout << it->print() << "\n\n";
}



//---------------------//
// FoliationFromSphere //
//---------------------//


FoliationFromSphere::FoliationFromSphere(const FoliationSphere& foliationSphere) :
    Foliation(foliationSphere)
{
}




