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

std::vector<std::string> tokenization(std::string line){
    std::vector<std::string> tokens;
    std::string current;

    for(int i = 0; i< line.length();i++)
    {
        if(line[i] == ' ' || line[i] == '\n' || line[i] == 'r' || line[i] == '\t' || line[i] == '*'){
            if(!current.empty()){
                tokens.push_back(current);
                current.clear();
            }
        }else current += line[i];
    
        if(!current.empty()) tokens.push_back(current);

    }

    return tokens;

}

int main() {
    std::ifstream fin;
    std::ofstream fout;
    std::string line;
    std::vector<std::string> fileContents;
    
    fin.open("h5.txt");
    fout.open("newh5.txt");

    
    while(std::getline(fin, line)){
        if(!isComment(line)){
            std::string tokenizedLine = removeSpaces(line);
            std::vector<std::string> tokens = tokenization(tokenizedLine);

            for(std::string token : tokens){
                fout << token << " ";
            }

        fout << std::endl;

        }
    }
    
    fin.close();
    fout.close();
    
    
    return 0;
}
