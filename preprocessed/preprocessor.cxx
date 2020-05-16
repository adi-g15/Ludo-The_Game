// #include<iostream>
#include<iostream>
#include<fstream>
#include<filesystem>

// using recursive_directory_iterator = std::experimental::filesystem::recursive_directory_iterator;
 namespace filesystem = std::filesystem;

using namespace std;

int main(int argc, char const *argv[])
{
    std::ifstream fin;    
    std::ofstream fout;
    
    string str = "ludo_goti";

    fin.open(str);
    fout.open(str + ".cpp");

    while( getline( fin, str ) ){
        if(str[0] != '#')
            fout<<str<<endl;
    }

     const filesystem::path path{    //OBSERVE
         argc>=2 ? argv[1] : filesystem::current_path()
     };

     for (auto &&entry : filesystem::directory_iterator(path)){
         const auto FNameStr = entry.path().filename().string();
         if(filesystem::is_directory(entry)){
             cout<<"dir : "<< FNameStr <<endl;
         }else if (filesystem::is_regular_file(entry))
         {
             cout<<"file : "<< FNameStr <<endl;
         }else
         {
             cout<<"??   "<< FNameStr <<endl;
         }       
     }    

    return 0;
}
