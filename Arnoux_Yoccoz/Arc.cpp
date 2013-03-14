

#include "Arc.h"


void Arc::CutOff(const CirclePoint& MarkingPoint, const CirclePoint& CuttingPoint){
    assert(ContainsQ(MarkingPoint));
    assert(MarkingPoint != CuttingPoint);
    assert(m_LeftEndpoint != m_RightEndpoint || MarkingPoint != m_LeftEndpoint);
    
    if(ContainsQ(CuttingPoint)){ 
        if (IsBetween(m_LeftEndpoint, MarkingPoint, CuttingPoint)) { // If CuttingPoint is on the left side of MarkingPoint
            m_LeftEndpoint = CuttingPoint;
        }
        else {                                                      // If CuttingPoint is on the right side of MarkingPoint
            m_RightEndpoint = CuttingPoint;
        }
    }
}




