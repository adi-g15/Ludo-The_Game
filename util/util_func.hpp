#define CUSTOM_UTIL_FUNC
#define stringify( name ) #name

#ifndef _GLIBCXX_STRING
    #include<string>
#endif

#ifndef CUSTOM_EXCEPTION
  #include "./exceptions.hpp"
#endif

#ifndef CUSTOM_SIMPLETUPLE
    #include "./simpleTuple.hpp"
#endif

intTuple getTerminalDimen();
inline bool align_text_center(int max_length,const std::string&);
inline std::string trimString(const std::string&);   //@returns trimmed std::string, but doesnt affect original std::string