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
#include "PerronFrobenius.h"






/*
 int FoliationRP2::SeparatrixIndexOfPair(int SeparatrixIndex, LeftOrRight Side){
 return Side == RIGHT ? IncreaseIndex(m_Pair[SeparatrixIndex]) : m_Pair[DecreaseIndex(SeparatrixIndex)] ;
 }
 */

FoliationRP2::FoliationRP2(const FoliationDisk& fd) :
    m_foliationDisk(fd)
{
}




std::ostream& operator<<(std::ostream& Out, const FoliationRP2& f){
    Out << f.m_foliationDisk;
    return Out;
}


FoliationRP2 FoliationRP2::arnouxYoccozRP2(){
    floating_point_type alpha = 1/arnouxYoccozStretchFactor(3);
    std::vector<floating_point_type> weighedTreeInput = { alpha + pow(alpha,2), pow(alpha,2) + pow(alpha, 3), pow(alpha, 3) + alpha };
    
    return FoliationRP2(WeighedTree(weighedTreeInput));
}







