/**
 * @file    Foliation.h
 * @author  Balazs Strenner, strenner@math.wisc.edu
 * @date    March 14, 2013
 *
 * @brief   This file contains the Foliation class.
 */

#ifndef __Arnoux_Yoccoz__Foliation__
#define __Arnoux_Yoccoz__Foliation__

#include <iostream>
#include <vector>
#include <array>
#include <list>
#include "UnitIntervalPoint.h"
#include "IntervalExchangeMap.h"
#include "PerronFrobenius.h"
#include "FoliationRP2.h"
#include "ArcsAroundDivPoints.h"

//-------------------------//
// InitArguments_Foliation //
//-------------------------//


class InitArguments_Foliation{
    friend class Foliation;
    
    struct ConnectedPoints {
        UnitIntervalPoint topPoint;
        UnitIntervalPoint bottomPoint;
    };
    
    InitArguments_Foliation() {}
    InitArguments_Foliation(const FoliationSphere& foliationSphere);
    void generateTopConnectingPairs(const FoliationSphere& foliationSphere, std::vector<ConnectedPoints>& allConnectedPoints);
    void generateBottomConnectingPairs(const FoliationSphere& foliationSphere, std::vector<ConnectedPoints>& allConnectedPoints);
    
    static std::vector<floating_point_type> arg_lengths;
    static Permutation arg_permutation;
    static floating_point_type arg_twist;
};




//-----------//
// Foliation //
//-----------//
 

class Foliation : private InitArguments_Foliation{
private:
    class ArcsAroundDivPoints;
    struct SeparatrixSegment;
    struct TransverseCurve;
    
    
    
    /**
     * @brief   A collection of arcs around a set of points on the circle of circumference 1.
     * @author  Balazs Strenner, strenner@math.wisc.edu
     * @date    March 6, 2013
     * @details Given a set of UnitIntervalPoints on the circle of circumference 1, called the "division points", an ArcAroundDivPoints object
     *          is just a collection of arcs around the division points with some properties:
     *
     *          1) There is one Arc assigned to each division point, so the number of Arcs and division points are equal.
     *
     *          2) The division points are ordinary UnitIntervalPoints.
     *
     *          3) Each division point is contained in the inside of the corresponding Arc.
     *
     *          4) It is true for any pair of Arcs in the collection that they either coincide, or touch each other only at the endpoints,
     *             or they are disjoint.
     *
     *          To better understand their structure, it might be better to understand how we are constructing them. First of all, we construct
     *          an empty object, which means that we initialize the division points, but no Arcs are assigned to them. This represents the idea
     *          that in this case the "arc" belonging to each division point is the whole circle, but since it is not really an arc, and it is
     *          not represented by the Arc class, we simply handle this situation by not assigning an Arc to the division points.
     *
     *          Then, we start to add "cutting points" to the picture. When a cutting point is added, each Arc that contains it is cut in half
     *          along it, and the smaller Arcs obtained which do not contain any division points are discarded. In other words, we look at
     *          each division point and their corresponding Arc in order, and we try to cut the Arcs in two by the cutting point and keep the
     *          Arc that contains the division point. Since the cutting points are always generalized UnitIntervalPoints, one can see that the above
     *          properties are satisfied.
     *
     *          In particular, when a cutting point is inserted in an empty object, then the whole circle has to be divided along the cutting
     *          point, the Arc of length 1, both endpoints being the cutting point will the Arc that is assigned to each division point.
     *
     * @see     UnitIntervalPoint, FoliationDisk
     */
    class ArcsAroundDivPoints{
        friend class Foliation;
    public:
        /**
         * @brief   Constructor of empty object.
         * @param ShiftedDivPoints     The division points have to be regular (not generalized) UnitIntervalPoints.
         *                      Also, there has to be at least one division point.
         */
        ArcsAroundDivPoints(int numDivPoints) :
            m_cuttingPoints(numDivPoints, std::vector<UnitIntervalPoint>(0)),
            m_numDivPoints(numDivPoints)
        {
        }
        
        
        /**
         * @brief   Inserts a new cutting point and updates the Arcs.
         * @param NewCuttingPoint   It must not coincide with any of the division points, because in that case case the operation won't be
         *                          well-defined.
         */
        void InsertPoint(const UnitIntervalPoint& NewCuttingPoint, int IndexOfInterval);
        
