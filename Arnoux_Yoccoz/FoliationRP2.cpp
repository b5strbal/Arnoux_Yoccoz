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



FoliationDisk::FoliationDisk(const WeighedTree& wt):
    m_intervalPairing(wt),
    m_weighedTree(wt)
{
}





std::ostream& operator<<(std::ostream& Out, const FoliationDisk& fd){
    using namespace std;

    Out << fd.m_intervalPairing << endl;
    
    std::vector<int> singularityType = fd.m_weighedTree.getDegrees();
    
    Out << "Singularity type: " << singularityType;

    return Out;
}







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


FoliationRP2 arnouxYoccozRP2(){
    floating_point_type alpha = 1/arnouxYoccozStretchFactor(3);
    std::vector<floating_point_type> weighedTreeInput = { alpha + pow(alpha,2), pow(alpha,2) + pow(alpha, 3), pow(alpha, 3) + alpha };
    
    return FoliationRP2(WeighedTree(weighedTreeInput));
}





FoliationSphere::FoliationSphere(const FoliationDisk& topFoliation, const FoliationDisk& bottomFoliation, floating_point_type twist) :
    m_topFoliation(topFoliation),
    m_bottomFoliation(bottomFoliation),
    m_twist(twist)
{
}


FoliationSphere::FoliationSphere(const FoliationRP2& foliationRP2) :
    FoliationSphere(foliationRP2.m_foliationDisk, foliationRP2.m_foliationDisk, 0.5)
{
}



std::ostream& operator<<(std::ostream& Out, const FoliationSphere& f){
    Out << "(Top Disk) " << f.topFoliation() << std::endl;
    Out << "(Bottom Disk) " << f.bottomFoliation() << std::endl;
    Out << "Twist: " << f.twist();
    
    return Out;
}




