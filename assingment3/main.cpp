#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

std::string reserved[] = {"while", "for", "switch", "do", "return"};


std::string number(std::string token){
    for (char const &ch : token){
        if (isdigit(ch) == 0){
            return "no";
        }
    }
    return "YES";
}

std::string reserve(std::string token){
    for (int i = 0; i < reserved->length(); i++){
        if(token == reserved[i]){
            return "YES";
        }
    }
    return "no";
}

int main(){
std::ifstream File;
std::string str;
File.open("sample.txt");
std::cout << "Token        number      identifier      reserved" << std::endl;
std::cout << std::setfill('-') << std::setw(50) << "-" << std::endl;
std::cout << std::setfill(' ');
if(File.is_open()){
    while(File.good()){
        File >> str;
        std::cout << std::setw(8) << str;
        std::cout << std::setw(10) << number(str);
        std::cout << std::setw(13) << reserve(str);
        std::cout << std::setw(16) << "Here";
        std::cout << std::endl;
    }
}
return 0;
}