        /**
         * @brief   Decides if a point is contained in any of the Arcs.
         */
        bool ContainsQ(const UnitIntervalPoint& cp, int IndexOfInterval) const;
        
        
        
        /**
         * @brief   Returns true if the an Arc is contained in any of the member Arcs and it passes through the division point contained in the
         *          member Arc.
         */
        bool ContainsArcThroughADivPointQ(const UnitIntervalPoint& LeftEndPoint, int LeftIndexOfInterval,
                                          const UnitIntervalPoint& RightEndPoint, int RightIndexOfInterval) const;
        
        /**
         * @brief   Prints out the division points and all the Arcs.
         */
        //friend std::ostream& operator<<(std::ostream& Out, const ArcsAroundDivPoints& adp);
        
    private:
        /**
         * @brief   
         */
        
        int m_numDivPoints;
        std::vector<std::vector<UnitIntervalPoint>> m_cuttingPoints;
    };

    
    
    struct SeparatrixSegment{
        const Foliation& m_foliation;
        int m_startingSingularity;
        int m_depth;
        UnitIntervalPoint m_endpoint;
        int m_smallContainingInterval;
        ArcsAroundDivPoints m_arcsAroundDivPoints;
        std::vector<int> m_intervalIntersectionCount;
        bool m_isGoingUp;
        
        SeparatrixSegment(const Foliation& foliation, int startingSingularity, bool IsGoingUp);
    };
    
    
    struct TransverseCurve{
        int a;
    // change it!
    };
    
    
    
// MEMBER VARIABLES
    int m_numIntervals;
    int m_numSeparatrices;
    TwistedIntervalExchangeMap m_twistedIntervalExchange;
    std::vector<UnitIntervalPoint> m_allRealDivPoints;
    std::vector<short> m_isTopDivPoint;
    std::vector<UnitIntervalPoint> m_topRealDivPoints;
    std::vector<UnitIntervalPoint> m_bottomRealDivPoints;
    std::vector<int> m_pairOfTopDivPoints;
    
    std::vector<SeparatrixSegment> m_currentSepSegmentsUp;
    std::vector<SeparatrixSegment> m_currentSepSegmentsDown;
    std::vector<std::list<SeparatrixSegment>> m_goodShiftedSeparatrixSegmentsUp;
    std::vector<std::list<SeparatrixSegment>> m_goodShiftedSeparatrixSegmentsDown;
    std::list<TransverseCurve> m_transverseCurves;
    
    
    
    
    
    
    
public:
    Foliation(const std::vector<floating_point_type>& lengths, const Permutation& permutation, floating_point_type twist);
    Foliation(const FoliationRP2&);
    Foliation(const FoliationSphere&);
    Foliation rotateBy(int rotationAmount) const;
    Foliation reflect() const;
    Foliation flipOver() const;
    
    friend std::ostream& operator<<(std::ostream& Out, Foliation f);
    
private:
    Foliation(const TwistedIntervalExchangeMap&);
    
    void Init();
    
    
    /**
     * @brief   Takes the intersection of two ArcsAroundDivPoints objects.
     */
    ArcsAroundDivPoints Intersect(const ArcsAroundDivPoints& adp1, const ArcsAroundDivPoints& adp2);
    
    
    
    

    
};


Foliation arnouxYoccozFoliation(int genus);





//------------------//
// FoliationFromRP2 //
//------------------//


class FoliationFromRP2 : public Foliation
{
public:
    FoliationFromRP2(const FoliationRP2& foliationRP2);
};









//---------------------//
// FoliationFromSphere //
//---------------------//

class FoliationFromSphere : public Foliation
{
public:
    FoliationFromSphere(const FoliationSphere& foliationSphere);
};



#endif /* defined(__Arnoux_Yoccoz__Foliation__) */
