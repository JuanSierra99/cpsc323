//---------------------------------------------------------------------------------------------------
//  Group Name: Carlos Mora, Juan Sierra, Juli Nazzario
//  CPSC 323 Final Project
//  Due Date: 5/09/2023
//---------------------------------------------------------------------------------------------------

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>
#include <stack>
#include <vector>
#include <regex>

//---------------------------------------------------------------------------------------------------
// Aux Functions
//---------------------------------------------------------------------------------------------------

// Function to load the source file into a formatted vector<String> for the parser
void FetchSourceFile();

// Function to check if a token is a reserved word
bool IsReservedWord(std::string word);

// Function to Tokenize the expressions on a cell in the parsing table
std::vector<std::string> ParsingTableTokenizer(std::string s);

// Function to translate the source code to c++
void TranslateToken(std::string token);

//---------------------------------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------------------------------

// Size of the predictive parsing table, update as required
#define MAX_EXPRESSIONS 23
#define MAX_LANGUAGE 30

// Source File Name and Directory
std::string filename = "finalp2.txt";

// Control Variables for void TranslateToken()
std::string translationMap = "";
std::string tempString = "";
std::vector<std::string> tempVariableList;
bool startTranslating = false;

// Control variable to store the tokenized source code
std::vector<std::string> sourceCodeTokens;

// Reserved Words List
std::string reservedWords[] = { "var", "program", "begin", "integer", "end.", "display", "\"value=\"," };
std::string requiredWords[] = { "var", "program", "begin", "integer", "end." };

// Valid Code control variable
bool isValid = true;

// Output Stream for the translated C++ file
std::ofstream output;

