#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
//---------------------------------------------------------------------------------------------------------------------------------

// Check if the current line is a comment
bool contains_comment(std::string line){
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

// Check if it is multiline comment
bool is_multiline_comment(std::string line) {
    std::regex r("\\s+");
    line = std::regex_replace(line, r, "");

    return ((line[0] == '/' && line[1] == '/') || (line[line.size()-1] == '/' && line[line.size()-2] == '/'));
}

std::string remove_comment(std::string line, size_t pos) {
    // Remove the comment starting from "//" to the end of the line
    return line.substr(0, pos);
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
        // If the line is empty or if has an inline comment start or end
        if(line.empty() || is_multiline_comment(line)){
            continue;
        }

        // Remove the whitespace from the line of code
        std::string tokenizedLine = removeSpaces(line);

        // Check if the line of code has a 
        if (contains_comment(tokenizedLine)) {
            int commentPos = tokenizedLine.find_first_of("//");
            tokenizedLine = remove_comment(tokenizedLine, commentPos);
        }
    
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
