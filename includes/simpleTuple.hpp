#pragma once

#include "exceptions.hpp"
#include <functional>

template <class T1, class T2>
class simpleTuple{
protected:
  // T1 first;
  // T2 second;

public:
  T1 first;
  T2 second;
  void setVal(T1 K, T2 V);
  const T1 getfirst() const;
  const T2 getVal() const;

  void operator=(const simpleTuple<T1,T2>&);
  bool operator==(const simpleTuple<T1,T2>&) const;
  bool operator!=(const simpleTuple<T1,T2>&) const;

  simpleTuple(T1 K, T2 V);
  simpleTuple(){} //!

};

template<class T>
class homoTuple : public simpleTuple<T,T>{
  public:
    typedef simpleTuple<T,T> super;

    T&   operator[](unsigned int); //! Whenever const access is required, use the overloaded methods, else use the operator
    void operator=(const simpleTuple<T,T>&);
    void operator=(const std::pair<int,int>&);
    bool operator<(const homoTuple<T>&) const;
    bool operator==(const homoTuple<T>&) const;
    bool operator==(const simpleTuple<T,T>&) const;
    bool operator!=(const homoTuple<T>&) const;
    bool operator!=(const simpleTuple<T,T>&) const;

    homoTuple(T first, T second) : super(first,second){}
    homoTuple(){};
};

//##    TYPEDEF    ##
typedef homoTuple<int> intTuple;
typedef homoTuple<char> charTuple;
typedef homoTuple<float> floatTuple;
typedef homoTuple<double> doubleTuple;
//##  / TYPEDEF    ##

//##  DEFINTIONS  ##//
//##    SIMPLETUPLE_METHODS   ##
template <class T1, class T2>
simpleTuple<T1,T2>::simpleTuple(T1 K, T2 V){
    first = K;
    second = V;
}

template <class T1, class T2>
void simpleTuple<T1,T2>::setVal(T1 K, T2 V){
    first = K;
    second = V;
  }

template <class T1, class T2>
const T1 simpleTuple<T1,T2>::getfirst() const{
    return first;
}

template <class T1, class T2>
const T2 simpleTuple<T1,T2>::getVal() const{
    return second;
}
//##    SIMPLETUPLE_METHODS   ##

//##    HOMOTUPLE_METHODS   ##
template<typename T>
bool homoTuple<T>::operator!=(const homoTuple<T>& tuple2) const{
  if( this->operator==(tuple2) == false){
    return true;
  }
  else return false;
}

template<typename T>
bool homoTuple<T>::operator==(const homoTuple<T>& tuple2) const{
  if(this->first == tuple2.first && this->second == tuple2.second){
    return true;
  }
  else return false;
}

template<typename T>
bool homoTuple<T>::operator!=(const simpleTuple<T,T>& tuple2) const{
  if( this->operator==(tuple2) == false){
    return true;
  }
  else return false;
}

template<typename T>
bool homoTuple<T>::operator==(const simpleTuple<T,T>& tuple2) const{
  if(this->first == tuple2.getfirst() && this->second == tuple2.getVal()){
    return true;
  }
  else return false;
}

template<typename T>
void homoTuple<T>::operator=(const simpleTuple<T,T> &parentTuple){
  this->first = parentTuple.getfirst();
  this->second = parentTuple.getVal();
}

template<typename T>
void homoTuple<T>::operator=(const std::pair<int,int> &parentTuple){
  this->first = parentTuple.first;
  this->second = parentTuple.second;
}

template<typename T>
bool homoTuple<T>::operator<(const homoTuple<T> &t2) const{

  if(this->first < t2.first){
    return true;
  }else if(this->second < t2.second){
    return true;
  }
  else return false;

}

template<typename T>
T& homoTuple<T>::operator[](unsigned int index){
  if(index == 0){
    return this->first;
    }
  else if(index == 1){
    return this->second;
  }
  else{
    throw OutOfBoundException("Tuple");
  }
}
//##  / HOMOTUPLE_METHODS   ##
//##  DEFINTIONS  ##//