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
    std::size_t numSingularities() const { return m_singularities.size(); }
    std::size_t genus() const { return 1 - eulerChar() / 2; }
    int eulerChar() const;
    std::vector<std::size_t> singularityTypeOfAbelianDiff() const;
    std::vector<std::size_t> singularityTypeProngs() const;

    const TwistedIntervalExchangeMap& intExchange() const { return m_twistedIntervalExchange; }
    const std::vector<Mod1NumberIntExchange>& allDivPoints() const { return m_allDivPoints; }
    const std::vector<Mod1NumberIntExchange>& topDivPoints() const { return m_twistedIntervalExchange.divPoints(); }
    const std::vector<Mod1NumberIntExchange>& bottomDivPoints() const
        { return m_twistedIntervalExchange.divPointsAfterExchange(); }
    bool isTopDivPoint(int divPointIndex) const;
    const std::vector<std::size_t>& indexOfSingularity() const { return m_indexOfSingularity; }
    const Mod1NumberIntExchange& firstIntersection(int singularityIndex, Direction::UpOrDown direction) const;

    Foliation rotateBy(int rotationAmount) const;
    Foliation reflect() const;
    Foliation flipOver() const;

    friend std::ostream& operator<<(std::ostream& out, const Foliation& f);


private:
    Foliation(const TwistedIntervalExchangeMap&);


    void initSingularities();

private:
    TwistedIntervalExchangeMap m_twistedIntervalExchange;
    std::vector<Mod1NumberIntExchange> m_allDivPoints;
    std::vector<short> m_isTopDivPoint;

    std::vector<std::vector<std::size_t>> m_singularities;
    std::vector<std::size_t> m_indexOfSingularity;
};




Foliation arnouxYoccozFoliation(int genus);


}



#endif /* defined(__Arnoux_Yoccoz__Foliation__) */
