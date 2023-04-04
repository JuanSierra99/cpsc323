//---------------------------------------------------------------------------------------------------
//  Group Name: Carlos Mora, Juan Sierra, Juli Nazzario
//  Assignment: No 8
//  Due Date: 4/6/2023
//  Purpose: This program reads a LR Parsing table to trace if a string is valid
//---------------------------------------------------------------------------------------------------
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <stack>

// Size of the predictive parsing table, update as required
#define MAX_STATES 16
#define MAX_LANGUAGE 11

// Function to print a stack
void PrintStack(std::stack<std::string> s);

// Input data
std::string parsingTable[MAX_STATES][MAX_LANGUAGE] = 
{
    {"S5", "", "", "", "", "S4", "", "", "1", "2", "3"},
    {"", "S6", "S7", "", "", "", "", "ACC", "", "", ""},
    {"", "R3", "R3", "S8", "S9", "", "R3", "R3", "", "", ""},
    {"", "R6", "R6", "R6", "R6", "", "R6", "R6", "", "", ""},
    {"S5", "", "", "", "", "S4", "", "", "10", "2", "3"},
    {"", "R8", "R8", "R8", "R8", "", "R8", "R8", "", "", ""},
    {"S5", "", "", "", "", "S4", "", "", "", "11", "3"},
    {"S5", "", "", "", "", "S4", "", "", "", "12", "3"},
    {"S5", "", "", "", "", "S4", "", "", "", "", "13"},
    {"S5", "", "", "", "", "S4", "", "", "", "", "14"},
    {"", "S6", "S7", "", "", "", "S15", "", "", "", ""},
    {"", "R1", "R1", "S8", "S9", "", "R1", "R1", "", "", ""},
    {"", "R2", "R2", "S8", "S9", "", "R2", "R2", "", "", ""},
    {"", "R4", "R4", "R4", "R4", "", "R4", "R3", "", "", ""},
    {"", "R5", "R5", "R5", "R5", "", "R5", "R5", "", "", ""},
    {"", "R7", "R7", "R7", "R7", "", "R7", "R7", "", "", ""}
};

std::string states[MAX_STATES] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};
std::string language[MAX_LANGUAGE] = {"a", "+", "-", "*", "/", "(", ")", "$", "E", "T", "F"};
int expressionsLen[8] = { 3, 3, 1, 3, 3, 1, 3, 1 };

// Control Variable
int currentIndex = 0;
bool isValid = true;
bool getNextIndex = false;

int main() 
{
    std::string word;
    std::stack<std::string> stack;
    int currentIndex = 0;

    // Get input from user
    std::cout << "Enter a postfix expression with $ at the end: ";
    std::getline(std::cin, word);

    // Initialize the stack
    stack.push("0");

    while(!stack.empty()) 
    {
        // Pop the first element to get the current expression
        std::string currentExpression = stack.top();
        stack.pop();

        // Check if the current expression is not garbage data
        if (currentExpression.empty()) 
        {
            stack.pop();
            continue;
        }

        // Check if you can proceed to trace the next character from the word
        if (getNextIndex) 
        {
            currentIndex++;
        }

        // std::cout << "POP: " << currentExpression << "\n";

        // Read the current state
        int stateIndex = std::distance(states, std::find(states, states + MAX_STATES, currentExpression));
        int languageIndex = std::distance(language, std::find(language, language + MAX_LANGUAGE, std::string() + word[currentIndex]));
        
        std::string currentStateResult = parsingTable[stateIndex][languageIndex];
        // std::cout << "GOTO: " << "[" + states[stateIndex] + "," + language[languageIndex] + "] = " << currentStateResult + "\n";

        // If the state result from the parsing table is empty, expression is invalid
        if (currentStateResult.empty()) 
        {
            isValid = false;
            break;
        }

        // Print control values
        if (getNextIndex) 
        {
            std::cout << "READ:" << word[currentIndex] << "\n";
            std::cout << "Stack -> ";
            PrintStack(stack);
            std::cout << "\n\n";
        }

        // From state result, check if result is case S, case R or accepted
        // CASE S
        if (currentStateResult[0] == 'S') 
        {
            // Push to the stack the currentState
            stack.push(states[stateIndex]);
            // Push to the stack the current character from word
            stack.push(language[languageIndex]);
            // Push to the stack the current state result ignoring the first character
            stack.push(currentStateResult.substr(1, currentStateResult.size() - 1));
            // Flag to trace the next character from word
            getNextIndex = true;

        }
        // CASE R
        else if (currentStateResult[0] == 'R') 
        {
            // Push back again the current expression
            stack.push(std::string() + currentExpression);

            // Remove items from stack based on the length of the expressions in the language
            int itemsToRemove = expressionsLen[stoi(currentStateResult.substr(1, currentStateResult.size() - 1)) - 1];

            for (int i=0; i<itemsToRemove*2; i++)
                stack.pop();

            // Load a buffer expression
            std::string bufferExpression = stack.top();
            stack.pop();

            // Get the non terminal character to push in the stack
            std::string nonTerminal;
            
            if (stoi(currentStateResult.substr(1, currentStateResult.size() - 1)) <= 3) 
                nonTerminal = "E";
            else if (stoi(currentStateResult.substr(1, currentStateResult.size() - 1)) >= 4 && stoi(currentStateResult.substr(1, currentStateResult.size() - 1)) <= 6)
                nonTerminal = "T";
            else 
                nonTerminal = "F";

            // Get the buffer state result to push in the stack
            int bufferExpressionIndex = stoi(bufferExpression);
            int bufferStateIndex = std::distance(language, std::find(language, language + MAX_LANGUAGE, nonTerminal));

            // Push the items to the stack
            stack.push(bufferExpression);
            stack.push(nonTerminal);
            stack.push(parsingTable[bufferExpressionIndex][bufferStateIndex]);

            // Flag to not trace the next character from word
            getNextIndex = false;

        } 
        // CASE: Accepted
        else 
        {
            break;
        }

        // PrintStack(stack);
        // std::cout << "\n";
        // std::cin.ignore(999, '\n');
    }
    
    std::cout << word;
    (isValid) ? std::cout << " Valid Expression \n\n" : std::cout << " Invalid Expression \n\n";
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
