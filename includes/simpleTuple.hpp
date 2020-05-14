#pragma once

#define CUSTOM_SIMPLETUPLE

// #ifndef CUSTOM_EXCEPTION
//   #include "./exceptions.hpp"
// #endif

template <class T1, class T2>
class simpleTuple{
protected:
  T1 key;
  T2 value;

public:
  void setVal(T1 K, T2 V);
  const T1 getKey() const;
  const T2 getVal() const;

  void operator=(const simpleTuple<T1,T2>&);
  bool operator==(const simpleTuple<T1,T2>&) const;
  bool operator!=(const simpleTuple<T1,T2>&) const;

  simpleTuple(T1 K, T2 V);
  simpleTuple(){} //Only as supporting for homoTuple<T>()

};

template<class T>
class homoTuple : public simpleTuple<T,T>{
  public:
    typedef simpleTuple<T,T> super;

    T&   operator[](unsigned int); //[NOTE] - Whenever const access is required, use the overloaded methods, else use the operator
    void operator=(const simpleTuple<T,T>&);
    bool operator<(const homoTuple<T>&) const;
    bool operator==(const homoTuple<T>&) const;
    bool operator==(const simpleTuple<T,T>&) const;
    bool operator!=(const homoTuple<T>&) const;
    bool operator!=(const simpleTuple<T,T>&) const;

    homoTuple(T key, T value) : super(key,value){}
    homoTuple(){};
    /*
      Operator '<' is ONLY to solve this - 
      In file included from /usr/include/c++/7/functional:49:0,
                 from includes/ludo_box.hpp:24,
                 from includes/board.hpp:9,
                 from includes/board.cpp:1:
/usr/include/c++/7/bits/stl_function.h: In instantiation of ‘constexpr bool std::less<_Tp>::operator()(const _Tp&, const _Tp&) const [with _Tp = homoTuple<int>]’:
/usr/include/c++/7/bits/stl_tree.h:2141:8:   required from ‘std::pair<std::_Rb_tree_node_base*, std::_Rb_tree_node_base*> std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_get_insert_hint_unique_pos(std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator, const key_type&) [with _Key = homoTuple<int>; _Val = std::pair<const homoTuple<int>, direction>; _KeyOfValue = std::_Select1st<std::pair<const homoTuple<int>, direction> >; _Compare = std::less<homoTuple<int> >; _Alloc = std::allocator<std::pair<const homoTuple<int>, direction> >; std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator = std::_Rb_tree_const_iterator<std::pair<const homoTuple<int>, direction> >; std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::key_type = homoTuple<int>]’
/usr/include/c++/7/bits/stl_tree.h:2201:33:   required from ‘std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_insert_unique_(std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator, _Arg&&, _NodeGen&) [with _Arg = const std::pair<const homoTuple<int>, direction>&; _NodeGen = std::_Rb_tree<homoTuple<int>, std::pair<const homoTuple<int>, direction>, std::_Select1st<std::pair<const homoTuple<int>, direction> >, std::less<homoTuple<int> >, std::allocator<std::pair<const homoTuple<int>, direction> > >::_Alloc_node; _Key = homoTuple<int>; _Val = std::pair<const homoTuple<int>, direction>; _KeyOfValue = std::_Select1st<std::pair<const homoTuple<int>, direction> >; _Compare = std::less<homoTuple<int> >; _Alloc = std::allocator<std::pair<const homoTuple<int>, direction> >; std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator = std::_Rb_tree_iterator<std::pair<const homoTuple<int>, direction> >; std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator = std::_Rb_tree_const_iterator<std::pair<const homoTuple<int>, direction> >]’
/usr/include/c++/7/bits/stl_tree.h:2452:21:   required from ‘void std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_insert_unique(_II, _II) [with _InputIterator = const std::pair<const homoTuple<int>, direction>*; _Key = homoTuple<int>; _Val = std::pair<const homoTuple<int>, direction>; _KeyOfValue = std::_Select1st<std::pair<const homoTuple<int>, direction> >; _Compare = std::less<homoTuple<int> >; _Alloc = std::allocator<std::pair<const homoTuple<int>, direction> >]’
/usr/include/c++/7/bits/stl_map.h:886:4:   required from ‘void std::map<_Key, _Tp, _Compare, _Alloc>::insert(_InputIterator, _InputIterator) [with _InputIterator = const std::pair<const homoTuple<int>, direction>*; _Key = homoTuple<int>; _Tp = direction; _Compare = std::less<homoTuple<int> >; _Alloc = std::allocator<std::pair<const homoTuple<int>, direction> >’
/usr/include/c++/7/bits/stl_map.h:823:15:   required from ‘void std::map<_Key, _Tp, _Compare, _Alloc>::insert(std::initializer_list<std::pair<const _Key, _Tp> >) [with _Key = homoTuple<int>; _Tp = direction; _Compare = std::less<homoTuple<int> >; _Alloc = std::allocator<std::pair<const homoTuple<int>, direction> >]’
includes/board.cpp:524:4:   required from here
/usr/include/c++/7/bits/stl_function.h:386:20: error: no match for ‘operator<’ (operand types are ‘const homoTuple<int>’ and ‘const homoTuple<int>’)
       { return __x < __y; }
    */
};

//##    TYPEDEF    ##
typedef homoTuple<int> intTuple;
typedef homoTuple<char> charTuple;
typedef homoTuple<float> floatTuple;
typedef homoTuple<double> doubleTuple;
//##  / TYPEDEF    ##
