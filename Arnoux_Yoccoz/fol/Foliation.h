#ifndef __Arnoux_Yoccoz__Foliation__
#define __Arnoux_Yoccoz__Foliation__

#include "../intex/TwistedIntervalExchangeMap.h"


namespace balazs{


/*!
 * \brief   An orientable foliation on an orientable surface.
 * \author  Balazs Strenner, strenner@math.wisc.edu
 * \date    April 28, 2013
 *
 */
class Foliation{
public:
    Foliation(const std::vector<floating_point_type>& lengths,
              const Permutation& permutation,
              floating_point_type twist);
    virtual ~Foliation() {}

    
    std::size_t numSeparatrices() const { return 2 * numIntervals(); }
    std::size_t numIntervals() const { return m_twistedIntervalExchange.size(); }

    const TwistedIntervalExchangeMap& intExchange() const;
    const std::vector<Mod1NumberIntExchange>& allDivPoints() const;
    const std::vector<Mod1NumberIntExchange>& topDivPoints() const;
    const std::vector<Mod1NumberIntExchange>& bottomDivPoints() const;
    bool isTopDivPoint(int divPointIndex) const;
    const Mod1NumberIntExchange& firstIntersection(int singularityIndex, Direction::UpOrDown direction) const;

    friend std::ostream& operator<<(std::ostream& out, const Foliation& f);


private:
    Foliation(const TwistedIntervalExchangeMap&);

    Foliation rotateBy(int rotationAmount) const;
    Foliation reflect() const;
    Foliation flipOver() const;

protected:
    TwistedIntervalExchangeMap m_twistedIntervalExchange;
    std::vector<Mod1NumberIntExchange> m_allDivPoints;
    std::vector<short> m_isTopDivPoint;
    //std::vector<int> m_pairOfTopDivPoints;
};




Foliation arnouxYoccozFoliation(int genus);


}



#endif /* defined(__Arnoux_Yoccoz__Foliation__) */
