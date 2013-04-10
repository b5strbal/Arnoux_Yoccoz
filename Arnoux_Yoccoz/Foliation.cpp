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



Foliation::DisjointIntervals::DisjointIntervals(const std::vector<UnitIntervalPoint>& points, bool wrapsAroundZero) :
m_points(points),
m_wrapsAroundZero(wrapsAroundZero)
{
    assert(points.size() % 2 == 0);
    std::sort(m_points.begin(), m_points.end());
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


floating_point_type Foliation::DisjointIntervals::totalLength() const {
    floating_point_type sum = 0;
    for (int i = 0; i < m_points.size(); i += 2) {
        sum += distanceBetween(m_points[i], m_points[i + 1]);
    }
    return m_wrapsAroundZero ? 1 - sum : sum;
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
    switch (m_cuttingPoints[IndexOfInterval].size()) {
        case 0:
            m_cuttingPoints[IndexOfInterval].push_back(NewCuttingPoint);
            break;
            
        case 1:
            if (m_cuttingPoints[IndexOfInterval][0] < NewCuttingPoint){
                m_cuttingPoints[IndexOfInterval].push_back(NewCuttingPoint);
            } else {
                m_cuttingPoints[IndexOfInterval].insert(m_cuttingPoints[IndexOfInterval].begin(), NewCuttingPoint);
            }
            break;
            
        case 2:
            if (NewCuttingPoint < m_cuttingPoints[IndexOfInterval][0]) {
                m_cuttingPoints[IndexOfInterval][0] = NewCuttingPoint;
            } else if (m_cuttingPoints[IndexOfInterval][1] < NewCuttingPoint){
                m_cuttingPoints[IndexOfInterval][1] = NewCuttingPoint;
            }
    }
}





bool Foliation::ArcsAroundDivPoints::ContainsQ(const UnitIntervalPoint& c, int IndexOfInterval) const{
    if (m_cuttingPoints[IndexOfInterval].size() == 2 && m_cuttingPoints[IndexOfInterval][0] < c && c < m_cuttingPoints[IndexOfInterval][1])
    {
        return false;
    }
    return true;
}








bool Foliation::ArcsAroundDivPoints::ContainsArcThroughADivPointQ(const UnitIntervalPoint& LeftEndPoint, int LeftIndexOfInterval,
                                                                  const UnitIntervalPoint& RightEndPoint, int RightIndexOfInterval,
                                                                  bool throughTopDivPointQ) const{
    
    if (LeftIndexOfInterval == RightIndexOfInterval) {
        return false;
    }
    if (m_cuttingPoints[LeftIndexOfInterval].size() > 0 && LeftEndPoint < m_cuttingPoints[LeftIndexOfInterval].back()) {
        return false;
    }
    if (m_cuttingPoints[RightIndexOfInterval].size() > 0 && m_cuttingPoints[RightIndexOfInterval].front() < RightEndPoint) {
        return false;
    }
    int i;
    bool foundGoodDivPoint = false;
    for (i = LeftIndexOfInterval + 1; i != RightIndexOfInterval; i = (i + 1) % m_foliation.m_numSeparatrices) {
        if ((throughTopDivPointQ && m_foliation.m_isTopDivPoint[i]) || (!throughTopDivPointQ && !m_foliation.m_isTopDivPoint[i])) {
            foundGoodDivPoint = true;
        }
        if (!m_cuttingPoints[i].empty()) {
            break;
        }
    }
    if (i != RightIndexOfInterval || !foundGoodDivPoint) {
        return false;
    }
    return true;
}



std::string Foliation::ArcsAroundDivPoints::print() const
{
    std::ostringstream s;
    for (auto& v : m_cuttingPoints){
        switch (v.size()) {
            case 0:
                s << "() ";
                break;
                
            case 1:
                s << "(" << v[0] << ") ";
                break;
                
            case 2:
                s << "(" << v[0] << "," << v[1] << ") ";
        }
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
            if (adp.m_cuttingPoints[i].empty()) {
                adp.m_cuttingPoints[i] = (*it)->m_cuttingPoints[i];
            } else if ((*it)->m_cuttingPoints[i].empty()){
            } else {
                if (adp.m_cuttingPoints[i].size() == 1) {
                    adp.m_cuttingPoints[i].push_back(adp.m_cuttingPoints[i].front());
                }
                adp.m_cuttingPoints[i][0] = std::min(adp.m_cuttingPoints[i].front(), (*it)->m_cuttingPoints[i].front());
                adp.m_cuttingPoints[i][1] = std::max(adp.m_cuttingPoints[i].back(), (*it)->m_cuttingPoints[i].back());
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



Foliation::TransverseCurve::TransverseCurve(const Foliation& foliation, const std::vector<const SeparatrixSegment*>& segments, bool wrapsAroundZero) :
    m_foliation(foliation)
{
    assert(segments.size() % 2 == 0);
    assert(segments.size() >= 2);
    std::vector<short> singularities(foliation.m_numIntervals, 0);
    for (int i = 0; i < segments.size()/2; i++){
        assert(segments[2 * i]->m_startingSingularity == segments[2 * i + 1]->m_startingSingularity);
        assert(singularities[segments[2 * i]->m_startingSingularity] == 0);
        singularities[segments[2 * i]->m_startingSingularity] = 1;
        assert(segments[2 * i]->m_direction != segments[2 * i + 1]->m_direction);
    }
    
    std::vector<std::pair<UnitIntervalPoint, int>> endpointsAndIndices;
    endpointsAndIndices.reserve(segments.size());
    for (int i = 0; i < segments.size(); i++){
        endpointsAndIndices.push_back(std::make_pair(segments[i]->m_endpoint, i));
    }
    std::sort(endpointsAndIndices.begin(), endpointsAndIndices.end());
    for (auto it = endpointsAndIndices.begin() + 1; it != endpointsAndIndices.end(); it++) {
        if (!((it - 1)->first < it->first)) {
            throw std::runtime_error("Some points are so close that we can't distinguish them.");
        }
    }
    
    std::vector<const ArcsAroundDivPoints*> adpVector;
    adpVector.reserve(segments.size());
    for (auto ps : segments)
        adpVector.push_back(&ps->m_arcsAroundDivPoints);
    ArcsAroundDivPoints adpIntersection = m_foliation.intersect(adpVector);
    
    int startindex = wrapsAroundZero ? 1 : 0;
    for (int i = startindex; i != startindex; i = (i + 2) % segments.size()) {
        bool throughTopDivPoint = segments[endpointsAndIndices[i].second]->m_direction == DOWNWARDS &&
                                    segments[endpointsAndIndices[i + 1].second]->m_direction == DOWNWARDS ? true : false;
            
    
        if (!adpIntersection.ContainsArcThroughADivPointQ(endpointsAndIndices[i].first,
                                                         segments[endpointsAndIndices[i].second]->m_smallContainingInterval,
                                                         endpointsAndIndices[i + 1].first,
                                                         segments[endpointsAndIndices[i + 1].second]->m_smallContainingInterval,
                                                         throughTopDivPoint))
        {
            throw std::runtime_error("No transverse curve can be constructed from the given separatrix segments.");
        }
    }

    std::vector<UnitIntervalPoint> endpoints;
    endpoints.reserve(endpointsAndIndices.size());
    for (auto& x : endpointsAndIndices) {
        endpoints.push_back(x.first);
    }
    
    m_disjointIntervals = DisjointIntervals(endpoints, wrapsAroundZero);
    m_separatrixSegments = segments; // We will probably have to find a more cleverly sorted way of storing the segments later.
}





std::string Foliation::TransverseCurve::print() const
{
    std::ostringstream s;
    s << "Separatrix segments:\n";
    for (auto segment : m_separatrixSegments){
        s << segment->print() << "\n";
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
    
    arg_twist = allBottomPoints[0].getPosition();
}






void InitArguments_Foliation::generateTopConnectingPairs(const FoliationSphere& foliationSphere,
                                                         std::vector<ConnectedPoints>& allConnectedPoints)
{
    int numSeparatrices = foliationSphere.topFoliation().intervalPairing().size();
    for (int i = 0; i < numSeparatrices; i++) {
        if (foliationSphere.topFoliation().intervalPairing().permutation()[i] != modAdd(i, -1, numSeparatrices) ) {
            // otherwise the current separatrix emanates from a 1-pronged singularity which is not important
            
            ConnectedPoints newConnectedPoints;
            newConnectedPoints.topPoint = foliationSphere.topFoliation().intervalPairing().divPoints()[i];
            
            int indexOfConnectedSeparatrix = modAdd(foliationSphere.topFoliation().intervalPairing().permutation()[i], 1, numSeparatrices);
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
        if (foliationSphere.bottomFoliation().intervalPairing().permutation()[i] != modAdd(i, -1, numSeparatrices) ) {
            
            ConnectedPoints newConnectedPoints;
            newConnectedPoints.bottomPoint = foliationSphere.bottomFoliation().intervalPairing().divPoints()[i] + foliationSphere.twist();
            
            int indexOfConnectedSeparatrix = modAdd(foliationSphere.bottomFoliation().intervalPairing().permutation()[i], 1, numSeparatrices);
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
    
    m_currentSepSegments[UPWARDS].reserve(m_numIntervals);
    m_currentSepSegments[DOWNWARDS].reserve(m_numIntervals);
    m_goodShiftedSeparatrixSegments[UPWARDS].reserve(m_numIntervals);
    m_goodShiftedSeparatrixSegments[DOWNWARDS].reserve(m_numIntervals);
    for (int i = 0; i < m_numIntervals; i++) {
        m_currentSepSegments[DOWNWARDS].emplace_back(*this, i, DOWNWARDS);
        m_currentSepSegments[UPWARDS].emplace_back(*this, i, UPWARDS);
        m_goodShiftedSeparatrixSegments[DOWNWARDS][i].push_back(m_currentSepSegments[DOWNWARDS].back());
        m_goodShiftedSeparatrixSegments[UPWARDS][i].push_back(m_currentSepSegments[UPWARDS].back());
    }
    
    
}




void Foliation::findNextSepSegment(Direction direction, int index){
    assert(m_currentSepSegments[direction][index].m_smallContainingInterval != CONTAINING_INTERVAL_NOT_UNIQUE);
    
    SeparatrixSegment& segment = m_currentSepSegments[direction][index];
    segment.m_intervalIntersectionCount[containingInterval(m_topRealDivPoints, segment.m_endpoint)]++;
    segment.m_arcsAroundDivPoints.InsertPoint(segment.m_endpoint, segment.m_smallContainingInterval);
    segment.m_depth++;
    segment.m_endpoint = segment.m_direction == UPWARDS ? m_twistedIntervalExchange.applyTo(segment.m_endpoint) : m_twistedIntervalExchange.applyInverseTo(segment.m_endpoint);
    segment.m_smallContainingInterval = containingInterval(m_allRealDivPoints, segment.m_endpoint);
    
    if (segment.m_smallContainingInterval != CONTAINING_INTERVAL_NOT_UNIQUE &&
        segment.m_arcsAroundDivPoints.ContainsQ(segment.m_endpoint, segment.m_smallContainingInterval)) {
        std::vector<SeparatrixSegment>& goodSegments = m_goodShiftedSeparatrixSegments[direction][index];
        goodSegments.push_back(segment);
    //    std::cout << currentSepSegments[index].m_depth << " ";
    }
}




void Foliation::generateSepSegments(int depth){
    for (Direction direction = Direction::UPWARDS; direction <= Direction::DOWNWARDS; direction++) {
        for (int index = 0; index < m_numIntervals; index++) {
            while (m_currentSepSegments[direction][index].m_smallContainingInterval != CONTAINING_INTERVAL_NOT_UNIQUE &&
                   m_currentSepSegments[direction][index].m_depth < depth) {
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
            std::cout << depth << (direction == UPWARDS ? " UP" : " DOWN") << "\n";
            for(auto &segment : m_goodShiftedSeparatrixSegments[direction][i])
            {
                if (verbose) {
                    std::cout << segment.print() << "\n\n";
                } else
                    std::cout << segment.m_depth << " ";
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
    Foliation(foliationRP2)
{
}


void FoliationFromRP2::generateLiftsOfGoodTransverseCurves(int depth) const{


}






//---------------------//
// FoliationFromSphere //
//---------------------//


FoliationFromSphere::FoliationFromSphere(const FoliationSphere& foliationSphere) :
    Foliation(foliationSphere)
{
}




