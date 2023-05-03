#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
//---------------------------------------------------------------------------------------------------------------------------------

// Check if the current line is a comment
bool isComment(std::string line){
    return line.find("**") != std::string::npos;
}

// Remove the unused blank spaces and separate each token with only 1 space
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
    
    // Open the input and output file streams
    fin.open("finalp1.txt");
    fout.open("finalp2.txt");

    int i = 0;

    // Get the whole line of the input file
    while(std::getline(fin, line)){
        // If the line is empty, continue to the next iteration
        if(line.empty()){
            continue;
        }

        // If the line is not comment, remove the unnecesary spaces of the line
        if(!isComment(line)){
            std::string tokenizedLine = removeSpaces(line);

        // If the line is a keyword
        if (tokenizedLine == " var" || tokenizedLine == " begin" || tokenizedLine == " end.") {
            // Leave as it is
            fout << tokenizedLine;
        } else {
            // Add 4 spaces of indentation
            fout << "    " << tokenizedLine;
        }

        // Add an end of line
        fout << std::endl;

        }
    }
    
    // Close the input and output file streams
    fin.close();
    fout.close();
    
    return 0;
}
