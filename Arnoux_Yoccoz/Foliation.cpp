/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains implementation for the Foliation class.
 */

#include "Foliation.h"


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
                                                                  const UnitIntervalPoint& RightEndPoint, int RightIndexOfInterval) const{
    
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
    for (i = LeftIndexOfInterval + 1; i != RightIndexOfInterval; i = (i + 1) % m_numDivPoints) {
        if (m_cuttingPoints[i].empty()) {
            break;
        }
    }
    if (i != RightIndexOfInterval) {
        return false;
    }
    return true;
    
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
Foliation::ArcsAroundDivPoints Foliation::Intersect(const ArcsAroundDivPoints& adp1, const ArcsAroundDivPoints& adp2)
{    
    ArcsAroundDivPoints adp = adp1;
    for (int i = 0; i < adp1.m_numDivPoints; i++) {
        if (adp1.m_cuttingPoints[i].empty()) {
            adp.m_cuttingPoints[i] = adp2.m_cuttingPoints[i];
        } else if (adp2.m_cuttingPoints[i].empty()){
        } else {
            adp.m_cuttingPoints[i].resize(2);
            adp.m_cuttingPoints[i][0] = std::min(adp1.m_cuttingPoints[i].front(), adp2.m_cuttingPoints[i].front());
            adp.m_cuttingPoints[i][1] = std::max(adp1.m_cuttingPoints[i].back(), adp2.m_cuttingPoints[i].back());
        }
    }
    return adp;
}






/*
 std::ostream& operator<<(std::ostream& Out, const ArcsAroundDivPoints& adp){
 using namespace std;
 
 Out << "Division points: ";
 for (int i = 0; i < adp.m_DivPoints.size(); i++) {
 Out << adp.m_DivPoints[i] << " ";
 }
 Out << endl << "Intervals: ";
 for (int i = 0; i < adp.m_DivPoints.size(); i++) {
 cout << endl;
 cout << i << ": ";
 if (adp.IsEmpty()) {
 cout << "whole circle";
 } else {
 cout << adp.m_Arcs[i];
 }
 }
 return Out;
 }
 */





Foliation::SeparatrixSegment::SeparatrixSegment(const Foliation& foliation, int startingSingularity, bool isGoingUp) :
    m_foliation(foliation),
    m_startingSingularity(startingSingularity),
    m_isGoingUp(isGoingUp),
    m_depth(1),
    m_intervalIntersectionCount(std::vector<int>(foliation.m_numIntervals, 0)),
    m_arcsAroundDivPoints(foliation.m_numSeparatrices)
{
    if (isGoingUp) {
        m_endpoint = UnitIntervalPoint( foliation.m_bottomRealDivPoints[foliation.m_pairOfTopDivPoints[startingSingularity]].getPosition(), 1);
    } else
        m_endpoint = UnitIntervalPoint( foliation.m_topRealDivPoints[startingSingularity].getPosition(), 1);
    
    m_smallContainingInterval = containingInterval(foliation.m_allRealDivPoints, m_endpoint);
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
    
    
    for (auto it = m_allRealDivPoints.begin() + 1; it != m_allRealDivPoints.end(); it++) {
        if (!(*(it - 1) < *it)) {
            throw std::runtime_error("Foliation: The foliation has an immediate saddle connection");
        }
    }
    std::cout << m_isTopDivPoint << std::endl;
    std::cout << m_allRealDivPoints << std::endl;
    std::cout << m_topRealDivPoints << std::endl;
    std::cout << m_bottomRealDivPoints << std::endl;
    std::cout << m_pairOfTopDivPoints << std::endl;

    m_currentSepSegmentsUp.reserve(m_numIntervals);
    m_currentSepSegmentsDown.reserve(m_numIntervals);
    m_goodShiftedSeparatrixSegmentsUp.resize(m_numIntervals);
    m_goodShiftedSeparatrixSegmentsDown.resize(m_numIntervals);
    for (int i = 0; i < m_numIntervals; i++) {
        m_currentSepSegmentsDown.emplace_back(*this, i, false);
        m_currentSepSegmentsUp.emplace_back(*this, i, true);
        m_goodShiftedSeparatrixSegmentsDown[i].push_back(m_currentSepSegmentsDown.back());
        m_goodShiftedSeparatrixSegmentsUp[i].push_back(m_currentSepSegmentsUp.back());
    }
    
    
}




void Foliation::findNextSepSegment(std::vector<SeparatrixSegment>& currentSepSegments, int index){
    assert(currentSepSegments[index].m_smallContainingInterval != CONTAINING_INTERVAL_NOT_UNIQUE);
    
    currentSepSegments[index].m_intervalIntersectionCount[containingInterval(m_topRealDivPoints, currentSepSegments[index].m_endpoint)]++;
    currentSepSegments[index].m_arcsAroundDivPoints.InsertPoint(currentSepSegments[index].m_endpoint, currentSepSegments[index].m_smallContainingInterval);
    currentSepSegments[index].m_depth++;
    currentSepSegments[index].m_endpoint = currentSepSegments[index].m_isGoingUp ? m_twistedIntervalExchange.applyTo(currentSepSegments[index].m_endpoint) : m_twistedIntervalExchange.applyInverseTo(currentSepSegments[index].m_endpoint);
    currentSepSegments[index].m_smallContainingInterval = containingInterval(m_allRealDivPoints, currentSepSegments[index].m_endpoint);
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






//---------------------//
// FoliationFromSphere //
//---------------------//


FoliationFromSphere::FoliationFromSphere(const FoliationSphere& foliationSphere) :
    Foliation(foliationSphere)
{
}




