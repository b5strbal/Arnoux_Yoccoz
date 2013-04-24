#ifndef TWISTEDINTERVALEXCHANGEMAP_H
#define TWISTEDINTERVALEXCHANGEMAP_H

#include "Mod1Number.h"
#include "Mod1NumberIntExchange.h"
#include "IntervalExchangeBase.h"

class TwistedIntervalExchangeMap : public IntervalExchangeBase{
   // friend class Foliation;
   // friend class FoliationFromRP2;
public:
    TwistedIntervalExchangeMap(); // default constructor: the identity interval exchange on one interval.
    TwistedIntervalExchangeMap(const std::vector<floating_point_type>& lengths,
                        const Permutation& permutation,
                        floating_point_type twist);


    virtual Mod1Number applyTo(const Mod1Number& point) const;
    virtual Mod1NumberIntExchange applyTo(const Mod1NumberIntExchange &point) const;
    virtual Mod1Number applyInverseTo(const Mod1Number& point) const;
    virtual Mod1NumberIntExchange applyInverseTo(const Mod1NumberIntExchange &point) const;
    //const std::vector<Mod1NumberIntExchange>& translations() const { return m_translations; }

    TwistedIntervalExchangeMap rotateBy(int rotationAmount) const;
    TwistedIntervalExchangeMap reverse() const;
    TwistedIntervalExchangeMap invert() const;
private:
    std::vector<Mod1NumberIntExchange> m_translations;
};

#endif // TWISTEDINTERVALEXCHANGEMAP_H
