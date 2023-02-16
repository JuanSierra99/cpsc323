//---------------------------------------------------------------------------------------------------
//  Group Name: Carlos Mora, Juan Sierra, Juli Nazzario
//  Assignment: No 3
//  Due Date: 2/16/2023
//  Purpose: This program reads a transition table to determine if an input is valid or not given a language
//---------------------------------------------------------------------------------------------------
#include <iostream>
#include <string>

//---------------------------------------------------------------------------------------------------
void checkLanguage(std::string w) {

    // Define the values of the transition table
    int t[4][3] = { 0, 1, 2, 2, 1, 3, 0, 3, 3, 1, 3, 2 };
    int state = 0, i=0, col;

    // Iterate through all the word minus the stop -> $
    while (i < w.length()-1) {
        // Switch the value of col depending the current input
        switch(w[i]) {
            case 'a':
                col = 0;
                break;
            case 'b':
                col = 1;
                break;
            case 'c':
                col = 2;
            default:
                break;
        }

        // Calculate the current state
        state = t[state][col];

        // Iterate to the next char of the word
        ++i;

        // cout << "w[i] = " << w[i] << " state: " << state << endl;
    }

    // Check if the word is valid or not with the given language
    if (state == 1 || state == 2)
        std::cout << "accepted" << std::endl;
    else
        std::cout << "rejected" << std::endl;
}

int main() {
    // Loop to iterate all the examples
    for (int i=0; i<3; i++ ) {

        // Input - gets the input by the user
        std::string w;
        std::cin >> w;

        // Processing - Evaluates the user input with the transition table of the language
        checkLanguage(w);
    }

    return 0;
}
