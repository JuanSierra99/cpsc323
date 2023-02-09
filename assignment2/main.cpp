//---------------------------------------------------------------------------------------------------
//  Group Name: Carlos Mora, Juan Sierra, Juli Nazzario
//  Assignment: No 1
//  Due Date: 2/2/2023
//  Purpose: This program reads in a postfix expression, evaluates the expression, and displays its value
//---------------------------------------------------------------------------------------------------
#include <iostream>
#include <istream>
#include <string>
#include <cstring>
#include <stack>

//---------------------------------------------------------------------------------------------------
    int postfixCalc(char expression[])
{
        char* token = strtok(expression, " ");
        while (token != NULL)
        {
            std::cout << token << std::endl;
            token = strtok(NULL, " ");
        }
        return 0;
        // std::stack<int> numStack;
        // for(int i = 0; i < expression.length()-1; i++)
        // {
        //     // Creates the stack of numbers only
        //     if(expression[i] == 'a')       numStack.push(5);
        //     else if (expression[i] == 'b') numStack.push(7);
        //     else if (expression[i] == 'c') numStack.push(2);
        //     else if (expression[i] == 'd') numStack.push(4);
        // else{
        //         // Assigns the top of the stack into a variable to use it mathematically and then pops it
        //         int num1 = numStack.top();
        //         numStack.pop();

        //         // Assigns the new top of the into a variable to use it mathematically and then pops it
        //         int num2 = numStack.top();
        //         numStack.pop();
                
        //         // makes the operator perform its operation
        //         switch (expression[i]) {
        //             case '+':
        //                 numStack.push(num2 + num1); break;
        //             case '-':
        //                 numStack.push(num2 - num1); break;
        //             case '*':
        //                 numStack.push(num2 * num1); break;
        //             case '/':
        //                 numStack.push(num2 / num1); break;
        //             case '$': break; }
        //     }
        // }
        // return numStack.top();
}
//---------------------------------------------------------------------------------------------------
int main() {
    int result = 0;
    char expression[] = "20 jerry 45 + tom - * $";
    std::string next;
    
    while(next != "n")
    {
        // Input - gets the expression input by the user
        std::cout << "Enter a postfix expression with $ at the end: ";
        std::cout << expression << std::endl; 
        std::cin.ignore(10000,'\n');
        
        // Processing - calculates the postfix expression
        result = postfixCalc(expression);
        std::cout << "\tValue = " << result << std::endl;
        
        std::cout << "CONTINUE(y/n)? ";
        getline(std::cin, next);
    }
    return 0;
}