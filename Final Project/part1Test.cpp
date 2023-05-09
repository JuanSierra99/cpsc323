#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
//---------------------------------------------------------------------------------------------------------------------------------

// Check if the current line is a comment
bool isComment(std::string line){
    return line.find("//") != std::string::npos;
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


    for(int i = 0; i < line.length(); i++){
        if(line[i] == ' '){
            if(!prevSpace) afterSpaces += ' ';
            prevSpace = true;
        }
    }

    return afterSpaces;
}

bool is_comment_start(std::string line, size_t pos) {
    // Check if "//" occurs at the beginning of the line or after a whitespace character
    return (pos == 0 || isspace(line[pos - 1])) && line.find("//", pos) == pos;
}

std::string remove_comment(std::string line, size_t pos) {
    // Remove the comment starting from "//" to the end of the line
    return line.substr(0, pos) + '\n';
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

    std::string tokenizedLine = removeSpaces(line);
    for (int i = 0; i< tokenizedLine.length(); i++){
        if(is_comment_start(tokenizedLine, i)){
            tokenizedLine = remove_comment(tokenizedLine, i);

    }}
    

        // If the line is a keyword
        if (tokenizedLine == " var" || tokenizedLine == " begin" || tokenizedLine == " end." || tokenizedLine == "display") {
            // Leave as it is
            fout << tokenizedLine;
        } else {
            fout << tokenizedLine;
        }

        // Add an end of line
        fout << std::endl;

    }
    
    // Close the input and output file streams
    fin.close();
    fout.close();
    
    return 0;
}
