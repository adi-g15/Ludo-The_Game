#pragma once

#include <string>
#include <algorithm>

#define stringify(name) #name

namespace util
{

    void trim(std::string &);                                   //@returns trimmed std::string, but doesnt modify original string
    std::string trim_copy(const std::string &);                 //@returns trimmed std::string, but doesnt modify original string
    void strip(std::string &, char toRemove);                   /*Returns a string without the passed character*/
    std::string strip_copy(const std::string &, char toRemove); /*Returns a string without the passed character*/
    bool icompare(const std::string &s1, const std::string &s2) noexcept;

} // namespace util

    //DEFINITIONS
void util::trim(std::string &s)
{
    auto i = s.end() - 1;
    for (; i != s.begin(); --i)
    {
        if (!isspace(static_cast<unsigned char>(*i)))
        {
            ++i;
            break;
        }
    }
    s.erase(i, s.end());

    i = s.begin();
    for (; i != s.end(); ++i)
    {
        if (!isspace(static_cast<unsigned char>(*i)))
        {
            break;
        }
    }
    s.erase(s.begin(), i);

        //earlier one, but has bug
    // s.erase(std::find_if(s.begin(), s.end(), isspace), s.rend().base());
    // s.erase(std::find_if(s.rbegin(), s.rend(), isspace).base(), s.end());
}

std::string util::trim_copy(const std::string &s)
{
    std::string str_out(s);
    util::trim(str_out);
    return str_out;
}

void util::strip(std::string &s, char toRemove)
{
    s.erase(
        std::remove(s.begin(), s.end(), toRemove),
        s.end());
}

std::string util::strip_copy(const std::string &s, char toRemove)
{
    std::string str_out(s);
    util::strip(str_out, toRemove);

    return str_out;
}

bool util::icompare(const std::string &s1, const std::string &s2) noexcept
{
    for (size_t i = 0; i < s1.size(); ++i)
    {
        if (std::tolower(s1[i]) != tolower(s2[i]))
            return false;
    }
    return true;
}
