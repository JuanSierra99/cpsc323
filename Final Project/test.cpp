//---------------------------------------------------------------------------------------------------
//  Group Name: Carlos Mora, Juan Sierra, Juli Nazzario
//  Assignment: No 7 - Part 1
//  Due Date: 3/23/2023
//  Purpose: This program reads a predictive parsing table to trace if 
//  an input is valid or not given a language
//---------------------------------------------------------------------------------------------------

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <stack>
#include <vector>

// Size of the predictive parsing table, update as required
#define MAX_EXPRESSIONS 23
#define MAX_LANGUAGE 30

// Function to print a stack
void PrintStack(std::stack<std::string> s);
std::vector<std::string> ParsingTableTokenizer(std::string s);

// Predictive Parsing Table
std::string parsingTable[MAX_EXPRESSIONS][MAX_LANGUAGE] = 
{
/*<prog>*/      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "program <identifier> ; var <dec-list> begin <stat-list> end.", "", "", "", "" }, // CHECKED
/*<identifier>*/ { "", "", "", "", "", "", "", "", "", "", "<letter> <X>", "<letter> <X>", "<letter> <X>", "<letter> <X>", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<X>*/         { "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<letter> <X>", "<letter> <X>", "<letter> <X>", "<letter> <X>", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "", "NULL", "NULL", "", "", "", "", "", "" }, // CHECKED
/*<dec-list>*/  { "", "", "", "", "", "", "", "", "", "", "<dec> : <type> ;", "<dec> : <type> ;", "<dec> : <type> ;", "<dec> : <type> ;", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<dec>*/       { "", "", "", "", "", "", "", "", "", "", "<identifier> , <dec>", "<identifier> , <dec>", "<identifier> , <dec>", "<identifier> , <dec>", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<D>*/         { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "NULL", "", "", ", <dec>", "", "", "", "", "", "", "" }, // CHECKED
/*<type>*/       { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "integer", "" }, // CHECKED
/*<stat-list>*/  { "", "", "", "", "", "", "", "", "", "", "<stat> <L>", "<stat> <L>", "<stat> <L>", "<stat> <L>", "", "", "", "", "", "", "", "", "", "", "","", "", "<stat> <L>", "", "" }, // CHECKED
/*<L>*/         { "", "", "", "", "", "", "", "", "", "", "<stat-list>", "<stat-list>", "<stat-list>", "<stat-list>", "", "", "", "", "", "", "", "", "", "", "", "", "", "<stat-list>", "", "NULL" }, // CHECKED
/*<stat>*/       { "", "", "", "", "", "", "", "", "", "", "<assign>", "<assign>", "<assign>", "<assign>", "", "", "", "", "", "", "", "", "", "", "", "", "", "<write>", "", "" }, // CHECKED
/*<write>*/      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "display ( <identifier> ) ; display( \" value \" , <identifier> ) ;", "", "" }, // CHECKED
/*<W>*/         { "", "", "", "", "", "", "", "", "", "", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "", "", "", "", "", "", "\" value \" ,", "", "", "", "", "" }, // CHECKED
/*<assign>*/    { "", "", "", "", "", "", "", "", "", "", "<identifier> = <expr> ;", "<identifier> = <expr> ;", "<identifier> = <expr> ;", "<identifier> = <expr> ;", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<expr>*/      { "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "", "", "<term> <expr'>", "<term> <expr'>", "", "", "", "<term> <expr'>", "", "", "", "", "", "", "", "" }, // CHECKED
/*<expr'>*/     { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "- <term> <expr'> ", "- <term> <expr'> ", "NULL", "", "", "", "", "NULL", "", "", "", "", "", "" }, // CHECKED, but recheck for the + sign
/*term>*/       { "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "", "", "<factor> <term'>", "<factor> <term'>", "", "", "", "<factor> <term'>", "", "", "", "", "", "", "", "" }, // CHECKED
/*<term'>*/      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "/ <factor> <term'> ", "* <factor> <term'> ", "NULL", "NULL", "NULL", "", "", "", "", "NULL", "", "", "", "", "", "" }, // CHECKED
/*<factor>*/     { "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", " <identifier>", " <identifier>", " <identifier>", " <identifier>", "", "", "<number>", "<number>", "", "", "", "( <expr> )", "", "", "", "", "", "", "", "" }, //CHECKED
/*<number>*/    { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "<sign> <digit> <Y>", "<sign> <digit> <Y>", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<Y>*/         { "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "<digit> <Y>", "", "", "", "", "NULL", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "NULL", "", "", "", "", "", "" }, // CHECKED
/*<sign>*/      { "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "", "", "+", "-", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<digit>*/     { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<letter>*/     { "", "", "", "", "", "", "", "", "", "", "p", "q", "r", "s", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
};      

// Predicting Parsing Table Headers
std::string language[MAX_LANGUAGE] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "p", "q", "r", "s", "/", "*", "+", "-", ";", ":", "=", "(", ",", ")", "\"", "program", "begin", "display", "integer", "end."};
std::string expressions[MAX_EXPRESSIONS] = {
    "<prog>", 
    "<identifier>",
    "<X>", 
    "<dec-list>", 
    "<dec>", 
    "<D>", 
    "<type>",
    "<stat-list>",
    "<L>", 
    "<stat>", 
    "<write>", 
    "<W>", 
    "<assign>", 
    "<expr", 
    "<expr'>",
    "<term>", 
    "<term'>", 
    "<factor>", 
    "<number>", 
    "<Y>", 
    "<sign>", 
    "<digit>", 
    "<letter>"
};

std::string validThings[] = {"<prog>", "<identifier>", "<X>", "<dec-list>", "<dec>", "<D>", "<type>",
                                            "<stat-list>","<L>", "<stat>", "<write>", "<W>", "<assign>", "<expr", "<expr>'",
                                            "<term>", "<factor>", "<number>", "<Y>", "<sign>", "<digit>", "<letter>",
                                            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                                      "p", "q", "r", "s", "/", "*", "+", "-", ";", ":", 
                                      "=", "(", ",", ")", "\"", "program", "begin", "display",
                                      "integer", "end.", "var"};

// Control Variable
bool isValid = true;

int main() 
{

    std::vector<std::string> newWord{"program", "s", ";", "var", "p", ",", "q", ":", "integer"};

    std::string word;
    std::stack<std::string> stack;
    int currentIndex = 0;

    // Get input from user
    std::cout << "Enter a postfix expression with $ at the end: ";
    std::getline(std::cin, word);

    // Initialize the stack
    // stack.push("$");
    stack.push(expressions[0]);

    // While the stack is not empty
    while (!stack.empty()) 
    {
        // Get the current expression
        std::string currentExpression = stack.top();

        // Check if the current expression is not garbage data
        int languageSize = sizeof(validThings) / sizeof(*validThings);
        bool isInvalidExpression = std::find(validThings, validThings + languageSize, currentExpression) == validThings + languageSize;
        if (isInvalidExpression) 
        {
            stack.pop();
            continue;
        }

        // Pop current expression from stack
        stack.pop();
        std::cout << "LOOKING FOR -> " << newWord[currentIndex] << "\n";
        std::cout << "POP -> " << currentExpression << " " << currentExpression.length() << std::endl;
 
        // Check if currentExpression is the target expression
        // If match, go to next iteration and search next character from word
        if (currentExpression == (std::string() + newWord[currentIndex])) 
        {
            std::cout << "MATCH: " << newWord[currentIndex] << "\n";
            std::cout << "Stack: -> ";
            PrintStack(stack);
            std::cout << std::endl;
            std::cout << std::endl;
            currentIndex++;
            continue;
        }
        
        // Read the current expression
        int expressionIndex = std::distance(expressions, std::find(expressions, expressions + MAX_EXPRESSIONS, currentExpression));
        int languageIndex = std::distance(language, std::find(language, language + MAX_LANGUAGE, newWord[currentIndex]));

        // Fetch the value of the current expression in the parsing table
        std::string parsingTableResult = parsingTable[expressionIndex][languageIndex];

        std::cout << "EXPRESSION INDEX -> " << expressionIndex << " LANGUAGE INDEX -> " << languageIndex << "\n";
        std::cout << "PUSH -> " << "[" << expressions[expressionIndex] << ", " << language[languageIndex] << "] = " << parsingTableResult << "\n";

        // If the parsing table result is lambda (skip in this case), skip to the next iteration
        if (parsingTableResult == "NULL") 
        {
            PrintStack(stack);
            std::cout << std::endl;
            std::cout << std::endl;
            continue;
        }

        // If the result from the parsing table is empty, expression is invalid
        if (parsingTableResult.empty()) 
        {
            isValid = false;
            break;
        } 
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

            // Else, Push in reverse the parsing table result expression to the stack

            // TODO: FIND A WAY TO SPLIT THE STRINGS PROPERLY

            // if (parsingTableResult.length() == 1) 
            // {
            //     stack.push(std::string() + parsingTableResult[0]);
            // } 
            // else 
            // {
            //     for (int i = newWord.size()-1; i>=0; i--) 
            //     {
            //         stack.push(std::string() + parsingTableResult[i]);
            //     }
            // }

        }

        std::cout << "Stack: -> ";
        PrintStack(stack);
        std::cout << std::endl;
        std::cout << std::endl;

        std::cin.ignore(999, '\n');
    }

    std::cout << word;
    (isValid) ? std::cout << " Valid Expression \n\n" : std::cout << " Invalid Expression \n\n";

    return 0;
}

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

void PrintStack(std::stack<std::string> s)
{
    // If stack is empty then return
    if (s.empty())
        return;
    
    std::string x = s.top();
 
    // Pop the top element of the stack
    s.pop();
 
    // Recursively call the function PrintStack
    PrintStack(s);
 
    // Print the stack element starting from the bottom
    std::cout << x;
 
    // Push the same element onto the stack to preserve the order
    s.push(x);
}
