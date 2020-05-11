#include "simpleTuple.hpp"

//##    SIMPLETUPLE_METHODS   ##
template <class T1, class T2>
simpleTuple<T1,T2>::simpleTuple(T1 K, T2 V){
    key = K;
    value = V;
}

template <class T1, class T2>
void simpleTuple<T1,T2>::setVal(T1 K, T2 V){
    key = K;
    value = V;
  }

template <class T1, class T2>
const T1 simpleTuple<T1,T2>::getKey() const{
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
  if(this->key == tuple2.getKey() && this->value == tuple2.getVal()){
    return true;
  }
  else return false;
}

template<typename T>
void homoTuple<T>::operator=(const simpleTuple<T,T> &parentTuple){
  this->key = parentTuple.getKey();
  this->value = parentTuple.getVal();
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


//LEARNT+QUESTIONS BELOW//
 /*[QUESTION_Answered] - This gives error without the 'this->'? Why?
                            Answer - It is because unqualified lookup doesn't work here because the base is a template.
                                      So, we need to use this-> to access inherited members in childs of template classes*/
/*[LEARNT] - Direct access through objects of Base Class, to protected members of Base Class,
                                                          is only in methods of the Base class, not even in Child
                                                          For. eg, you take an object of parent type (not 'this' object), then can't access in Child*/
  //DOWNSIDE_C++, QUESTION - We can't have virtual templated member function
  //LEARNT - In a templated function, if return types in if and blocks is different, then It wont be able to know the type
/*[LEARNT] - In C++, protected members can't be accessed directly
                         In Java, protected members 'can' be accessed directly, but in same package*/
/*NOTE, QUESTION - Did this to suppress this - In instantiation of ‘homoTuple<T>::homoTuple(T, T) [with T = int]’:
./util/terminalDimensions.hpp:17:24:   required from here
./util/./simpleTuple.hpp:70:30: error: no matching function for call to ‘simpleTuple<int, int>::simpleTuple()’
     homoTuple(T key, T value){ */