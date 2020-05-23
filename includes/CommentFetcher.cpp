#define FILE_NAME "includes/board.hpp"
// #define COMMENT_TYPE "Learnt"

#include<fstream>
#include<iostream>
#include<string>
#include<algorithm>

using namespace std;

int main(int argc, char const *argv[])
{
    ifstream fin;
    fin.open(FILE_NAME);
    ofstream fout;
    std::cout<<"Reached here1"<<endl;
    fout.open("Comments.txt", ios::app);

    auto lambda = [](char ch){
        return !isspace(ch) || ch=='\n';
    };

    bool endingQuoteFound = false;
    std::string tmpStr, tmp2;
    int counter = 1;
    // int len_commentName = string(COMMENT_TYPE).size();

    fout<<"\n\n"<<FILE_NAME<<"\n"<<"---------------------------------------------------\n\n";
    // fout<<COMMENT_TYPE<<"\n";

    while (!fin.eof())
    {
        tmpStr.clear();
        getline(fin, tmpStr);
        tmpStr.erase(std::find_if( tmpStr.begin(),tmpStr.end(),lambda ), tmpStr.rend().base());
        auto iter1 = find(tmpStr.begin(), tmpStr.end(), '/');
        if( iter1 != tmpStr.end() ){
            if( *(iter1+1) == '/'){ //Single Line Comment
                tmpStr.erase(tmpStr.begin(),iter1+2);   //End position not included
                fout<<'\n'<<counter<<". "<<tmpStr<<'\n';
                ++counter;
            }else if ( tmpStr[1] == '*')   //Multi-Line Comment
            {
                tmpStr.erase(0,1);
                endingQuoteFound = false;
                tmp2 = "";
                while( !endingQuoteFound && !fin.eof()){
                    auto iter = std::find(tmp2.begin()+2, tmp2.end(), '*');
                    if( iter != tmp2.end() ){
                        if( *(iter+1) == '/' ){ //Single-Line Comment (b/t /**/)
                            tmp2.erase(iter, tmp2.end());
                            endingQuoteFound = true;
                            tmpStr.insert(tmpStr.end(), tmp2.begin(), tmp2.end());
                        }
                    }
                    getline( fin, tmp2 );
                }

                if( endingQuoteFound ){
                        fout<<counter<<". "<<tmpStr<<'\n';
                        ++counter;
                }
            }        
        }
    }
    
    return 0;
}
