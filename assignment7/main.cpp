//---------------------------------------------------------------------------------------------------
//  Group Name: Carlos Mora, Juan Sierra, Juli Nazzario
//  Assignment: No 7
//  Due Date: 3/23/2023
//  Purpose: This program reads a predictive parsing table to trace if an input is valid or not given a language
//---------------------------------------------------------------------------------------------------

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <stack>

// Function to print a stack
void PrintStack(std::stack<char> s);

std::string parsingTable[5][8] = 
{
    { "TQ",  "",     "",     "",    "",    "TQ",   "",     "" },
    { "",    "+TQ",  "-TQ",  "",    "",    "",     "skip", "skip" },
    { "FR",  "",     "",     "",    "",    "FR",   "",     ""},
    { "",    "skip", "skip", "*FR", "/FR", "",     "skip", "skip" },
    { "i",   "",     "",     "",    "",    "(E)",  "",     "" }
};

char expressions[5] = {'E', 'Q', 'T', 'R', 'F'};
char language[8] = {'i', '+', '-', '*', '/', '(', ')', '$'};

int main() 
{
    std::string word;
    std::stack<char> stack;
    int currentIndex = 0;

    std::cout << "Enter a postfix expression with $ at the end: ";
    std::getline(std::cin, word);

    // Initialize the stack
    stack.push(language[7]);
    stack.push(expressions[0]);

    while (!stack.empty()) 
    {
        // Get the current expression
        char currentExpression = stack.top();

        // Check if the current expression is not garbage data
        if (currentExpression == '\0') 
        {
            stack.pop();
            continue;
        }

        // Print current stack
        std::cout << "Stack: -> ";
        PrintStack(stack);
        std::cout << std::endl;

        // Pop current expression from stack
        stack.pop();
        std::cout << "POP -> " << currentExpression << std::endl;
 
        // Check if currentExpression is the target expression
        if (currentExpression == word[currentIndex]) 
        {
            std::cout << "FOUND: " << word[currentIndex] << "\n";
            currentIndex++;
            std::cout << std::endl;
            continue;
        }
        
        // Read the current expression
        int expressionIndex = std::distance(expressions, std::find(expressions, expressions+5, currentExpression));
        int languageIndex = std::distance(language, std::find(language, language+8, word[currentIndex]));

        // Fetch the value of the current expression in the parsing table
        std::string parsingTableResult = parsingTable[expressionIndex][languageIndex];
        std::cout << "[" << expressions[expressionIndex] << ", " << language[languageIndex] << "] = " << parsingTableResult << "\n";

        // If the parsing table result is lambda (skip in this case), skip to the next iteration
        if (parsingTableResult == "skip") 
        {
            std::cout << std::endl;
            continue;
        }

        // If the result from the parsing table is empty
        if (parsingTableResult.empty()) 
        {
            std::cout << "Invalid Expression \n";
            break;
        } else 
        {
            // Else, Push in reverse the parsing table result expression to the stack
            if (parsingTableResult.length() == 1) 
            {
                stack.push(parsingTableResult[0]);
            } else 
            {
                for (int i = word.length()-1; i>=0; i--) 
                {
                    stack.push(parsingTableResult[i]);
                }
            }
        }

        std::cout << std::endl;
    }

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