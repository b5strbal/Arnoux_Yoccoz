

#include "ArcsAroundDivPoints.h"




void Foliation::ArcsAroundDivPoints::InsertPoint(const UnitIntervalPoint& NewCuttingPoint, int IndexOfInterval){
    switch (m_CuttingPoints[IndexOfInterval].size()) {
        case 0:
            m_CuttingPoints[IndexOfInterval].push_back(NewCuttingPoint);
            break;
            
        case 1:
            if (m_CuttingPoints[IndexOfInterval][0] < NewCuttingPoint){
                m_CuttingPoints[IndexOfInterval].push_back(NewCuttingPoint);
            } else {
                m_CuttingPoints[IndexOfInterval].insert(m_CuttingPoints[IndexOfInterval].begin(), NewCuttingPoint);
            }
            break;
            
        case 2:
            if (NewCuttingPoint < m_CuttingPoints[IndexOfInterval][0]) {
                m_CuttingPoints[IndexOfInterval][0] = NewCuttingPoint;
            } else if (m_CuttingPoints[IndexOfInterval][1] < NewCuttingPoint){
                m_CuttingPoints[IndexOfInterval][1] = NewCuttingPoint;
            }
    }
}





bool Foliation::ArcsAroundDivPoints::ContainsQ(const UnitIntervalPoint& c, int IndexOfInterval) const{
    if (m_CuttingPoints[IndexOfInterval].size() == 2 && m_CuttingPoints[IndexOfInterval][0] < c && c < m_CuttingPoints[IndexOfInterval][1])
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
    if (m_CuttingPoints[LeftIndexOfInterval].size() > 0 && LeftEndPoint < m_CuttingPoints[LeftIndexOfInterval].back()) {
        return false;
    }
    if (m_CuttingPoints[RightIndexOfInterval].size() > 0 && m_CuttingPoints[RightIndexOfInterval].front() < RightEndPoint) {
        return false;
    }
    int i;
    for (i = LeftIndexOfInterval + 1; i != RightIndexOfInterval; i = (i + 1) % m_Foliation->m_numSeparatrices) {
        if (m_CuttingPoints[i].empty()) {
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
    assert(adp1.m_Foliation == adp2.m_Foliation);
    
    ArcsAroundDivPoints adp = adp1;
    for (int i = 0; i < adp1.m_Foliation->m_numSeparatrices; i++) {
        if (adp1.m_CuttingPoints[i].empty()) {
            adp.m_CuttingPoints[i] = adp2.m_CuttingPoints[i];
        } else if (adp2.m_CuttingPoints[i].empty()){
        } else {
            adp.m_CuttingPoints[i].resize(2); 
            adp.m_CuttingPoints[i][0] = std::min(adp1.m_CuttingPoints[i].front(), adp2.m_CuttingPoints[i].front());
            adp.m_CuttingPoints[i][1] = std::max(adp1.m_CuttingPoints[i].back(), adp2.m_CuttingPoints[i].back());
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




 









