//---------------------------------------------------------------------------------------------------
//  Group Name: Carlos Mora, Juan Sierra, Juli Nazzario
//  Assignment: Final Project
//  Due Date: 5/9/2023
//  Purpose: This program reads a predictive parsing table to trace if 
//  an input is valid or not given a language
//---------------------------------------------------------------------------------------------------
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <stack>

// Size of the predictive parsing table, update as required
#define MAX_EXPRESSIONS 21
#define MAX_LANGUAGE 30

// Function to print a stack
void PrintStack(std::stack<std::string> s);

// Predictive Parsing Table
std::string parsingTable[MAX_EXPRESSIONS][MAX_LANGUAGE] = 
{
/*<prog>*/      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "program <identifier>; var <dec-list> begin <stat-list> end.", "", "", "", "" },
/*<identifier*/ { "", "", "", "", "", "", "", "", "", "", "<letter><X>", "<letter><X>", "<letter><X>", "<letter><X>", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
/*<X>*/         { "<digit><X>", "<digit><X>", "<digit><X>", "<digit><X>", "<digit><X>", "<digit><X>", "<digit><X>", "<digit><X>", "<digit><X>", "<digit><X>", "<letter><X>", "<letter><X>", "<letter><X>", "<letter><X>", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "", "NULL", "NULL", "", "", "", "", "", "" },
/*<dec-list>*/  { "", "", "", "", "", "", "", "", "", "", "<dec> : <type>;", "<dec> : <type>;", "<dec> : <type>;", "<dec> : <type>;", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
/*<dec>*/       { "", "", "", "", "", "", "", "", "", "", "<identifier>, <dec>", "<identifier>, <dec>", "<identifier>, <dec>", "<identifier>, <dec>", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
/*<D>*/         { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "NULL", "", "", ",<dec>", "", "", "", "", "", "", "" },
/*<type*/       { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "integer", "" },
/*<stat-list*/  { "", "", "", "", "", "", "", "", "", "", "<stat><L>", "<stat><L>", "<stat><L>", "<stat><L>", "", "", "", "", "", "", "", "", "", "", "","", "", "<stat><L>", "", "" },
/*<L>*/         { "", "", "", "", "", "", "", "", "", "", "<stat-list>", "<stat-list>", "<stat-list>", "<stat-list>", "", "", "", "", "", "", "", "", "", "", "", "", "", "<stat-list>", "", "NULL" },
/*<stat*/       { "", "", "", "", "", "", "", "", "", "", "<assign>", "<assign>", "<assign>", "<assign>", "", "", "", "", "", "", "", "", "", "", "", "", "", "<write>", "", "" },
/*<write*/      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "display(<identifier>); display(\"value\", <identifier>);", "", "" },
/*<W>*/         { "", "", "", "", "", "", "", "", "", "", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "", "", "", "", "", "", "“value”,", "", "", "", "", "" },
/*<assign>*/    { "", "", "", "", "", "", "", "", "", "", "<identifier> = <expr>;", "<identifier> = <expr>;", "<identifier> = <expr>;", "<identifier> = <expr>;", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
/*<expr>*/      { "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "<term><expr’>", "", "", "<term><expr’>", "<term><expr’>", "", "", "", "<term><expr’>", "", "", "", "", "", "", "", "" },
/*<expr'>*/     { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "-<term><expr’> ", "-<term><expr’> ", "NULL", "", "", "", "", "NULL", "", "", "", "", "", "" },
/*term>*/       { "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "<factor><term’>", "", "", "<factor><term’>", "<factor><term’>", "", "", "", "<factor><term’>", "", "", "", "", "", "", "", "" },
/*<term'*/      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "/<factor><term’> ", "*<factor><term’> ", "NULL", "NULL", "NULL", "", "", "", "", "NULL", "", "", "", "", "", "" },
/*<factor*/     { "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", " <identifier>", " <identifier>", " <identifier>", " <identifier>", "", "", "<number>", "<number>", "", "", "", "(<expr>)", "", "", "", "", "", "", "", "" },
/*<number>*/    { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "<sign><digit><Y>", "<sign><digit><Y>", "", "", "", "", "", "", "", "", "", "", "", "" },
/*<Y>*/         { "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "", "", "", "", "NULL", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "NULL", "", "", "", "", "", "" },
/*<sign>*/      { "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "", "", "+", "-", "", "", "", "", "", "", "", "", "", "", "", "" },
/*<digit>*/     { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
/*<letter*/     { "", "", "", "", "", "", "", "", "", "", "p", "q", "r", "s", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
};

// Predicting Parsing Table Headers
std::string expressions[MAX_EXPRESSIONS] = {"<prog>", "<identifier>", "<X>", "<dec-list>", "<dec>", "<D>", "<type>",
                                            "<stat-list>","<L>", "<stat>", "<write>", "<W>" "<assign>", "<expr", "<expr>'",
                                            "<term>", "<factor>", "<number>", "<Y>", "<sign>", "<digit>", "<letter>"};

std::string language[MAX_LANGUAGE] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                                      "p", "q", "r", "s", "/", "*", "+", "-", ";", ":", 
                                      "=", "(", ",", ")", "\"", "program", "begin", "display",
                                      "integer", "end."};

// Control Variable
bool isValid = true;

int main() 
{
    std::string word;
    std::stack<std::string> stack;
    int currentIndex = 0;

    // Get input from user
    std::cout << "Enter a postfix expression with $ at the end: ";
    std::getline(std::cin, word);

    // Initialize the stack
    stack.push("$");
    stack.push(expressions[0]);

    // While the stack is not empty
    while (!stack.empty()) 
    {
        // Get the current expression
        std::string currentExpression = stack.top();

        // Check if the current expression is not garbage data
        if (currentExpression == "\0") 
        {
            stack.pop();
            continue;
        }

        // Print current stack
        // std::cout << "Stack: -> ";
        // PrintStack(stack);
        // std::cout << std::endl;

        // Pop current expression from stack
        stack.pop();
        // std::cout << "POP -> " << currentExpression << std::endl;
 
        // Check if currentExpression is the target expression
        // If match, go to next iteration and search next character from word
        if (currentExpression == word) 
        {
            std::cout << "Stack: -> ";
            PrintStack(stack);
            std::cout << std::endl;

            std::cout << "MATCH: " << word[currentIndex] << "\n";
            currentIndex++;
            std::cout << std::endl;
            continue;
        }
        
        // Read the current expression
        int expressionIndex = std::distance(expressions, std::find(expressions, expressions + MAX_EXPRESSIONS, currentExpression));
        int languageIndex = std::distance(language, std::find(language, language + MAX_LANGUAGE, word[currentIndex]));

        // Fetch the value of the current expression in the parsing table
        std::string parsingTableResult = parsingTable[expressionIndex][languageIndex];
        // std::cout << "PUSH -> " << "[" << expressions[expressionIndex] << ", " << language[languageIndex] << "] = " << parsingTableResult << "\n";

        // If the parsing table result is lambda (skip in this case), skip to the next iteration
        if (parsingTableResult == "skip") 
        {
            // std::cout << std::endl;
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
            // Else, Push in reverse the parsing table result expression to the stack
            if (parsingTableResult.length() == 1) 
            {
                stack.push(parsingTableResult);
            } 
            else 
            {
                for (int i = word.length()-1; i>=0; i--) 
                {
                    stack.push(parsingTableResult);
                }
            }
        }

        // std::cout << std::endl;
    }

    std::cout << word;
    (isValid) ? std::cout << " Valid Expression \n\n" : std::cout << " Invalid Expression \n\n";

    return 0;
}

void PrintStack(std::stack<char> s)
{
    // If stack is empty then return
    if (s.empty())
        return;
    
    char x = s.top();
 
    // Pop the top element of the stack
    s.pop();
 
    // Recursively call the function PrintStack
    PrintStack(s);
 
    // Print the stack element starting from the bottom
    std::cout << x;
 
    // Push the same element onto the stack to preserve the order
    s.push(x);
}
