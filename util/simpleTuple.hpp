#define CUSTOM_SIMPLETUPLE

#ifndef CUSTOM_EXCEPTION
  #include "./exceptions.hpp"
#endif

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

};

template<class T>
class homoTuple : public simpleTuple<T,T>{
  public:
    typedef simpleTuple<T,T> super;

    T&   operator[](unsigned int); //[NOTE] - Whenever const access is required, use the overloaded methods, else use the operator
    void operator=(const simpleTuple<T,T>&);
    bool operator==(const homoTuple<T>&) const;
    bool operator==(const simpleTuple<T,T>&) const;
    bool operator!=(const homoTuple<T>&) const;
    bool operator!=(const simpleTuple<T,T>&) const;

    homoTuple(T key, T value) : super(key,value){}
};

//##    TYPEDEF    ##
typedef homoTuple<int> intTuple;
typedef homoTuple<char> charTuple;
typedef homoTuple<float> floatTuple;
typedef homoTuple<double> doubleTuple;
//##  / TYPEDEF    ##
