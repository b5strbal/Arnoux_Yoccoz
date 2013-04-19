#ifndef FOLIATIONRP2_H
#define FOLIATIONRP2_H

/*******************************************************************************
 *  CLASS NAME:	FoliationDisk
 *  AUTHOR:		Balazs Strenner
 *				Copyright 2013 UW-Madison. All rights reserved.
 *  DATE:		1/8/13
 *
 *	DESCRIPTION: Measured foliation on the disk, transverse to the boundary.
 
    MORE DETAILS: If the number of prongs of the singularities are n_1, n_2, etc, then by the Euler-Poincare formula,
                sum(n_i - 2) = -2. In particular, the number of prongs are always even. We are interseted in creating
                measured foliations on RP^2 or on the sphere from foliations on the disk that are stable or unstable
                foliations of pseudo-anosovs, therefore we assume that there are no saddle connections. In this case
                all separatrices intersect the boundary. We call those points DIVISION POINTS (m_DivPoints). 
                Because of the above, there is always even number of them.   

                The division points divide the boundary circle (in short: Circle) into same number of intervals 
                (m_Intervals). These Intervals are naturally paired up by leaves of the foliation (m_Pair).
                The lengths of the Intervals in pairs have the same length (m_Lengths).
 
                Conversely, given a list of even number of positive numbers, and a pairing of them such that pairs
                have the same length and such that the parity of the indices of the pairs are different (e.g. the 1st
                and 3rd Intervals can't be paired up by the foliation because then the 2nd one does not have a pair), then
                up to Whitehead equivalence, there exists a unique measured foliation with that data. (Unique
                to isotopy among the ones with no saddle connections.)
 
                We assume that the boundary has length 1, so sum(m_Lenghts[i]) = 1. 
 
                We use the CirclePoint class to represent the division points. The first division point is always 0.
 
 *				
 *
 ******************************************************************************/

/*
*          Imagine that the edges of the tree are made just a little thicker, and air is pumped into the inside of the tree. Since it is a tree,
*          it expands to a disk. Assume that the boundary doesn't stretch, so each side of each edge takes up as long part of the boundary
*          circle as it's weighed. The circumference of the circle will be twice the sum of the weights of the edges.
*          There is a unique measured foliation up to Whitehead equivalence that pairs up the segments of the boundary circle that come from the same edge.\n\n";
cout << "Each vertex of the tree corresponds to a singularity of the foliation, the degree of the vertex and number of prongs being the same. In particular, leafs correspond to 1-pronged singularities, and we may assume that there are no degree 2-vertices, and that there is at least one vertex of degree at least 3.\n";
*/

//#include <iostream>
//#include <vector>
//#include <cassert>
//#include "IntervalExchangeMap.h"
//#include "PerronFrobenius.h"
#include "FoliationDisk.h"

//const int THE_POINT_IS_A_DIVPOINT = -1;







class FoliationRP2
{
    friend class FoliationSphere;
    
public:
    FoliationRP2(const FoliationDisk&);
    static FoliationRP2 arnouxYoccozRP2();

    //void Rotate();
    //void Reflect();
    
    friend std::ostream& operator<<(std::ostream& Out, const FoliationRP2&);
    
    
private:
    FoliationDisk m_foliationDisk;
    

};






#endif
