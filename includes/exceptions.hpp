#pragma once

#define CUSTOM_EXCEPTION 1

// #ifndef _GLIBCXX_STRING
//   #include<string>
// #endif

// #ifndef CUSTOM_ENUMERATIONS
//   #include "enumerations.hpp"
// #endif

#include<exception>
#include<string>
#include "enumerations.hpp"

class OutOfBoundException : std::exception{
  std::string source;
public:
  const char* what(); //@returns char array denoting description of problem
  OutOfBoundException(const char*); //@params DataStructure name, in which the exception occured
  OutOfBoundException() = delete; //deleted so as to make giving a reason mandatory
};

class GotiNotAvailableException : std::exception{
  colours Colour;
  public:
    const char* what();
    GotiNotAvailableException(colours);
    GotiNotAvailableException() = delete;
};