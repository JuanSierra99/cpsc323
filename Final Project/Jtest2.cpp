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
//Function to help check for errors
int Parse(std::stack<std::string> stackCopy, int i, std::string lan, std::vector<std::string> newWordCopy);
std::vector<std::string> ParsingTableTokenizer(std::string s);

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

std::string validThings[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "p", "q", "r", "s", "/", "*", "+", "-", ";", ":", "=", "(", ",", ")", "\"", "program", "begin", "display", "integer", "end.", "value", "var",
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

// Control Variable
bool isValid = true;
int main() 
{
    // std::vector<std::string> newWord{"program", "s", "2", "0","2", "3", "var", "p", "1", ",", "p", "2", "q", "p", "r", ":", "integer", ";", "begin", "p", "1", "=", "3", "9", ";", "p", "2", "q", "=", "4", "1", "2", ";", "p", "r", "=", "p", "1", "+", "p", "2", "q", ";", "display", "(", "p", "r", ")", ";", "p", "r", "=", "p", "1", "*", "(", "p", "2", "q", "+", "2", "*", "p", "r", ")", ";", "display", "(", "\"", "value", "=", "\"", ",", "p", "r", ")", ";", "end."};
    std::vector<std::string> newWord{"program", "p", "2", ";", "var", "p", "1", ",", "q", "2", ":", "integer", ";", "begin", "p", "1", "=", "3", "3", "+", "(", "2", "/", "3", "*", "2", ";", "display", "(", "\"", "value", "=", "\"", ",", "p", "1", ")", ";", "end."};
    std::stack<std::string> stack;
    int currentIndex = 0;

    // Initialize the stack
    // stack.push("$");
    stack.push(expressions[0]);
    // While the stack is not empty

    std::vector<std::string> check{"program", "var", "integer", "begin", "end."};
    // look at the reserved words in check and make sure they are written in the example program. These words only show up once and must be included
    for (auto reserved: check){
        if (std::find(newWord.begin(), newWord.end(), reserved) == newWord.end()){
            std::cout << reserved << " is expected \n";
            return 0;
        }
    }

    while (!stack.empty()) 
    {
        // Get the current expression
        std::string currentExpression = stack.top();

        // Check if the current expression is not garbage data
        int languageSize = sizeof(validThings) / sizeof(*validThings);
        bool isInvalidExpression = std::find(validThings, validThings + languageSize, currentExpression) == validThings + languageSize;
        if (isInvalidExpression) 
        {
            std::cout << "INVALID -> " << currentExpression << "\n";
            stack.pop();
            continue;
        }

        // Pop current expression from stack
        std::string lastExpression = stack.top();
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

            // std::cin.ignore(999, '\n');

            continue;
        }
        
        // Read the current expression
        int expressionIndex = std::distance(expressions, std::find(expressions, expressions + MAX_EXPRESSIONS, currentExpression));
        int languageIndex = std::distance(language, std::find(language, language + MAX_LANGUAGE, newWord[currentIndex]));

        // Fetch the value of the current expression in the parsing table
        std::string parsingTableResult = parsingTable[expressionIndex][languageIndex];

        std::cout << "EXPRESSION INDEX -> " << expressionIndex << " LANGUAGE INDEX -> " << languageIndex << "\n";

        // index out of bounds errors
        // if (expressionIndex == 23){
        //         std::cout << "-------------------------------------------------------------------\n";
        //         std::cout << "ERROR\n";
        //         std::cout << "-------------------------------------------------------------------\n";
        // }
        // Index out of bounds error
        if (languageIndex == 30 || expressionIndex == 23 || parsingTableResult.empty()){
                languageIndex = 0;
                 while (languageIndex < 30)
                 {
                    std::stack<std::string> stackCopy = stack;
                    std::vector<std::string> newWordCopy = newWord;
                    newWordCopy.insert(newWordCopy.begin() + currentIndex, language[languageIndex]);
                    stackCopy.push(lastExpression);
                    parsingTableResult = parsingTable[expressionIndex][languageIndex];
                    if (!parsingTableResult.empty()){
                        std::cout << "try this: " << language[languageIndex] << " with " << lastExpression << std::endl;
                        // stackCopy.push(parsingTableResult);
                        int max = Parse(stackCopy, currentIndex, language[languageIndex], newWordCopy);
                        if(max == 0){
                            std::cout << language[languageIndex] << " is missing. error\n";
                            exit(0);
                        }
                        std::cout << max << std::endl;
                    }
                    languageIndex++;
                }
            }
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
        }

        std::cout << "Stack: -> ";
        PrintStack(stack);
        std::cout << std::endl;
        std::cout << std::endl;

        // std::cin.ignore(999, '\n');
    }
    (isValid) ? std::cout << " Valid Expression \n\n" : std::cout << " Invalid Expression \n\n";

    return 0;
}

int Parse(std::stack<std::string> stackCopy, int i, std::string lan, std::vector<std::string> newWordCopy){
    bool nowValid = true;
    std::cout << "-------------------------------------------------------------------\n";
    std::cout << "WE ARE IN PARSE NOW\n";
    std::cout << "-------------------------------------------------------------------\n";
    std::vector<std::string> newWord = newWordCopy;
    int currentIndex = i;
    while (!stackCopy.empty()) 
    {
        // Get the current expression
        std::string currentExpression = stackCopy.top();

        // Check if the current expression is not garbage data
        int languageSize = sizeof(validThings) / sizeof(*validThings);
        bool isInvalidExpression = std::find(validThings, validThings + languageSize, currentExpression) == validThings + languageSize;
        if (isInvalidExpression) 
        {
            std::cout << "INVALID -> " << currentExpression << "\n";
            stackCopy.pop();
            continue;
        }

        // Pop current expression from stack
        stackCopy.pop();
        std::cout << "LOOKING FOR -> " << newWord[currentIndex] << "\n";
        std::cout << "POP -> " << currentExpression << " " << currentExpression.length() << std::endl;
 
        // Check if currentExpression is the target expression
        // If match, go to next iteration and search next character from word
        if (currentExpression == (std::string() + newWord[currentIndex])) 
        {
            std::cout << "MATCH: " << newWord[currentIndex] << "\n";
            std::cout << "Stack: -> ";
            PrintStack(stackCopy);
            std::cout << std::endl;
            std::cout << std::endl;
            currentIndex++;

            // std::cin.ignore(999, '\n');

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
            PrintStack(stackCopy);
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
                stackCopy.push(tempVector[0]);
            }
            else 
            {
                for (int i = tempVector.size()-1; i>=0; i--) 
                {
                    stackCopy.push(tempVector[i]);
                }
            }
        }

        std::cout << "Stack: -> ";
        PrintStack(stackCopy);
        std::cout << std::endl;
        std::cout << std::endl;

        // std::cin.ignore(999, '\n');
    }
    return stackCopy.size();
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