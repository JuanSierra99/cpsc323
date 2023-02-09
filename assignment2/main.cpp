//---------------------------------------------------------------------------------------------------
//  Group Name: Carlos Mora, Juan Sierra, Juli Nazzario
//  Assignment: No 2
//  Due Date: 2/08/2023
//  Purpose: This program reads in a postfix expression, evaluates the expression, and displays its value
//---------------------------------------------------------------------------------------------------
#include <iostream>
#include <istream>
#include <string>
#include <cstring>
#include <stack>

//---------------------------------------------------------------------------------------------------
    int postfixCalc(const char* expression)
{
        int value = 0;  // to store value of variables in expression
        char* token = std::strtok(const_cast<char*>(expression), " ");
        std::stack<int> numStack;
        while (token != NULL && token[0]!='$')
        {
            // std::cout << token << std::endl;
            if (isalpha(token[0])){
                std::cout << "Enter the value for " << token << " ";
                std::cin >> value;
                numStack.push(value);
            }
            else if (isdigit(token[0])){
                numStack.push(std::stoi(token));
            }
            else{
                // Assigns the top of the stack into a variable to use it mathematically and then pops it
                int num1 = numStack.top();
                numStack.pop();

                // Assigns the new top of the into a variable to use it mathematically and then pops it
                int num2 = numStack.top();
                numStack.pop();
                
                // makes the operator perform its operation
                switch (token[0]) {
                    case '+':
                        numStack.push(num2 + num1); break;
                    case '-':
                        numStack.push(num2 - num1); break;
                    case '*':
                        numStack.push(num2 * num1); break;
                    case '/':
                        numStack.push(num2 / num1); break;
                    case '$':
                        break; }
            }
            token = strtok(NULL, " ");
        }
        return numStack.top();
}
//---------------------------------------------------------------------------------------------------
int main() {
    int result = 0;
    std::string expression;
    std::string next;
    
    while(next != "n")
    {
        // Input - gets the expression input by the user
        std::cout << "Enter a postfix expression with $ at the end: ";
        std::getline(std::cin, expression);
        
        // Processing - calculates the postfix expression
        result = postfixCalc(expression.c_str());
        std::cout << "\tValue = " << result << std::endl;
        std::cin.ignore(100000, '\n');
        
        std::cout << "CONTINUE(y/n)? ";
        getline(std::cin, next);
    }
    return 0;
}
