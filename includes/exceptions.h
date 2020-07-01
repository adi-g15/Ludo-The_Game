#pragma once

#include<exception>
#include<string>
#include "enumerations.hpp"

class OutOfBoundException : std::exception{
  std::string source;
public:
  const char* what();
  explicit OutOfBoundException(const char*); //! @params DataStructure name, in which the exception occured
  OutOfBoundException() = delete; //! deleted so as to make giving a reason mandatory
};

class GotiNotAvailableException : std::exception{
  colours Colour;
  public:
    const char* what();
    explicit GotiNotAvailableException(colours);
    GotiNotAvailableException() = delete;
};

class endApplication : public std::exception{
  std::string source;
  public:
    const char* what();
    explicit endApplication(const char*);
    explicit endApplication(std::string);
    endApplication() = delete;	//Passing source of request/error is mandatory
};