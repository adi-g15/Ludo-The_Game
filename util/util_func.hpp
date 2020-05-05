#ifndef _GLIBCXX_STRING
    #include<string>
#endif
#ifndef _GLIBCXX_IOSTREAM
    #include<iostream>
#endif
#ifndef _GLIBCXX_ALGORITHM
    #include<algorithm>
#endif

/*LEARNT - ASCII value of integers 0-9 are actually 48-57*/

inline bool align_text_center(int max_length,const std::string&);
inline string trimString(const std::string&);   //@returns trimmed string, but doesnt affect original string

//Definitions
inline bool align_text_center(int max_length,const std::string& str){    //@returns bool indicating, if max_len is enough or not
    if(max_length < str.size()){
        return false;
    }
    else
    {
        for (size_t i = 0; i < (max_length-str.size())/2; i++)  std::cout<<' ';
        std::cout<<str;
        for (size_t i = 0; i < (max_length-str.size()) - (max_length-str.size())/2; i++)  std::cout<<' ';
    }
    
}

inline string trimString(const std::string &s){
    /*Learnt - In case of only 1 argument passed to string::erase()...
                1. If we provide an index number, ALL including and after it are erased
                2. If we provide an iterator,    ONLY that position is removed*/

    //find_if - Keep finding, until the predicate is true
    string str_out(s);
    str_out.erase(std::find_if(str_out.rbegin(),str_out.rend(),[](char ch){
        return ! isspace(ch);
    }).base(), str_out.end());

    //LEARNT - Using .base() on a reverse_iterator returns us the normal_iterator, BUT CAUTION- This isn't advisable, and might cause SEGFAULT, (for eg. accessing .rbegin().base())

    str_out.erase(std::find_if(str_out.begin(),str_out.end(),[](char ch){
        return ! isspace(ch);
    }), str_out.rend().base());
    
}