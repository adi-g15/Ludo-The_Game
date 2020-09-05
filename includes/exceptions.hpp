#pragma once

#include <stdexcept>
#include <string>

class endApplication : public std::logic_error{
  public:
    explicit endApplication(const std::string& s) : std::logic_error(s){}

    endApplication() = delete;	//Passing source of request/error is mandatory
};
