#pragma once

#include <set>
#include <vector>
#include <list>
#include <algorithm>

namespace util
{
    /*NOTE - Read respective details about the function and what it does in respective function definitions*/

    /*CAUTION - Unless a container argument is `const`, the algorithm will be MODIFIED*/
    // template <typename T>
    // size_t remove_common(std::_Tree<T> &container, const std::_Tree<T> &duplicateValues);
    // template <typename T>
    // size_t take_common(std::_Tree<T> &container, const std::_Tree<T> &duplicateValues);
    // template <typename T>
    // size_t remove_distinct(std::_Tree<T> &container, const std::_Tree<T> &duplicateValues);
    template <typename T>
    size_t remove_common(std::set<T> &container, const std::set<T> &duplicateValues);
    template <typename T>
    void remove_common(std::vector<T> &, const std::vector<T> &);
    template <typename T>
    size_t take_common(std::set<T> &container, const std::set<T> &duplicateValues);
    template <typename T>
    size_t remove_distinct(std::set<T> &container, const std::set<T> &duplicateValues);

    //contains... for < C++20... these are there in C++20, and implemented in all 4 major compilers
    template <typename T = int>
    bool contains(const std::set<T> &C, const T &val); //for sets, maps

    // bool contains(const std::_Tree<T> &C, const T &val); //for sets, maps
    // template <typename T>
    template <typename T = int>
    bool contains(const std::vector<T> &C, const T &val);
    template <typename T = int>
    bool contains(const std::list<T> &C, const T &val);
    //contains... for < C++20

} // namespace util

template <typename T>
size_t util::remove_common(std::set<T> &container, const std::set<T> &duplicateValues)
{
    /*@brief - Removes all common elements between 2 sets passed... FROM THE ORIGINAL SET (Notice the const in the 2nd set, we won't modify it)

    @params - container -> the set from which to remove the common elements
              duplicateValues -> set containing any number of elements, out of which some MAYBE common with `original set`, remove these from the container one

    @complexity - O( (logN)* dupl.size() )

    @returns - Number of elements removed from container

    @example - original -> {1,2,3,4,5,6,7}
                duplicateValues -> {4,1,9}

                Then, 4 and 1 will be removed from original (9 will be also passed to erase, but since it isn't in original, so no effect of that call)
                so finally, original -> {  2,3,  5,6,7,8}
    */

    size_t removedCount = 0U;
    for (auto &val : duplicateValues)         //remove common elements
        removedCount += container.erase(val); //_Tree::erase() returns number of elements removed

    return removedCount;
}

template <typename T>
void util::remove_common(std::vector<T> &first, const std::vector<T> &second){
    std::for_each(second.crbegin(), second.crend(), [&](T i) {
        first.erase(std::find(first.begin(), first.end(), i));
    });
}

template <typename T>
size_t util::take_common(std::set<T> &container, const std::set<T> &duplicateValues)
{
    /*@brief - An alias for remove_distinct

    @complexity- O()
    @note - for details refer util::remove_common() definition
    */

    return util::remove_distinct(container, duplicateValues);
}

template <typename T>
size_t util::remove_distinct(std::set<T> &container, const std::set<T> &duplicateValues)
{
    /*@brief - Removes all DISTINCT elements between 2 sets passed... FROM THE ORIGINAL SET (Notice the const in the 2nd set, we won't modify it)
            So, only the

    @params - container -> the set from which to remove the distinct elements
              duplicateValues -> set containing any number of elements, having values NOT IN `original set`, remove these from the container one

    @complexity - O( (logN)* container.size() )

    @returns - Number of elements removed from original

    @example - original -> {1,2,3,4,5,6,7}
                duplicateValues -> {4,1,9}

                Then, 4 and 1 will be removed from original (9 will be also passed to erase, but since it isn't in original, so no effect of that call)
                so finally, original -> {  2,3,  5,6,7,8}
    */

    //CAUTION - @TODO Check if iterator invalidation may happen when we erase elements from a tree during for range loop
    size_t removedCount = 0U;
    for (auto &&val : container) //remove distinct elements
        if (!util::contains(duplicateValues, val))
            removedCount += container.erase(val); //_Tree::erase() returns number of elements removed

    return removedCount;
}

template <typename T>
bool util::contains(const std::set<T> &C, const T &val) //for sets, maps
{
    /*
        @note- std::_Tree is the parent class for std::set, std::multiset, std::unordered_set, std::map, std::multimap, std::unordered_map
        @brief- Returns whether an element exists in a given container

        @complexity- O(logN) average (worst O(size()) for unordered)
        @returns bool indicating element present in container or not
        */

    return C.find(val) != C.end();
}

/*
The compiler failed to deduce the argument
'bool util::contains(const std::set<T,std::less<_Ty>,std::allocator<_Ty>> &,const T &)': could not deduce template argument for 'const std::set<T,std::less<_Ty>,std::allocator<_Ty>> &' from 'std::vector<_dieVal,std::allocator<_dieVal>>'
*/

template <typename T>
bool util::contains(const std::vector<T> &C, const T &val)
{
    /*
    @brief- Returns whether an element exists in a given container

    @complexity- O(N)
    @returns bool indicating element present in container or not
    */

    return std::find(C.begin(), C.end(), val) != C.end();
}

template <typename T>
bool util::contains(const std::list<T> &C, const T &val)
{
    /*
    @brief- Returns whether an element exists in a given container

    @complexity- O(N)
    @returns bool indicating element present in container or not
    */

    return std::find(C.begin(), C.end(), val) != C.end();
}
