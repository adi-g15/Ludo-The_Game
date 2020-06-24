#pragma once

/*[DEPRECATED] USING std::pair INSTEAD OF THIS*/

#include "exceptions.h"
#include <functional>

template <class T1, class T2>
class simpleTuple{
protected:
  // T1 key;
  // T2 value;

public:
  T1 key;
  T2 value;
  void setVal(T1 K, T2 V);
  const T1 getkey() const;
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

    homoTuple(T key, T value) : super(key,value){}
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
simpleTuple<T1,T2>::simpleTuple(T1 K, T2 V) : key(K), value(V){}

template <class T1, class T2>
void simpleTuple<T1,T2>::setVal(T1 K, T2 V){
    key = K;
    value = V;
  }

template <class T1, class T2>
const T1 simpleTuple<T1,T2>::getkey() const{
    return key;
}

template <class T1, class T2>
const T2 simpleTuple<T1,T2>::getVal() const{
    return value;
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
  if(this->key == tuple2.key && this->value == tuple2.value){
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
  if(this->key == tuple2.getkey() && this->value == tuple2.getVal()){
    return true;
  }
  else return false;
}

template<typename T>
void homoTuple<T>::operator=(const simpleTuple<T,T> &parentTuple){
  this->key = parentTuple.getkey();
  this->value = parentTuple.getVal();
}

template<typename T>
void homoTuple<T>::operator=(const std::pair<int,int> &parentTuple){
  this->key = parentTuple.key;
  this->value = parentTuple.value;
}

template<typename T>
bool homoTuple<T>::operator<(const homoTuple<T> &t2) const{

  if(this->key < t2.key){
    return true;
  }else if(this->value < t2.value){
    return true;
  }
  else return false;

}

template<typename T>
T& homoTuple<T>::operator[](unsigned int index){
  if(index == 0){
    return this->key;
    }
  else if(index == 1){
    return this->value;
  }
  else{
    throw OutOfBoundException("Tuple");
  }
}
//##  / HOMOTUPLE_METHODS   ##
//##  DEFINTIONS  ##//