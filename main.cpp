//---------------------------------------------------------------------------------------------------
//  Group Name: 
//  Assignment: No 1
//  Due Date: 2/2/2023
//  Purpose: This program reads an expression in postfix form, evaluates the expression,
//  and displays its value
//---------------------------------------------------------------------------------------------------

#include <iostream>
#include <istream>
#include <string>
#include <stack>

// This is supposed to translate the characters input by the user into numerical values but it is not working :(
int getValue(char c)
{
    switch (c) {
        case 'a':
            return 5;
            break;
        case 'b':
            return 7;
            break;
        case 'c':
            return 2;
            break;
        case 'd':
            return 4;
            break;
            
        default: return -1;
            break;
    }
    
}
//---------------------------------------------------------------------------------------------------
    int postfixCalc(std::string expression)
{
        std::stack<int> stackStack;

        for(int i = 0; i < expression.length(); i++)
        {
            // Creates the stack of numbers only
            if(isdigit(expression[i]))
            {
                int num = getValue(expression[i]);
                stackStack.push(num);
            }else
            {
                // Assigns the top of the stack into a variable to use it mathematically and then pops it
                int num1 = stackStack.top();
                stackStack.pop();

                // Assigns the new top of the into a variable to use it mathematically and then pops it
                int num2 = stackStack.top();
                stackStack.pop();
                
                // makes the operator perform its operation
                switch (expression[i]) {
                    case '+':
                        stackStack.push(num1 + num2);
                        break;
                    case '-':
                        stackStack.push(num1 - num2);
                        break;
                    case '*':
                        stackStack.push(num1 * num2);
                        break;
                    case '/':
                        stackStack.push(num1 / num2);
                        break;
                    case '$':
                        break;
                }
            }
        }

        return stackStack.top();
}
//---------------------------------------------------------------------------------------------------
int main() {
    int result = 0;
    std::string expression;
    std::string continuee;
    
    while(continuee != "n")
    {
        // Input - gets the expression input by the user
        std::cout << "Enter a postfix expression with $ at the end: ";
        std::cin  >> expression;
        std::cin.ignore(10000,'\n');
        
        // Processing - calculates the postfix expression
        result = postfixCalc(expression);
        
        std::cout << "\tValue = " << result << std::endl;
        
        std::cout << "CONTINUE(y/n)? ";
        getline(std::cin, continuee);
    }
    return 0;
}
