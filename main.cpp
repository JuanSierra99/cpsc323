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

//---------------------------------------------------------------------------------------------------
    int postfixCalc(std::string expression)
{
        std::stack<int> numStack;

        for(int i = 0; i < expression.length()-1; i++)
        {
            // Converts the string characters into numbers
            if(expression[i] == 'a')
                numStack.push(5);
            else if (expression[i] == 'b')
                numStack.push(7);
            else if (expression[i] == 'c')
                numStack.push(2);
            else if (expression[i] =='d')
                numStack.push(4);
        else{
                // Assigns the top of the stack into a variable to use it mathematically and then pops it
                int num1 = numStack.top();
                numStack.pop();

                // Assigns the new top of the into a variable to use it mathematically and then pops it
                int num2 = numStack.top();
                numStack.pop();
                
                // makes the operator perform its operation
                switch (expression[i]) {
                    case '+':
                        numStack.push(num1 + num2); break;
                    case '-':
                        numStack.push(num1 - num2); break;
                    case '*':
                        numStack.push(num1 * num2); break;
                    case '/':
                        numStack.push(num1 / num2); break;
                    case '$': break;
                }
            }
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
        std::cin  >> expression;
        std::cin.ignore(10000,'\n');
        
        // Processing - calculates the postfix expression
        result = postfixCalc(expression);
        
        std::cout << "\tValue = " << result << std::endl;
        
        std::cout << "CONTINUE(y/n)? ";
        getline(std::cin, next);
    }
    return 0;
}