#pragma once

#include <vector>

namespace util
{

    /*@brief Returns true if `num` can be written as a sum of some elements of the vector v; Else returns false*/
    template <typename T = int>
    bool isSum(T num, const std::vector<T> &v);
    template <typename T = int>
    std::vector<T> isSumOfElements(T num, const std::vector<T> &v);

} // namespace util

    //DEFINITIONS
template <typename T>
bool util::isSum(T num, const std::vector<T> &v)
{
    auto total = 1 << v.size();
    T sum;

    for (auto i = 0; i < total; ++i)
    {
        sum = 0;
        for (auto j = 0; j < v.size(); ++j)
        {
            if (i & 1 << j)
            { //Cheching if the jth bit is set
                sum += v[j];
            }
        }
        if (num == sum)
            return true;
    }
    return false;
}

template <typename T>
std::vector<T> util::isSumOfElements(T num, const std::vector<T> &v)
{
    auto sum = 0;
    auto total = 1 << v.size();
    std::vector<T> elements;

    for (int i = 0; i < total; ++i)
    {
        for (size_t j = 0; j < v.size(); ++j)
        {
            if (i & 1 << j)
            { //Cheching if the jth bit is set
                elements.push_back(v[j]);
                sum += v[j];
            }
        }
        if (num == sum)
            return elements;
        sum = 0;
        elements.clear();
    }
    return {};
}
