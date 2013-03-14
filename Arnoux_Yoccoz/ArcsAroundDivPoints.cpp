

#include "ArcsAroundDivPoints.h"


ArcsAroundDivPoints::ArcsAroundDivPoints(const std::vector<CirclePoint>& DivPoints) : m_DivPoints(DivPoints){
    for (int i = 0; i < DivPoints.size(); i++) {
        assert(DivPoints[i].GetSide() == CENTER);
    }
    assert(DivPoints.size() > 0);
}




void ArcsAroundDivPoints::InsertPoint(const CirclePoint& NewCuttingPoint){
    if (IsEmpty()) {
        for (int i = 0; i < m_DivPoints.size(); i++) {
            assert(m_DivPoints[i] != NewCuttingPoint);
        }
        m_Arcs.resize(m_DivPoints.size(), Arc(NewCuttingPoint, NewCuttingPoint));
        
    } else { 
        for (int i = 0; i < m_DivPoints.size(); i++) {
            // In this case we don't check if the NewCuttingPoint is different from the division points, because Arc::CutOff will do the assertion.
            
            m_Arcs[i].CutOff(m_DivPoints[i], NewCuttingPoint);
        }
    }
}





bool ArcsAroundDivPoints::ContainsQ(const CirclePoint& c) const{
    if (IsEmpty()) {    // For an empty object, there is the whole Circle around each point, therefore it contains everything.
        return true;
    }
    for (int i = 0; i < m_DivPoints.size(); i++) {
        if (m_Arcs[i].ContainsQ(c)) {
            return true;
        }
    }
    return false; 
}








bool ArcsAroundDivPoints::ContainsArcThroughADivPointQ(const Arc& arc) const{
    for (int i = 0; i < m_DivPoints.size(); i++) {
        if (arc.ContainsQ(m_DivPoints[i]))
            return (arc.ContainsQ(m_Arcs[i].GetLeftEndpoint()) || arc.ContainsQ(m_Arcs[i].GetRightEndpoint())) ? false : true;
    }
    return false;
 }










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






ArcsAroundDivPoints Intersect(const ArcsAroundDivPoints& adp1, const ArcsAroundDivPoints& adp2)
{
    assert(adp1.m_DivPoints == adp2.m_DivPoints);
     
    if (adp1.IsEmpty()) { // if one object is empty, it does not add new intersection point, so we return the other object.
        return adp2;
    }
    if (adp2.IsEmpty()) {
        return adp1;
    }
    const std::vector<CirclePoint>& DivPoints = adp1.m_DivPoints;
    ArcsAroundDivPoints adp = adp1;
    for (int i = 0; i < DivPoints.size(); i++) {
        adp.m_Arcs[i].CutOff(DivPoints[i], adp2.m_Arcs[i].GetLeftEndpoint());
        adp.m_Arcs[i].CutOff(DivPoints[i], adp2.m_Arcs[i].GetRightEndpoint());
    }
    
    return adp;
}
    













/*
 bool ArcsAroundDivPoints::ContainsADivPoint(const Arc& arc) const{
 for (int i = 0; i < m_DivPoints.size(); i++) {
 if (arc.ContainsQ(m_DivPoints[i])) {
 return true;
 }
 }
 return false;
 }
 
 
 
 
 
 bool ArcsAroundDivPoints::ContainsArcQ(const Arc& arc) const{
 for (int i = 0; i < m_DivPoints.size(); i++) {
 if (arc.ContainsQ(m_Arcs[i].GetLeftEndpoint()) || arc.ContainsQ(m_Arcs[i].GetRightEndpoint())) {
 return false;
 }
 }
 for (int i = 0; i < m_DivPoints.size(); i++) {
 if (m_Arcs[i].ContainsQ(arc.GetRightEndpoint())) {
 return true;
 }
 }
 return false;
 }
 */








