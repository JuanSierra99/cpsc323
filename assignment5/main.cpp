//---------------------------------------------------------------------------------------------------
//  Group Name: Carlos Mora, Juan Sierra, Juli Nazzario
//  Assignment: No 5
//  Due Date: 3/02/2023
//  Purpose: This program reads an input file, removes any extra spaces and comments, and outputs it to a new file
//---------------------------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

//---------------------------------------------------------------------------------------------------------------------------------
bool isComment(std::string line){
    return line.find("**") != std::string::npos;
}

std::string removeSpaces(std::string line){
    std::string afterSpaces;
    bool prevSpace = false;
    for (int i = 0; i< line.length(); i++){
        if(line[i] == ' '){
            if(!prevSpace) afterSpaces += ' ';

            prevSpace = true;
        } else {
            afterSpaces += line[i];
            prevSpace = false;
        }
    }

    return afterSpaces;
}


int main() {
    std::ifstream fin;
    std::ofstream fout;
    std::string line;
    std::vector<std::string> fileContents;
    
    fin.open("h5.txt");
    fout.open("newh5.txt");

    int i = 0;
    while(std::getline(fin, line)){
        if(line.empty()){
            continue;
        }   
        if(!isComment(line)){
            std::string tokenizedLine = removeSpaces(line);
            fout << tokenizedLine;
        fout << std::endl;

        }
    }
    
    fin.close();
    fout.close();
    
    return 0;
}