// Predictive Parsing Table
std::string parsingTable[MAX_EXPRESSIONS][MAX_LANGUAGE] = 
{
/* <program */      {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "program <identifier> ; var <dec-list> begin <stat-list> end.", "", "", "", ""}, // <program>
/* <dec-list> */    {"", "", "", "", "", "", "", "", "", "", "<dec> : <type> ;", "<dec> : <type> ;", "<dec> : <type> ;", "<dec> : <type> ;", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""}, // <dec-list>
/* <dec> */         {"", "", "", "", "", "", "", "", "", "", "<identifier> <D>", "<identifier> <D>", "<identifier> <D>", "<identifier> <D>", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""}, // <dec>
/* <D> */           {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "NULL", "", "", ", <dec>", "", "", "", "", "", "", ""}, // <D>
/* <stat-list> */   {"", "", "", "", "", "", "", "", "", "", "<stat> <L>", "<stat> <L>", "<stat> <L>", "<stat> <L>", "", "", "", "", "", "", "", "", "", "", "", "", "", "<stat> <L>", "", ""}, // <stat-list>
/* <L> */           {"", "", "", "", "", "", "", "", "", "", "<stat-list>", "<stat-list>", "<stat-list>", "<stat-list>", "", "", "", "", "", "", "", "", "", "", "", "", "", "<stat-list>", "", "NULL"}, // <L>
/* <stat> */        {"", "", "", "", "", "", "", "", "", "", "<assign>", "<assign>", "<assign>", "<assign>", "", "", "", "", "", "", "", "", "", "", "", "", "", "<write>", "", ""}, // <stat>
/* <write> */       {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "display ( <W> <identifier> ) ;", "", ""}, // <write>
/* <W> */           {"", "", "", "", "", "", "", "", "", "", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "", "", "", "", "", "", "\" value = \" ,", "", "", "", "", ""}, // <W>
/* <assign> */      {"", "", "", "", "", "", "", "", "", "", "<identifier> = <expr> ;", "<identifier> = <expr> ;", "<identifier> = <expr> ;", "<identifier> = <expr> ;", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""}, // <assign>
/* <expr> */        {"<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "", "", "<term> <expr'>", "<term> <expr'>", "", "", "", "<term> <expr'>", "", "", "", "", "", "", "", ""}, // <expr>
/* <expr'> */       {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "+ <term> <expr'>", "- <term> <expr'>", "NULL", "", "", "", "", "NULL", "", "", "", "", "", ""}, // <expr'>
/* <term> */        {"<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "", "", "<factor> <term'>", "<factor> <term'>", "", "", "", "<factor> <term'>", "", "", "", "", "", "", "", ""}, // <term>
/* <term'> */       {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "/ <factor> <term'>", "* <factor> <term'>", "NULL", "NULL", "NULL", "", "", "", "", "NULL", "", "", "", "", "", ""}, // <term'>
/* <factor> */      {"<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<identifier>", "<identifier>", "<identifier>", "<identifier>", "", "", "<number>", "<number>", "", "", "", "( <expr> )", "", "", "", "", "", "", "", ""}, // <factor>
/* <number> */      {"<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "", "", "", "", "", "", "<sign> <digit> <Y>", "<sign> <digit> <Y>", "", "", "", "", "", "", "", "", "", "", "", ""}, // <number>
/* <Y> */           {"<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "", "", "", "", "NULL", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "NULL", "", "", "", "", "", ""}, // <Y>
/* <sign> */        {"NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "", "", "+", "-", "", "", "", "", "", "", "", "", "", "", "", ""}, // <sign>
/* <identifier> */  {"", "", "", "", "", "", "", "", "", "", "<letter> <X>", "<letter> <X>", "<letter> <X>", "<letter> <X>", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""}, // <identifier>
/* <X> */           {"<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<letter> <X>", "<letter> <X>", "<letter> <X>", "<letter> <X>", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "", "NULL", "NULL", "", "", "", "", "", "" }, // <X>
/* <digit> */       {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""}, // <digit>
/* <letter> */      {"", "", "", "", "", "", "", "", "", "", "p", "q", "r", "s", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""}, // <letter>
/* <type> */        {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "integer", ""}, // <type>
};      

// Predicting Parsing Table Headers
std::string language[MAX_LANGUAGE] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "p", "q", "r", "s", "/", "*", "+", "-", ";", ":", "=", "(", ",", ")", "\"", "program", "begin", "display", "integer", "end."};
std::string expressions[MAX_EXPRESSIONS] = {
    "<program>",
    "<dec-list>",
    "<dec>",
    "<D>",
    "<stat-list>",
    "<L>",
    "<stat>",
    "<write>",
    "<W>",
    "<assign>",
    "<expr>",
    "<expr'>",
    "<term>",
    "<term'>",
    "<factor>",
    "<number>",
    "<Y>",
    "<sign>",
    "<identifier>",
    "<X>",
    "<digit>",
    "<letter>",
    "<type>"
};

// Valid Tokens For the Parser
std::string validTokens[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "p", "q", "r", "s", "/", "*", "+", "-", ";", ":", "=", "(", ",", ")", "\"", "program", "begin", "display", "integer", "end.", "value", "var",
    "<program>",
    "<dec-list>",
    "<dec>",
    "<D>",
    "<stat-list>",
    "<L>",
    "<stat>",
    "<write>",
    "<W>",
    "<assign>",
    "<expr>",
    "<expr'>",
    "<term>",
    "<term'>",
    "<factor>",
    "<number>",
    "<Y>",
    "<sign>",
    "<identifier>",
    "<X>",
    "<digit>",
    "<letter>",
    "<type>"
};

//---------------------------------------------------------------------------------------------------
// Main Function
//---------------------------------------------------------------------------------------------------

int main() 
{
    // Get the source file
    FetchSourceFile();

    // Look at the reserved words in check and make sure they are written in the example program. These words only show up once and must be included
    for (auto reserved: requiredWords){
        if (std::find(sourceCodeTokens.begin(), sourceCodeTokens.end(), reserved) == sourceCodeTokens.end()){
            std::cout << reserved << " is expected \n";
            return 0;
        }
    }

    // Parsing Table Control Local Variables
    std::stack<std::string> stack;
    int currentIndex = 0;

    // Output File name
    output.open("test_output.cpp");

    // Initialize the stack
    stack.push(expressions[0]);

    // While the stack is not empty
    while (!stack.empty()) 
    {
        // Get the current expression
        std::string currentExpression = stack.top();

        // Check if the current expression is not garbage data
        int languageSize = sizeof(validTokens) / sizeof(*validTokens);
        bool isInvalidExpression = std::find(validTokens, validTokens + languageSize, currentExpression) == validTokens + languageSize;
        if (isInvalidExpression) 
        {
            std::cout << "ERROR, INVALID -> " << currentExpression << "\n";
            stack.pop();
            continue;
        }

        // Pop current expression from stack
        stack.pop();
 
        // Check if currentExpression is the target expression
        // If match, go to next iteration and search next character from word
        if (currentExpression == (std::string() + sourceCodeTokens[currentIndex])) 
        {
            currentIndex++;

            // Start Translating After Reading the Source Code Filename
            if (currentExpression == "var")
            {
                startTranslating = true;
            }

            if (startTranslating)
            {
                TranslateToken(currentExpression);
            }
            
            continue;
        }
        
        // Read the current expression
        int expressionIndex = std::distance(expressions, std::find(expressions, expressions + MAX_EXPRESSIONS, currentExpression));
        int languageIndex = std::distance(language, std::find(language, language + MAX_LANGUAGE, sourceCodeTokens[currentIndex]));

        // Fetch the value of the current expression in the parsing table
        std::string parsingTableResult = parsingTable[expressionIndex][languageIndex];

        // Check index out of bounds errors
        if (expressionIndex == 23)
        {
            if (sourceCodeTokens[currentIndex] == ")" && currentExpression == ";")
            {
                std::cout << "(" << " is missing\n"; //we are looking for a right parenthesis, but got to a semicolon in stack. We should have done (<expr>) instead
                return 0;
            }
            if (currentExpression == "(" || currentExpression == ")" || currentExpression == ":" || currentExpression == ";" || currentExpression == ",")
            {
                std::cout << currentExpression << " is missing\n";
            }
            else
            {
                std::cout << "expected " << currentExpression << std::endl;
            }
            return 0;
        }
        
        if (languageIndex == 30 and sourceCodeTokens[currentIndex] == "var")
        {
            std::cout << "; is missing" << std::endl;
            return 0;
        }


        // If the parsing table result is lambda (skip in this case), skip to the next iteration
        if (parsingTableResult == "NULL") 
        {
            continue;
        }

        // If the result from the parsing table is empty, expression is invalid
        if (parsingTableResult.empty()) 
        {
            if (currentExpression == "<type>"){
                std::cout << "integer is expected\n";
            }
            else if (currentExpression == "<L>" && (sourceCodeTokens[currentIndex] == "(" || sourceCodeTokens[currentIndex] == "d")){ // display is a FIRST of <L>, but if we get ( instead, then we know that we missed display.
                std::cout << "display is expected\n";
            }
            else if (currentExpression == "<identifier>"){ //if the issue is here, its because we did not proved a valid <identifier>
                std::cout << "Unknown identifier\n";
            }
            else if (currentExpression == "<dec-list>" && sourceCodeTokens[currentIndex] == ":"){  // if the issue is here, its because variable was not defined correctly
                std::cout << "Unknown identifier\n";
            }
            else if (currentExpression == "<X>" && sourceCodeTokens[currentIndex] == "integer"){ //there should be a : between <X> and integer in stack
                std::cout << ": is missing \n";
            }
            else if (currentExpression == "<term'>" && sourceCodeTokens[currentIndex] == "display"){
                std::cout << "; is missing\n";
            }
            else if (currentExpression == "<Y>")
            {
                std::cout << "; is missing\n"; // I DONT THINK THIS IS RIGHT <
            }

            // Mark code as invalid
            isValid = false;
            break;
        }
        // Else, Process the Token Into the Stack of the Parser
        else 
        {
            std::vector<std::string> tempVector = ParsingTableTokenizer(parsingTableResult);

            if (tempVector.size() == 1) 
            {
                stack.push(tempVector[0]);
            }
            else 
            {
                for (int i = tempVector.size()-1; i>=0; i--) 
                {
                    stack.push(tempVector[i]);
                }
            }
        }
    }

    // Print if the expression was valid or not
    (isValid) ? std::cout << "\nValid Expression \n\n" : std::cout << "\nInvalid Expression \n\n";

    return 0;
}

//---------------------------------------------------------------------------------------------------
// Aux Functions
//---------------------------------------------------------------------------------------------------

// Function to load the source file into a formatted vector<String> for the parser
void FetchSourceFile() 
{
	std::string line, word;
	std::fstream file (filename, std::ios::in);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::stringstream str(line);
 
			while (getline(str, word, ' ')) 
            {

                if (word == "\"value=\",") {
				    sourceCodeTokens.push_back("\"");
				    sourceCodeTokens.push_back("value");
				    sourceCodeTokens.push_back("=");
				    sourceCodeTokens.push_back("\"");
				    sourceCodeTokens.push_back(",");;
                }
                else if (IsReservedWord(word)) 
                {
				    sourceCodeTokens.push_back(word);
                }
                else 
                {
                    for (int i=0; i<word.length(); i++) 
                    {
                        sourceCodeTokens.push_back(std::string() + word[i]);
                    }
                }
            }
		}
	}
	else 
    {
		std::cout<<"Could not open the file\n";
    }
}

bool IsReservedWord(std::string word) 
{
    int size = sizeof(reservedWords) / sizeof(*reservedWords);
    return std::find(reservedWords, reservedWords + size, word) != reservedWords + size;
}

// Function to Tokenize the expressions on a cell in the parsing table
std::vector<std::string> ParsingTableTokenizer(std::string s) 
{
    std::stringstream ss(s);
    std::vector<std::string> output;
    std::string word;

    while (ss >> word) 
    { 
        output.push_back(word);
    }

    return output;
}

// Function to translate the source code to c++
void TranslateToken(std::string token) {
    std::regex r("[p-s0-9]");

    // WRITE MAP
    if (translationMap == "write") 
    {    
        if (token == ";") 
        {
            output << ";\n\t";
            tempString = "";
            translationMap = "";
        }
        else if (token == "(") 
        {
            output << " << ";
            tempString = "";
        }
        else if (token == ")") 
        {
            output << tempString;
            output << " << std::endl";
            tempString = "";
        }
        else if (token == "\"") 
        {
            output << "\"";
            tempString = "";
        }
        else if (token == "=") 
        {
            output << "=";
            tempString = "";
        }
        else if (token == "value") 
        {
            output << "value";
            tempString = "";
        }
        else if (token == ",") 
        {
            output << " << ";
            tempString = "";
        }
        else if (std::regex_match(token, r)) 
        {
            tempString.append(token);
        }
    } 
    // VARIABLE MAP
    else if (translationMap == "var") 
    {
        if (token == ";") 
        {
            output << ";\n\t";
            tempString = "";
            translationMap = "";
        }
        else if (token == ",") 
        {
            tempVariableList.push_back(tempString);
            tempVariableList.push_back(", ");
            tempString = "";
        }
        else if (token == ":") 
        {
            tempVariableList.push_back(tempString);
            tempString = "";
        }
        else if (token == "integer") 
        {
            output << "int ";

            for (int i=0; i<tempVariableList.size(); i++) 
            {
                output << tempVariableList[i];
            }

            tempString = "";
        }
        else if (std::regex_match(token, r)) 
        {
            tempString.append(token);
        }
    }
    else 
    {
        // IGNORE -> program <identifier> ; since it is useless for the c++ translation
        if (token == "var") 
        {
            output << "#include <iostream>\n\nint main() {\n\t";
            translationMap = "var";
        } 
        else if (token == ";") 
        {
            output << tempString;
            output << ";\n\t";
            tempString = "";
        } 
        else if (token == ":") 
        {
            output << tempString;
            output << " : ";
            tempString = "";
        }
        else if (token == ",") 
        {
            output << tempString;
            output << ", ";
            tempString = "";
        }
        else if (token == "integer") 
        {
            output << "integer";
            tempString = "";
        }
        else if (token == "=") 
        {
            output << tempString;
            output << " = ";
            tempString = "";
        }
        else if (token == "+") 
        {
            output << tempString;
            output << " + ";
            tempString = "";
        }
        else if (token == "-") 
        {
            output << tempString;
            output << " - ";
            tempString = "";
        }
        else if (token == "*") 
        {
            output << tempString;
            output << " * ";
            tempString = "";
        }
        else if (token == "/") 
        {
            output << tempString;
            output << " / ";
            tempString = "";
        }
        else if (token == "(") 
        {
            output << tempString;
            output << "(";
            tempString = "";
        }
        else if (token == ")") 
        {
            output << tempString;
            output << ")";
            tempString = "";
        }
        else if (token == ")") 
        {
            output << tempString;
            output << ")";
            tempString = "";
        }
        else if (token == "display") 
        {
            output << "std::cout";
            tempString = "";
            translationMap = "write";
        }
        else if (token == "end.") 
        {
            output << "\n\treturn 0;\n}";
            tempString = "";
        }
        else if (std::regex_match(token, r)) 
        {
            tempString.append(token);
        }
    }
}
