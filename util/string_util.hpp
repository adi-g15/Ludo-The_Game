#pragma once

#include <string>
#include <vector>
#include <sstream>  //for tokenizer
#include <algorithm>
#include <string_view>

#define stringify(name) #name

namespace util{

    static void trim(std::string&);                                   //@returns trimmed std::string, but doesnt modify original string
    static std::string&& trim_copy(const std::string&);                 //@returns trimmed std::string, but doesnt modify original string
    static void trimChar(std::string& s, char ch); //Trims out `ch` from both ends
    static bool replace_all(std::string& str, const std::string& _old, const std::string& _new); //can use boost::algorithm::replace_all also
    static void strip(std::string&, char toRemove);                   /*Returns a string without the passed character*/
    static std::string&& strip_copy(const std::string&, char toRemove); /*Returns a string without the passed character*/

    static std::vector< std::string > tokenizeIt(const std::string& dir, char sep);
    static std::vector<std::string> split(const std::string& s, char delim = ','); //similar to tokenizeIt(), but this is meant to consider double quotes too

    inline namespace view_util
    {
        static std::string&& trim_copy(std::string_view);
        static std::string&& strip_copy(std::string_view, char toRemove);
        static bool icompare(std::string_view s1, std::string_view s2) noexcept;

        static std::vector< std::string > tokenizeIt(std::string_view dir, char sep);
        static std::vector<std::string> split(std::string_view s, char delim = ',');
    } // namespace view_util


} // namespace util

    //DEFINITIONS
void util::trim(std::string& s){
    if (s.empty())   return;

    s.erase(
        s.begin(),
        std::find_if_not(s.begin(), s.end(), isspace)
    );

    auto end_iter = std::find_if_not(s.rbegin(), s.rend(), isspace);
    if ( end_iter == s.rend() )   return;
    s.erase(
        end_iter.base(),    /*s.erase() doesn't erase the first one ??*/
        s.end()
    );
}

std::string&& util::trim_copy(const std::string& s){
    return std::move(view_util::trim_copy(s.c_str()));
}

void util::trimChar(std::string& s, char ch){
    while( s.back() == ch ){
        s.pop_back();
    }
    if( s.back() == ch ){
        s.pop_back();
    }
}

bool util::replace_all(std::string& s, const std::string& _old, const std::string& _new){
    //replace all occurences of `old` with `new` in str
    size_t start = s.find(_old);

    if( start == std::string::npos ){
        return false;   //oldval not found
    }
    while( start != std::string::npos ){
        s.replace(start, _old.size(), _new);
        start = s.find(_old);
    }

    return true;
}


void util::strip(std::string& s, char toRemove){
    s.erase(
        std::remove(s.begin(), s.end(), toRemove),
        s.end());
}

std::string&& util::strip_copy(const std::string& s, char toRemove){
    return std::move(view_util::strip_copy(s.c_str(), toRemove));
}

std::vector< std::string > util::tokenizeIt(const std::string& dir, char sep){
    return view_util::tokenizeIt(dir.c_str(), sep);
}


namespace util::view_util
{
    std::string&& trim_copy(std::string_view s){
        std::string str_out(s);
        util::trim(str_out);
        return std::move(str_out);
    }

    std::string&& strip_copy(std::string_view s, char toRemove){
        std::string str_out(s);
        util::strip(str_out, toRemove);

        return std::move(str_out);
    }

    bool icompare(std::string_view s1, std::string_view s2) noexcept{
        for( size_t i = 0; i < s1.size(); ++i ){
            if( std::tolower(s1[i]) != tolower(s2[i]) )
                return false;
        }
        return true;
    }


    //@todo - Debug it
    std::vector<std::string> split(std::string_view s, char delim) {
        std::vector<std::string> retVec;

        auto start = s.begin(), end = std::find(s.begin(), s.end(), delim);
        while( end != s.end() ){
            //this while loops while there is an unclosed quote present
            while( std::count(start, end, '"') % 2 != 0 ){	//ie. odd number of quotes
                if( end == s.end() ){
                    throw std::logic_error("Parsing Error: Odd number of quotes in passed string, end of input");
                }
                //++end so that it includes the last found returned by std::find
                end = std::find(++end, s.end(), '"')+1;
            }
            retVec.push_back( std::string( s.substr(start - s.begin(), end - start) ) );

            //incremented just to ignore the delim
            start = end;
            if( end != s.end() ){
                start = ++end;
            }
            end = std::find(start, s.end(), delim);
        }
        if( start != s.end() ){
            retVec.push_back( std::string( s.substr(start - s.begin()) ) );
        }

        return retVec;
    }

    std::vector< std::string > tokenizeIt(std::string_view dir, char sep){
        std::istringstream stream(dir.data());
        std::vector< std::string > tokens;
        std::string token;

        while( std::getline(stream, token, sep) ){
            tokens.push_back(token);
        }

        return tokens;
    }

}
