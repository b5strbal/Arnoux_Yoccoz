#ifndef LENGTHSANDTWIST_H
#define LENGTHSANDTWIST_H

#include <vector>

namespace balazs{


class LengthsAndTwist
{
public:
    LengthsAndTwist(const std::vector<long double>& lengths, long double twist);
    LengthsAndTwist(const LengthsAndTwist&) = delete;
    LengthsAndTwist& operator=(const LengthsAndTwist&) = delete;

    const std::vector<long double>& lengths() const { return m_lengths; }
    const long double& twist() const { return m_twist; }
private:
    std::vector<long double> m_lengths;
    long double m_twist;
};

}


#endif // LENGHTSANDTWIST_H
