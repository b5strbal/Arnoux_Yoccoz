/*******************************************************************************
 *  CLASS NAME:	FoliationDisk
 *  AUTHOR:		Balazs Strenner
 *				Copyright 2013 UW-Madison. All rights reserved.
 *  DATE:		1/8/13
 *
 *	DESCRIPTION: Measured foliation on the disk, transverse to the boundary.
 *				
 *
 ******************************************************************************/

#include "FoliationRP2.h"



FoliationRP2::FoliationRP2(const WeighedTree& wt):
    m_intervalPairing(wt),
    m_weighedTree(wt)
{
}













/*
int FoliationRP2::SeparatrixIndexOfPair(int SeparatrixIndex, LeftOrRight Side){
    return Side == RIGHT ? IncreaseIndex(m_Pair[SeparatrixIndex]) : m_Pair[DecreaseIndex(SeparatrixIndex)] ;
}
*/







std::ostream& operator<<(std::ostream& Out, FoliationRP2 f){
    using namespace std;

    Out << f.m_intervalPairing << endl;
    
    std::vector<int> singularityType;
    f.m_weighedTree.getDegrees(singularityType);
    Out << "Singularity type: " << singularityType;

    return Out;
}



