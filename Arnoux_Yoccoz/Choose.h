#ifndef CHOOSE_H
#define CHOOSE_H

#include <vector>

class Choose{
public:
    Choose(int n, int k);
    Choose& operator++();
    inline bool isAfterLast() const { return m_chosenIndices.empty(); }
    operator const std::vector<int>&(){ return m_chosenIndices; }

private:
    int m_n;
    int m_k;
    std::vector<int> m_chosenIndices;
};

#endif // CHOOSE_H
