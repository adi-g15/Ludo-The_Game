#ifndef CUSTOM_EXCEPTION
  #include "./exceptions.hpp"
#endif

template <class T1, class T2>
class simpleTuple{
protected:  /*[LEARNT] - In C++, protected members can't be accessed directly
                         In Java, protected members 'can' be accessed directly, but in same package*/
  T1 key;
  T2 value;

public:
  /*NOTE, QUESTION - Did this to suppress this - In instantiation of ‘homoTuple<T>::homoTuple(T, T) [with T = int]’:
./util/terminalDimensions.hpp:17:24:   required from here
./util/./simpleTuple.hpp:70:30: error: no matching function for call to ‘simpleTuple<int, int>::simpleTuple()’
     homoTuple(T key, T value){ */

  simpleTuple(T1 K, T2 V){
    key = K;
    value = V;
  }

  void setVal(T1 K, T2 V){
    key = K;
    value = V;
  }

  const T1 getKey() const{
    return key;
  }
  const T2 getVal() const{
    return value;
  }

  //DOWNSIDE_C++, QUESTION - We can't have virtual templated member function
  //LEARNT - In a templated function, if return types in if and blocks is different, then It wont be able to know the type

};

template<class T>
class homoTuple : public simpleTuple<T,T>{
  public:

    typedef simpleTuple<T,T> super;

//Whereever const access is required, use the overloaded methods, else use the operator
    T&   operator[](unsigned int index);  
    void operator=(const simpleTuple<T,T>&);
    bool operator==(const homoTuple<T>& tuple2) const;
    bool operator==(const simpleTuple<T,T>& tuple2) const;
    bool operator!=(const homoTuple<T>& tuple2) const;
    bool operator!=(const simpleTuple<T,T>& tuple2) const;

    homoTuple(T key, T value) : super(key,value){}
};

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
  if(this->x_coords == tuple2.x_coords && this->y_coords == tuple2.y_coords){
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
  if(this->x_coords == tuple2.getKey() && this->y_coords == tuple2.getVal()){
    return true;
  }
  else return false;
}

template<typename T>
void homoTuple<T>::operator=(const simpleTuple<T,T> &parentTuple){  /*[LEARNT] - Direct access through objects of Base Class, to protected members of Base Class,
                                                          is only in methods of the Base class, not even in Child
                                                          For. eg, you take an object of parent type (not 'this' object), then can't access in Child*/ 
  this->key = parentTuple.getKey();
  this->value = parentTuple.getVal();
}

template<typename T>
T& homoTuple<T>::operator[](unsigned int index){
  if(index == 0){
    return this->key; /*[QUESTION_Answered] - This gives error without the 'this->'? Why?
                            Answer - It is because unqualified lookup doesn't work here because the base is a template.
                                      So, we need to use this-> to access inherited members in childs of template classes*/
  }
  else if(index == 1){
    return this->value;
  }
  else{
    throw OutOfBoundException("Tuple");
  }
}
//##  / HOMOTUPLE_METHODS   ##

//##    TYPEDEF    ##
typedef homoTuple<int> intTuple;
typedef homoTuple<char> charTuple;
typedef homoTuple<float> floatTuple;
typedef homoTuple<double> doubleTuple;
//##  / TYPEDEF    ##
