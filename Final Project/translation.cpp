// Program p2
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <stack>
#include <vector>
#include <regex>

std::string map = "";
std::string tempString = "";
std::vector<std::string> tempVariableList;

void TranslateToken(std::string token) {
    std::regex r("[p-s0-9]");

    // WRITE MAP
    if (map == "write") {
        if (token == ";") {
            std::cout << ";\n\t";
            tempString = "";
            map = "";
        }
        else if (token == "(") {
            std::cout << " << ";
            tempString = "";
        }
        else if (token == ")") {
            std::cout << tempString;
            std::cout << " << std::endl";
            tempString = "";
        }
        else if (token == "\"") {
            std::cout << "\"";
            tempString = "";
        }
        else if (token == "=") {
            std::cout << "=";
            tempString = "";
        }
        else if (token == "value") {
            std::cout << "value";
            tempString = "";
        }
        else if (token == ",") {
            std::cout << " << ";
            tempString = "";
        }
        else if (std::regex_match(token, r)) {
            tempString.append(token);
        }

    } 
    // VARIABLE MAP
    else if (map == "var") {
        if (token == ";") {
            std::cout << ";\n\t";
            tempString = "";
            map = "";
        }
        else if (token == ",") {
            tempVariableList.push_back(tempString);
            tempVariableList.push_back(", ");
            tempString = "";
        }
        else if (token == ":") {
            tempVariableList.push_back(tempString);
            tempString = "";
        }
        else if (token == "integer") {
            std::cout << "int ";

            for (int i=0; i<tempVariableList.size(); i++) {
                std::cout << tempVariableList[i];
            }

            tempString = "";
        }
        else if (std::regex_match(token, r)) {
            tempString.append(token);
        }
    }
    else {
        if (token == "var") { // IGNORE -> program <identifier> ; since it is useless for the c++ translation
            std::cout << "#include <stdio.h>\n#include <iostream>\n\nint main() {\n\t";
            map = "var";
        } 
        else if (token == ";") {
            std::cout << tempString;
            std::cout << ";\n\t";
            tempString = "";
        } 
        else if (token == ":") {
            std::cout << tempString;
            std::cout << " : ";
            tempString = "";
        }
        else if (token == ",") {
            std::cout << tempString;
            std::cout << ", ";
            tempString = "";
        }
        else if (token == "integer") {
            std::cout << "integer";
            tempString = "";
        }
        else if (token == "begin") {
            // SKIP
        }
        else if (token == "=") {
            std::cout << tempString;
            std::cout << " = ";
            tempString = "";
        }
        else if (token == "+") {
            std::cout << tempString;
            std::cout << " + ";
            tempString = "";
        }
        else if (token == "-") {
            std::cout << tempString;
            std::cout << " - ";
            tempString = "";
        }
        else if (token == "*") {
            std::cout << tempString;
            std::cout << " * ";
            tempString = "";
        }
        else if (token == "/") {
            std::cout << tempString;
            std::cout << " / ";
            tempString = "";
        }
        else if (token == "(") {
            std::cout << tempString;
            std::cout << "(";
            tempString = "";
        }
        else if (token == ")") {
            std::cout << tempString;
            std::cout << ")";
            tempString = "";
        }
        else if (token == ")") {
            std::cout << tempString;
            std::cout << ")";
            tempString = "";
        }
        else if (token == "display") {
            std::cout << "std::cout";
            tempString = "";
            map = "write";
        }
        else if (token == "end.") {
            std::cout << "\n\treturn 0;\n}";
            tempString = "";
        }
        else if (std::regex_match(token, r)) {
            tempString.append(token);
        }
    }
}

int main() {

    std::vector<std::string> newWord{"var", "p", "1", ",", "q", "2", ":", "integer", ";", "begin", "p", "1", "=", "3", "3", "+", "(", "2", "/", "3", "*", "2", ")", ";", "display", "(", "\"", "value", "=", "\"", ",", "p", "1", ")", ";", "end."};

    for (int i=0; i<newWord.size(); i++) {
        TranslateToken(newWord[i]);
    }
}