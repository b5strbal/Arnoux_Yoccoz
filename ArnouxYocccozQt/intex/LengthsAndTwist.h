#ifndef LENGTHSANDTWIST_H
#define LENGTHSANDTWIST_H

#include <vector>
#include "../global.h"

namespace balazs{


class LengthsAndTwist
{
public:
    LengthsAndTwist(const std::vector<floating_point_type>& lengths, floating_point_type twist);
    const std::vector<floating_point_type>& lengths() const { return m_lengths; }
    const floating_point_type& twist() const { return m_twist; }
private:
    std::vector<floating_point_type> m_lengths;
    floating_point_type m_twist;
};

}


#endif // LENGHTSANDTWIST_H
